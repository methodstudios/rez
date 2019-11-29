#ifndef REZ_TOKEN_HPP
#define REZ_TOKEN_HPP

#include "Types.hpp"

#include <vector>

//
// Alphanumeric Sub Token
//
class SubToken
{
public:
    using value_type = string_view;

    explicit SubToken(const value_type& v) REZ_NOEXCEPT
    {
        s = v;
        n = is_numeric(s) ? to_int(s) : REZ_INT_INVALID;
    }

    explicit SubToken(value_type&& v) REZ_NOEXCEPT
    {
        s = v;
        n = is_numeric(s) ? to_int(s) : REZ_INT_INVALID;
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

template <template <typename> class CreationPolicy>
struct PolicyTypeFactory<NumericToken, CreationPolicy> : CreationPolicy<NumericToken>
{
    using value_type = typename CreationPolicy<NumericToken>::value_type;
    using return_type = typename CreationPolicy<NumericToken>::return_type;

    using CreationPolicy<NumericToken>::New;

    static NumericToken Create(string_view token)
    {
        if (token.empty())
        {
            throw std::invalid_argument(std::string{"Invalid string to parse: "} + token.to_string());
        }

        return New(to_int(token));
    }
};

template <template <typename> class CreationPolicy>
struct PolicyTypeFactory<AlphanumericToken, CreationPolicy> : CreationPolicy<AlphanumericToken>
{
    using value_type = typename CreationPolicy<AlphanumericToken>::value_type;
    using return_type = typename CreationPolicy<AlphanumericToken>::return_type;

    using CreationPolicy<AlphanumericToken>::New;
    using CreationPolicy<AlphanumericToken>::Get;

    static return_type Create(string_view token)
    {
        if (token.empty() || !is_alphanumeric(token))
        {
            std::string message;
            message.append("Invalid string to parse ");
            message.append(token.data(), token.size());
            throw std::runtime_error(message);
        }

        return_type new_tokens = New();
        value_type& tokens = Get(new_tokens);

        for (auto token_str : SplitAlphanumeric(token))
        {
            tokens.emplace_back(token_str);
        }

        return new_tokens;
    }

private:
    static std::vector<string_view> SplitAlphanumeric(string_view token)
    {
        std::vector<string_view> tokens;

        size_t index{}, n_count{}, a_count{};
        for (; index < token.size(); ++index)
        {
            if (is_numeric(token[index]))
            {
                if (a_count) // pre existing alpha
                {
                    tokens.push_back(token.substr(index - a_count, a_count));
                    a_count = 0;
                }

                ++n_count;
                continue;
            }

            if (is_alpha(token[index]))
            {
                if (n_count) // pre existing numeric
                {
                    tokens.push_back(token.substr(index - n_count, n_count));
                    n_count = 0;
                }

                ++a_count;
                continue;
            }
        }

        // final check, push remaining numeric or alpha
        if (a_count) tokens.push_back(token.substr(index - a_count, a_count));
        if (n_count) tokens.push_back(token.substr(index - n_count, n_count));

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
