#ifndef REZ_COMPARABLE_HPP
#define REZ_COMPARABLE_HPP

#include "Types.hpp"

#include <type_traits>

constexpr bool NORMAL = false;
constexpr bool REVERSED = true;
constexpr bool DEFAULT = NORMAL;

//
// Comparable reversible arbitrary value
//
// Explicit constructor allow conversion from Comparable<T, true> to Comparable<T, false>,
// Conversion from normal and reverse comparable is not free, it requires copy.
//
template<typename _Typ, bool _Rev = DEFAULT> struct Comparable
{
    using value_type = _Typ;

    // constructor
    template<typename... Args> explicit Comparable(Args&&... args)
        : _value{_Typ{std::forward<Args>(args)...}}
    {
    }

    // converting constructor
    template<bool _> explicit Comparable(const Comparable<value_type, _>& other)
        : _value{other.Get()}
    {
    }

    // data access
    const value_type& Get() const REZ_NOEXCEPT { return _value; }
    value_type& Get()  REZ_NOEXCEPT { return _value; }

    // operator->
    const value_type* operator->() const REZ_NOEXCEPT { return &Get; }
    value_type* operator->() REZ_NOEXCEPT { return &Get(); }

    // operator*
    const value_type& operator*() const REZ_NOEXCEPT { return Get(); }
    value_type& operator*() REZ_NOEXCEPT { return Get(); }

    // operator<
    template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return (Get() < other.Get());
    }

    // operator<
    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return (Get() < other);
    }

    // operator==
    template<bool _> bool operator==(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return (Get() == other.Get());
    }

    // operator==
    bool operator==(const value_type& other) const REZ_NOEXCEPT
    {
        return (Get() == other);
    }

private:
    value_type _value;
};

//
// Partial specialization for ReversedComparable
//
template<typename _Typ> struct Comparable<_Typ, REVERSED>
{
    using value_type = _Typ;

    // constructor
    template<typename... Args> explicit Comparable(Args&&... args)
        : _value{_Typ{std::forward<Args>(args)...}}
    {
    }

    // converting constructor
    template<bool _> explicit Comparable(const Comparable<value_type, _>& other)
        : _value{other.Get()}
    {
    }

    // data access
    const value_type& Get() const REZ_NOEXCEPT { return _value; }
    value_type& Get()  REZ_NOEXCEPT { return _value; }

    // operator->
    const value_type* operator->() const REZ_NOEXCEPT { return &Get(); }
    value_type* operator->() REZ_NOEXCEPT { return &Get(); }

    // operator*
    const value_type& operator*() const REZ_NOEXCEPT { return Get(); }
    value_type& operator*() REZ_NOEXCEPT { return Get(); }

    // operator<
    template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return !(Get() < other.Get());
    }

    // operator<
    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return !(Get() < other);
    }

    // operator==
    template<bool _> bool operator==(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return !(Get() == other.Get());
    }

    // operator==
    bool operator==(const value_type& other) const REZ_NOEXCEPT
    {
        return !(Get() == other);
    }

private:
    value_type _value;
};

template<typename _Typ> using ReversedComparable = Comparable<_Typ, REVERSED>;

//
// switches normal comparable <=> reversed comparable, returns a copy! bool template parameter is flipped.
//
template<typename _Typ, bool _Rev> Comparable<_Typ, !_Rev> reverse_sort_key(const Comparable<_Typ, _Rev>& other)
{
    return Comparable<_Typ,!_Rev>{other};
}

template<typename _Typ, bool _Rev> Comparable<_Typ, !_Rev> reverse_sort_key(Comparable<_Typ, _Rev>&& other)
{
    return Comparable<_Typ,!_Rev>{std::move(other)};
}

//
// is_comparable type
//
template<typename _Typ> struct is_comparable : std::false_type
{
};

template<typename _Typ, bool _Rev> struct is_comparable<Comparable<_Typ, _Rev>> : std::true_type
{
};

//
// is reversed comparable type
//
template<typename _Typ> struct is_reversed : std::false_type
{
    static_assert(is_comparable<_Typ>::value, "Invalid template parameter, expected Comparable!");
};

template<typename _Typ> struct is_reversed<Comparable<_Typ, false>> : std::false_type
{
};

template<typename _Typ> struct is_reversed<Comparable<_Typ, true>> : std::true_type
{
};

//
// operator<<
//
template<typename _Typ, bool _Rev> std::ostream& operator<<(std::ostream& os, const Comparable<_Typ, _Rev>& cv)
{
    os << std::boolalpha << "Comparable: [ reversed: " << _Rev << ", value: " << cv.Get() << " ]";
    return os;
}

//
// Comparable Factory, specialize to provide custom constructor
//
template<typename _Typ> struct Factory
{
    static_assert(is_comparable<_Typ>::value, "Invalid template parameter, expected Comparable!");

    using value_type = typename _Typ::value_type;

    template<typename... Args> static Comparable<value_type, is_reversed<_Typ>::value> Create(Args&&... args)
    {
        return Comparable<value_type, is_reversed<_Typ>::value>{std::forward<Args>(args)...};
    }
};

#endif // REZ_COMPARABLE_HPP
