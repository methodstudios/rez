#ifndef REZ_VERSION_HPP
#define REZ_VERSION_HPP

#include "Token.hpp"

#include <iostream>

template<typename _Tok> struct VersionData
{
    static_assert(is_comparable<_Tok>::value, "Invalid template parameter, expected Comparable");

    using value_type = _Tok; // comparable token

    size_t Size() const REZ_NOEXCEPT{ return tokens.size(); }

    explicit operator bool() const REZ_NOEXCEPT { return Size() > 0; }

    value_type& operator[](size_t index) REZ_NOEXCEPT { assert(index < Size()); return tokens[index]; }
    const value_type& operator[](size_t index) const REZ_NOEXCEPT { assert(index < Size()); return tokens[index];}

    const value_type& Major() const REZ_NOEXCEPT { assert(Size() > 0); return tokens[0]; }
    const value_type& Minor() const REZ_NOEXCEPT { assert(Size() > 1); return tokens[1]; }
    const value_type& Patch() const REZ_NOEXCEPT { assert(Size() > 2); return tokens[2]; }

    size_t Hash() const REZ_NOEXCEPT
    {
        if(_hash == REZ_INDEX_INVALID)
        {
            static std::hash<std::string> hasher;
        }

        return _hash;
    }

    std::vector<value_type> tokens;
    std::vector<string_view> seps;

    mutable std::string _str;
    mutable size_t _hash{REZ_INDEX_INVALID};
};

//
// Version aliases
//
template<typename _Typ, bool _Rev> using VersionT = Comparable<VersionData<_Typ>, _Rev>;

//
// is_version
//
template<typename _Typ> struct is_version : std::false_type
{
};

template<typename _Typ, bool _Rev> struct is_version<VersionT<_Typ, _Rev>> : std::true_type
{
};

//
// partial specialization for version creation
//
using NumericVersion = VersionT<NumericToken, NORMAL>;


template<typename _Tok, bool _> struct Factory<VersionT<_Tok, _>>
{
    static_assert(is_comparable<_Tok>::value, "Invalid template parameter, expected Comparable!");
    static_assert(is_version<VersionT<_Tok, _>>::value, "Invalid template parameter, expected VersionT!");

    using value_type = _Tok;

    template<bool _Rev = DEFAULT> static VersionT<value_type, _Rev> Create(string_view version)
    {
        VersionT<value_type, _Rev> result;
        if(version.empty())
        {
            return result;
        }

        static const std::regex re_token{R"([a-zA-Z0-9_]+)"};

        std::regex_iterator<string_view::const_iterator> it{version.cbegin(), version.cend(), re_token};
        std::regex_iterator<string_view::const_iterator> last{};

        //
        if(it == last)
        {
            throw std::runtime_error("Invalid version");
        }

        // iterate over numeric/alphanumeric tokens
        for (; it != last; ++it)
        {
            if (it->prefix().length())
            {
                const std::cmatch::value_type& match = it->prefix();
                string_view sep{match.first, static_cast<size_t>(match.length())};
                result->seps.push_back(sep);
            }

            // emplace T
            const std::cmatch::value_type& match = (*it)[0];
            string_view token_str{match.first, static_cast<size_t>(match.length())};
            auto token = Factory<value_type>::Create(token_str);
            result->tokens.push_back(std::move(token));
        }

        return result;
    }
};

using AlphanumericVersion = VersionT<AlphanumericToken, NORMAL>;

#endif // REZ_VERSION_HPP
