#ifndef REZ_VERSION_HPP
#define REZ_VERSION_HPP

#include "NewToken.hpp"

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
template<bool _Rev = false>
VersionT<NumericToken, _Rev> create_numeric_version(string_view version)
{
    typename VersionT<NumericToken, _Rev>::value_type tokens;
    tokens.emplace_back(to_int(version));
    return VersionT<NumericToken, _Rev>{std::move(tokens)};
}

//
// Alphanumeric Version construction
//
AlphanumericVersion create_alphanumeric_version(string_view)
{
    AlphanumericVersion::value_type tokens;
    return AlphanumericVersion{std::move(tokens)};
}

#endif // REZ_VERSION_HPP
