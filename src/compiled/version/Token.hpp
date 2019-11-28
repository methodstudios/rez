#ifndef REZ_TOKEN_HPP
#define REZ_TOKEN_HPP

#include "Types.hpp"

#include <regex>

//
//
//
class SubToken
{
public:
    using value_type = string_view;

    explicit SubToken(const value_type& v) REZ_NOEXCEPT
    {
        s = v;
        n = is_digit(s) ? to_int(s) : REZ_INT_INVALID;
    }

    explicit SubToken(value_type&& v) REZ_NOEXCEPT
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

    bool operator<(rez_int other) const REZ_NOEXCEPT { return other == REZ_INT_INVALID ? false : n < other; }

    bool operator==(const SubToken& other) const REZ_NOEXCEPT { return (s == other.s) && (n == other.n); }
    bool operator==(rez_int other) const REZ_NOEXCEPT { return other == REZ_INT_INVALID ? false : n == other; }

    value_type s;
    rez_int n;
};

//
// Token Aliases
//

using NumericToken = rez_int;
using AlphanumericToken = std::vector<SubToken>;

//
// To string conversion
//

inline std::string to_string(const NumericToken& other) { return std::to_string(other); }

inline std::string to_string(const AlphanumericToken& other)
{
    std::string str;
    for (const auto& sub_token : other)
    {
        str.append(sub_token.s.cbegin(), sub_token.s.cend());
    }
    return str;
}

inline std::ostream& operator<<(std::ostream& os, const AlphanumericToken& other) { return (os << to_string(other)); }

//
// Token Factory
//

template <> struct Factory<NumericToken>
{
    static NumericToken Create(string_view token)
    {
        if (token.empty())
        {
            throw std::invalid_argument(std::string{"Invalid string to parse: "} + token.to_string());
        }

        return to_int(token);
    }
};

template <> struct Factory<AlphanumericToken>
{
    static AlphanumericToken Create(string_view token)
    {
        if (token.empty() || !is_alphanumeric(token))
        {
            std::string message;
            message.append("Invalid string to parse ");
            message.append(token.data(), token.size());
            throw std::runtime_error(message);
        }

        // TODO: make more generic it work with string. string_view
        SubToken::value_type cached_token = token;

        // split by numbers
        static const std::regex n_regex{R"([0-9]+)", std::regex::optimize};

        std::regex_iterator<SubToken::value_type::const_iterator> it{cached_token.cbegin(), cached_token.cend(), n_regex};
        std::regex_iterator<SubToken::value_type::const_iterator> last{};

        AlphanumericToken tokens;

        // no numbers
        if (it == last)
        {
            tokens.emplace_back(std::move(cached_token));
            return {tokens};
        }

        // iterate number splits
        for (; it != last; ++it)
        {
            // convert to string value: 12_hello_world_34 -> [12(int), "_hello_world_"(str), 34(int)]
            if (it->prefix().length())
            {
                // TODO: make more generic it work with string. string_view
                const auto& match = it->prefix();
                tokens.emplace_back(SubToken::value_type{match.first, static_cast<size_t>(match.length())});
            }

            // TODO: make more generic it work with string. string_view
            const auto& match = (*it)[0];
            tokens.emplace_back(SubToken::value_type{match.first, static_cast<size_t>(match.length())});
        }

        return tokens;
    }
};

//
// User defined literals
//

inline NumericToken operator"" _nt(const char* v, size_t s) { return Factory<NumericToken>::Create(string_view{v, s}); }
inline AlphanumericToken operator"" _ant(const char* v, size_t s)
{
    return Factory<AlphanumericToken>::Create(string_view{v, s});
}

namespace rez_impl
{

inline SubToken operator"" _st(const char* v, size_t s) { return SubToken{SubToken::value_type{v, s}}; }

} // namespace rez_impl

#endif // REZ_TOKEN_HPP
