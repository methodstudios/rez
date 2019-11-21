#ifndef REZ_TYPES_HPP
#define REZ_TYPES_HPP

#include <nonstd/string_view.hpp>

#define REZ_NOEXCEPT noexcept

using rez_int = int64_t;
using string_view = nonstd::string_view;

inline rez_int operator"" _ri(unsigned long long int v) REZ_NOEXCEPT { return static_cast<rez_int>(v); }
inline std::string operator"" _rs(const char* v) REZ_NOEXCEPT { return {v}; }

static constexpr rez_int REZ_INT_INVALID = static_cast<rez_int>(-1);

// size of static array
template <typename T, size_t N> constexpr size_t size(const T (&)[N]) REZ_NOEXCEPT { return N; }

inline size_t is_digit(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!std::isdigit(c))
            return false;
    }
    return true;
}

inline size_t is_alphanumeric(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!(std::isalnum(c) || c == '_'))
            return false;
    }
    return true;
}

inline rez_int to_int(string_view value)
{
    if (value.empty())
    {
        throw std::invalid_argument(std::string{"Invalid string to parse: "} + value.to_string());
    }

    // parse chars to rez_int, sigma(10^(iteration) * digit)
    rez_int result{};
    rez_int digit_mul = 1;
    for (auto it = value.crbegin(); it != value.crend(); ++it, digit_mul *= 10_ri)
    {
        if (!std::isdigit(*it))
        {
            throw std::invalid_argument(std::string{"Invalid string to parse: "} + value.to_string());
        }
        result += (*it - static_cast<rez_int>('0')) * digit_mul;
    }

    return result;
}

#endif // REZ_TYPES_HPP
