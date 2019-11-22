#ifndef REZ_COMPARABLE_VALUE_HPP
#define REZ_COMPARABLE_VALUE_HPP

#include "Types.hpp"

//
// Comparable reversible arbitrary value
//
// Explicit constructor constructor allow conversion from ComparableValue<T, true> to ComparableValue<T, false>,
// Conversion from normal and reverse comparable is not free, it requires copy.
//
template <typename _Type, bool _Rev> struct ComparableValue
{
    using value_type = _Type;
    static const bool reversed = _Rev;

    // Allows explicitly convert from Normal/Reversible type
    template<bool _> explicit ComparableValue(const ComparableValue<value_type, _>& other)
        : value_{other.Get()}
    {
    }

    // comparision, Reversible type is discarded, that allows compare Normal and Reversed types
    template<bool _> bool operator<(const ComparableValue<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return (Get() < other.Get());
    }

    //
    explicit ComparableValue(value_type&& v)
        : value_{std::move(v)}
    {
    }

    const value_type& Get() const REZ_NOEXCEPT  { return value_; }
    value_type& Get()  REZ_NOEXCEPT { return value_; }

    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return (Get() < other);
    }

    bool operator>(const value_type& other) const REZ_NOEXCEPT
    {
        return (Get() > other);
    }

private:
    value_type value_;
};

//
// Partial specialization for reversed comparision
//
template <typename _Type> struct ComparableValue<_Type, true>
{
    using value_type = _Type;
    static const bool reversed = true;

    // Allows explicitly convert from Normal/Reversible type
    template<bool _> explicit ComparableValue(const ComparableValue<value_type, _>& other)
        : value_{other.Get()}
    {
    }

    // comparision, Reversible type is discarded, that allows compare Normal and Reversed types
    template<bool _> bool operator<(const ComparableValue<value_type, _>& other ) const REZ_NOEXCEPT
    {
        return !(Get() < other.Get());
    }

    //
    explicit ComparableValue(value_type&& v)
        : value_{std::move(v)}
    {
    }

    const value_type& Get() const REZ_NOEXCEPT  { return value_; }
    value_type& Get()  REZ_NOEXCEPT { return value_; }

    bool operator<(const value_type& other) const REZ_NOEXCEPT
    {
        return !(Get() < other);
    }

    bool operator>(const value_type& other) const REZ_NOEXCEPT
    {
        return !(Get() > other);
    }

private:
    value_type value_;
};

//
// switches normal comparable <=> reversed comparable, returns a copy! bool template parameter is flipped.
//
template<typename T, bool R> ComparableValue<T, !R> reverse_sort_key(const ComparableValue<T, R>& other)
{
    return ComparableValue<T,!R>{other};
}

template<typename T, bool _Rev>
std::ostream& operator<<(std::ostream& os, const ComparableValue<T, _Rev>& cv)
{
    os << std::boolalpha << "ComparableValue: [ reversed: " << _Rev << ", value: " << cv.Get() << " ]";
    return os;
}

#endif // REZ_COMPARABLE_VALUE_HPP
