#include "Version.hpp"
#include "Bound.hpp"

//
// Numeric Infinity
//
template<> const NumericVersion::value_type& NumericVersion::TokenInf() REZ_NOEXCEPT
{
    static NumericVersion::value_type token{REZ_INT_INFINITY};
    return token;
}
template<> const NumericVersion& NumericVersion::Inf() REZ_NOEXCEPT
{
    static NumericVersion version{{NumericVersion::TokenInf()}};
    return version;
}

//
// Alphanumeric Infinity
//
template<> const AlphanumericVersion::value_type& AlphanumericVersion::TokenInf() REZ_NOEXCEPT
{
    static AlphanumericVersion::value_type token{ SubToken{REZ_INT_INFINITY} };
    return token;
}
template<> const AlphanumericVersion& AlphanumericVersion::Inf() REZ_NOEXCEPT
{
    static AlphanumericVersion version{{AlphanumericVersion::TokenInf()}};
    return version;
}

//
// Min Lower Bound
//
template<> const LowerBound<NumericVersion>& LowerBound<NumericVersion>::Min() REZ_NOEXCEPT
{
    static NumericVersion empty_version = Factory<NumericVersion>::Create("");
    static LowerBound<NumericVersion> lower_bound{empty_version, true};
    return lower_bound;
}
template<> const LowerBound<AlphanumericVersion>& LowerBound<AlphanumericVersion>::Min() REZ_NOEXCEPT
{
    static AlphanumericVersion empty_version = Factory<AlphanumericVersion>::Create("");
    static LowerBound<AlphanumericVersion> lower_bound{empty_version, true};
    return lower_bound;
}