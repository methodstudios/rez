#ifndef REZ_VERSION_HPP
#define REZ_VERSION_HPP

#include "Token.hpp"

//
// Version aliases
//
template<typename T, bool _Rev>
using VersionT = ComparableValue<std::vector<T>, _Rev>;

using NumericVersion = VersionT<NumericToken, NumericToken::reversed>;
using AlphanumericVersion = VersionT<AlphanumericToken, AlphanumericToken::reversed>;

//
// Numeric Version construction
//
template<bool _Rev = false> VersionT<NumericToken, _Rev> create_numeric_version(string_view version)
{
    typename VersionT<NumericToken, _Rev>::value_type tokens;
    return VersionT<NumericToken, _Rev>{std::move(tokens)};
}

//
// Alphanumeric Version construction
//
template<bool _Rev = false> VersionT<AlphanumericToken, _Rev> create_alphanumeric_version(string_view)
{
    typename VersionT<AlphanumericToken, _Rev>::value_type tokens;
    return VersionT<AlphanumericToken, _Rev>{std::move(tokens)};
}

#endif // REZ_VERSION_HPP
