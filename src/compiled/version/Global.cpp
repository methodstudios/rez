#include "Version.hpp"
#include "Bound.hpp"

//
// Inf Numeric
//
template<> const NumericVersion& NumericVersion::Inf() REZ_NOEXCEPT
{
    static NumericVersion version{{NumericVersion::value_type{REZ_INT_INFINITY}}};
    return version;
}

//
// Inf Alphanumeric
//
template<> const AlphanumericVersion& AlphanumericVersion::Inf() REZ_NOEXCEPT
{
    static AlphanumericVersion version{ {AlphanumericVersion::value_type{SubToken{REZ_INT_INFINITY}}}};
    return version;
}

//
// Min Lower Bound
//
template<> const LowerBound<NumericVersion>& LowerBound<NumericVersion>::Min() REZ_NOEXCEPT
{
    static LowerBound<NumericVersion> lower_bound{Factory<NumericVersion>::Create(""), true};
    return lower_bound;
}
template<> const LowerBound<AlphanumericVersion>& LowerBound<AlphanumericVersion>::Min() REZ_NOEXCEPT
{
    static LowerBound<AlphanumericVersion> lower_bound{Factory<AlphanumericVersion>::Create(""), true};
    return lower_bound;
}

//
// Inf Upper Bound
//
template<> const UpperBound<NumericVersion>& UpperBound<NumericVersion>::Inf() REZ_NOEXCEPT
{
    static UpperBound<NumericVersion> upper_bound{NumericVersion{NumericVersion::Inf()}, true};
    return upper_bound;
}
template<> const UpperBound<AlphanumericVersion>& UpperBound<AlphanumericVersion>::Inf() REZ_NOEXCEPT
{
    static UpperBound<AlphanumericVersion> upper_bound{AlphanumericVersion{AlphanumericVersion::Inf()}, true};
    return upper_bound;
}

//
// Any Bound - [Min, Inf]
//
template<> const Bound<NumericVersion>& Bound<NumericVersion>::Any() REZ_NOEXCEPT
{
    static Bound<NumericVersion> any_bound{};
    return any_bound;
}
template<> const Bound<AlphanumericVersion>& Bound<AlphanumericVersion>::Any() REZ_NOEXCEPT
{
    static Bound<AlphanumericVersion> any_bound{};
    return any_bound;
}