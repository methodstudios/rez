#ifndef REZ_COMPARABLE_VALUE_HPP
#define REZ_COMPARABLE_VALUE_HPP

#include "Types.hpp"

//
// Helper class to provide custom constructor to Comparable
//
template<typename T> struct Data
{
    using value_type = T;

    Data(T&& data)
        : _data{std::move(data)}
    {
    }

    bool operator<(const Data<value_type>& other) const REZ_NOEXCEPT
    {
        return _data < other._data;
    }

    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return _data < other;
    }

    T _data;
};

//
// Comparable reversible arbitrary value
//
// Explicit constructor constructor allow conversion from Comparable<T, true> to Comparable<T, false>,
// Conversion from normal and reverse comparable is not free, it requires copy.
//
template <typename _Typ, bool _Rev = false> struct Comparable
{
    using value_type = _Typ;
    static const bool reversed = _Rev;

    // constructor
    template<typename... Args> explicit Comparable(Args&&... args)
        : value_{std::forward<Args>(args)...}
    {
    }

    const value_type& Get() const REZ_NOEXCEPT { return value_; }
    value_type& Get()  REZ_NOEXCEPT { return value_; }

    // converters
    template<bool _> explicit Comparable(const Comparable<value_type, _>& other)
        : value_{other.Get()}
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
    value_type value_;
};

//
// Partial specialization for ReversedComparable representation
//
template <typename _Typ> struct Comparable<_Typ, true>
{
    using value_type = _Typ;
    static const bool reversed = true;

    // constructors
    explicit Comparable(const value_type& v)
        : value_{v}
    {
    }

    explicit Comparable(value_type&& v)
        : value_{std::move(v)}
    {
    }

    const value_type& Get() const REZ_NOEXCEPT  { return value_; }
    value_type& Get()  REZ_NOEXCEPT { return value_; }

    // Allows explicitly convert from Normal/Reversible type
    template<bool _> explicit Comparable(const Comparable<value_type, _>& other)
        : value_{other.Get()}
    {
    }

    // comparision, Reversible type is discarded, that allows compare Normal and Reversed types
    template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return !(Get() < other.Get());
    }

    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return !(Get() < other);
    }

private:
    value_type value_;
};

template<typename T> using ReversedComparable = Comparable<T, true>;

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

#endif // REZ_COMPARABLE_VALUE_HPP
