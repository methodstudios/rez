#ifndef REZ_TYPES_HPP
#define REZ_TYPES_HPP

#include <type_traits>

#include <nonstd/string_view.hpp>

#define REZ_NOEXCEPT noexcept

using rez_int = int64_t;
using string_view = nonstd::string_view;

inline rez_int operator"" _ri(unsigned long long int v) REZ_NOEXCEPT { return static_cast<rez_int>(v); }
inline std::string operator"" _rs(const char* v) REZ_NOEXCEPT { return {v}; }

static constexpr rez_int REZ_INT_INVALID = static_cast<rez_int>(-1);
static constexpr size_t REZ_INDEX_INVALID = static_cast<size_t>(-1);

// size of static array
template <typename T, size_t N> constexpr size_t size(const T (&)[N]) REZ_NOEXCEPT { return N; }

inline size_t is_digit(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

inline bool is_alphanumeric(const char c) REZ_NOEXCEPT { return std::isalnum(c) || c == '_'; }
inline size_t is_alphanumeric(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!is_alphanumeric(c)) return false;
    }
    return true;
}

inline bool is_version_separator(const char c) REZ_NOEXCEPT { return c == '.' || c == '-'; }

inline rez_int to_int(string_view value)
{
    // parse chars to rez_int, sigma(10^(iteration) * digit)
    rez_int result{};
    rez_int digit_mul = 1;
    for (auto it = value.crbegin(); it != value.crend(); ++it, digit_mul *= 10_ri)
    {
        if (!std::isdigit(*it))
        {
            throw std::invalid_argument("Invalid string to parse: " + value.to_string());
        }
        result += (*it - static_cast<rez_int>('0')) * digit_mul;
    }

    return result;
}

//
// Factory to create object, when implementation is not provided then it fails to compile
//
template <typename _Typ> struct Factory
{
    static_assert(true, "Factory not implemented for given type!");
};

#endif // REZ_TYPES_HPP
