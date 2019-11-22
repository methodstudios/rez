#ifndef REZ_TOKEN_HPP
#define REZ_TOKEN_HPP

#include "ComparableValue.hpp"

#include <regex>

template<typename T> class SubToken
{
public:
    explicit SubToken(const T& v)
    {
        s = v;
        n = is_digit(s) ? to_int(s) : REZ_INT_INVALID;
    }

    explicit SubToken(T&& v)
    {
        s = std::move(v);
        n = is_digit(s) ? to_int(s) : REZ_INT_INVALID;
    }

    bool operator<(const SubToken& other) const REZ_NOEXCEPT
    {
        if (n == REZ_INT_INVALID)
        {
            return other.n == REZ_INT_INVALID ? s < other.s : true;
        }

        return other.n == REZ_INT_INVALID ? false : std::tie(n, s) < std::tie(other.n, other.s);
    }

    bool operator==(const SubToken& other) const REZ_NOEXCEPT
    {
        return (s == other.s) && (n == other.n);
    }

    T s;
    rez_int n;
};

//
// Token aliases
//

using NumericValue = rez_int;

template<bool _Rev>
using NumericTokenT = ComparableValue<NumericValue, _Rev>;
using NumericToken = NumericTokenT<false>;
using ReversedNumericToken = NumericTokenT<true>;

using AlphanumericSubToken = SubToken<string_view>;
using AlphanumericValue = std::vector<AlphanumericSubToken>;

template<bool _Rev>
using AlphanumericTokenT = ComparableValue<AlphanumericValue, _Rev>;
using AlphanumericToken = AlphanumericTokenT<false>;
using ReversedAlphanumericToken = AlphanumericTokenT<true>;

//
// Token construction
//

template<bool _Rev = false> NumericTokenT<_Rev> create_numeric_token(string_view token)
{
    return NumericTokenT<_Rev>{to_int(token)};
}

template<bool _Rev = false> AlphanumericTokenT<_Rev> create_alphanumeric_token(string_view token)
{
    if (token.empty() && !is_alphanumeric(token))
    {
        std::string message;
        message.append("Invalid string to parse ");
        message.append(token.data(), token.size());
        throw std::runtime_error(std::move(message));
    }

    string_view cached_token = token;

    // split by numbers
    static const std::regex n_regex{R"([0-9]+)", std::regex::optimize};

    std::regex_iterator<string_view::const_iterator> it{cached_token.cbegin(), cached_token.cend(), n_regex};
    std::regex_iterator<string_view::const_iterator> last{};

    typename AlphanumericTokenT<_Rev>::value_type tokens;

    // no numbers
    if (it == last)
    {
        tokens.emplace_back(cached_token);
        return AlphanumericTokenT<_Rev>{std::move(tokens)};
    }

    // iterate number splits
    for (; it != last; ++it)
    {
        // convert to string value: 12_hello_world_34 -> [12(int), "_hello_world_"(str), 34(int)]
        if (it->prefix().length())
        {
            const std::cmatch::value_type& match = it->prefix();
            tokens.emplace_back(string_view{match.first, static_cast<size_t>(match.length())});
        }

        // convert to int value
        const std::cmatch::value_type& match = (*it)[0];
        tokens.emplace_back(string_view{match.first, static_cast<size_t>(match.length())});
    }

    return AlphanumericTokenT<_Rev>{std::move(tokens)};
}

//
// To string conversion
//

template<bool _Rev> std::string to_string(const NumericTokenT<_Rev>& other)
{
    return std::to_string(other.Get());
}

template<bool _Rev> std::string to_string(const AlphanumericTokenT<_Rev>& other)
{
    std::string str;
    for(const auto& sub_token : other.Get() )
    {
        str += sub_token.s.to_string();
    }
    return str;
}

//
// User defined literals
//

inline NumericToken operator"" _nt(const char* v, size_t s)
{
    return create_numeric_token(string_view{v, s});
}

inline AlphanumericSubToken operator "" _st(const char* v, size_t s)
{
    return AlphanumericSubToken{string_view{v, s}};
}

inline AlphanumericToken operator"" _at(const char* v, size_t s)
{
    return create_alphanumeric_token(string_view{v, s});
}

#endif // REZ_TOKEN_HPP
