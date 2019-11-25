#ifndef REZ_COMPARABLE_HPP
#define REZ_COMPARABLE_HPP

#include "Types.hpp"

constexpr bool NOR_CMP = false;
constexpr bool REV_CMP = true;
constexpr bool DEF_CMP = NOR_CMP;

//
// Comparable reversible arbitrary value
//
// Explicit constructor constructor allow conversion from Comparable<T, true> to Comparable<T, false>,
// Conversion from normal and reverse comparable is not free, it requires copy.
//
template<typename _Typ, bool _Rev = DEF_CMP> struct Comparable
{
    using value_type = _Typ;
    static constexpr bool reversed = _Rev;

    // constructor
    template<typename... Args> explicit Comparable(Args&&... args)
        : _value{_Typ{std::forward<Args>(args)...}}
    {
    }

    // data access
    const value_type& Get() const REZ_NOEXCEPT { return _value; }
    value_type& Get()  REZ_NOEXCEPT { return _value; }

    // converters
    template<bool _> explicit Comparable(const Comparable<value_type, _>& other)
        : _value{other.Get()}
    {
    }

    // comparision Comparable
    template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return (Get() < other.Get());
    }

    // comparision value_type
    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return (Get() < other);
    }

private:
    value_type _value;
};

//
// Partial specialization for ReversedComparable
//
template<typename _Typ> struct Comparable<_Typ, REV_CMP>
{
    using value_type = _Typ;
    static constexpr bool reversed = REV_CMP;

    // constructor
    template<typename... Args> explicit Comparable(Args&&... args)
        : _value{_Typ{std::forward<Args>(args)...}}
    {
    }

    // data access
    const value_type& Get() const REZ_NOEXCEPT { return _value; }
    value_type& Get()  REZ_NOEXCEPT { return _value; }

    // converters
    template<bool _> explicit Comparable(const Comparable<value_type, _>& other)
        : _value{other.Get()}
    {
    }

    // comparision Comparable
    template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return !(Get() < other.Get());
    }

    // comparision value_type
    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return !(Get() < other);
    }

private:
    value_type _value;
};

template<typename T> using ReversedComparable = Comparable<T, REV_CMP>;

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
// operator << overloads
//
template<typename T, bool _Rev> std::ostream& operator<<(std::ostream& os, const Comparable<T, _Rev>& cv)
{
    os << std::boolalpha << "Comparable: [ reversed: " << _Rev << ", value: " << cv.Get() << " ]";
    return os;
}

//
// Comparable Factory, specialize to provide custom constructor
//
template<typename _Typ> struct Factory
{
    using value_type = _Typ;
    template<bool _Rev = DEF_CMP, typename... Args> static Comparable<value_type, _Rev> Create(Args&&... args)
    {
        return Comparable<value_type, _Rev>{std::forward<Args>(args)...};
    }
};

#endif // REZ_COMPARABLE_HPP
