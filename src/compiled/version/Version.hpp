#ifndef REZ_VERSION_HPP
#define REZ_VERSION_HPP

#include "Token.hpp"

template<typename T> struct VersionData
{
    explicit VersionData(string_view str)
    {
        if(!str.empty())
        {
            static const std::regex re_token{R"([a-zA-Z0-9_]+)"};

            std::regex_iterator<string_view::const_iterator> it{str.cbegin(), str.cend(), re_token};
            std::regex_iterator<string_view::const_iterator> last{};

            //
            if(it == last)
            {
                throw std::runtime_error("Invalid version");
            }

            // iterate over numeric/alphanumeric tokens
            for (; it != last; ++it)
            {
                //
                if (it->prefix().length())
                {
                    const std::cmatch::value_type& match = it->prefix();
                    seps.emplace_back(string_view{match.first, static_cast<size_t>(match.length())});
                }

                // emplace T
                const std::cmatch::value_type& match = (*it)[0];
                //tokens.emplace_back(string_view{match.first, static_cast<size_t>(match.length())});
            }
        }
    }

    // version details
    string_view Major() const REZ_NOEXCEPT { assert(Size() > 0); return tokens[0]; }
    string_view Minor() const REZ_NOEXCEPT { assert(Size() > 1); return tokens[1]; }
    string_view Patch() const REZ_NOEXCEPT { assert(Size() > 2); return tokens[2]; }

    // data accessors
    size_t Size() const REZ_NOEXCEPT{ return tokens.size(); }
    T& operator[](size_t index) REZ_NOEXCEPT { assert(index < Size()); return tokens[index]; }
    const T& operator[](size_t index) const REZ_NOEXCEPT { assert(index < Size()); return tokens[index];}

    explicit operator bool() const REZ_NOEXCEPT { return Size() > 0; }
    explicit operator std::string() const
    {
        return _str;
    }

    size_t Hash() const REZ_NOEXCEPT
    {
        if(_hash == REZ_INDEX_INVALID)
        {
            static std::hash<std::string> hasher;
        }

        return _hash;
    }

private:
    std::vector<T> tokens;
    std::vector<string_view> seps;

    mutable std::string _str;
    mutable size_t _hash{REZ_INDEX_INVALID};
};

//
// Version aliases
//
template<typename T, bool _Rev> using VersionT = Comparable<VersionData<T>, _Rev>;

using NumericVersion = VersionT<NumericToken, NumericToken::reversed>;
using AlphanumericVersion = VersionT<AlphanumericToken, AlphanumericToken::reversed>;

//
// Numeric Version construction
//
template<bool _Rev = DEF_CMP> VersionT<NumericToken, _Rev> create_numeric_version(string_view version)
{
    typename VersionT<NumericToken, _Rev>::value_type tokens;
    return VersionT<NumericToken, _Rev>{std::move(tokens)};
}

//
// Alphanumeric Version construction
//
template<bool _Rev = DEF_CMP> VersionT<AlphanumericToken, _Rev> create_alphanumeric_version(string_view)
{
    typename VersionT<AlphanumericToken, _Rev>::value_type tokens;
    return VersionT<AlphanumericToken, _Rev>{std::move(tokens)};
}

#endif // REZ_VERSION_HPP
