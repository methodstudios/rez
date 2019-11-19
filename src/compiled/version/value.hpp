#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <limits>
#include <string>

#include <nonstd/string_view.hpp>

#define REZ_NOEXCEPT noexcept

namespace rez
{

using r_int = int64_t;
using r_str = std::string;

using string_view = nonstd::string_view;

static constexpr r_int R_INT_INVALID = static_cast<r_int>(-1);

inline r_int operator"" _ri(unsigned long long int v) { return static_cast<r_int>(v); }
inline r_str operator"" _rs(const char* v) { return {v}; }

inline size_t is_digit(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!std::isdigit(c))
            return false;
    }
    return true;
}

inline size_t is_alnum(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!(std::isalnum(c) || c == '_'))
            return false;
    }
    return true;
}

inline r_int string_to_int(string_view value)
{
    if (value.empty())
    {
        throw std::invalid_argument(std::string{"Invalid string to parse: "} + value.to_string());
    }

    // parse chars to r_int, sigma(10^(iteration) * digit)
    r_int result{};
    r_int digit_mul = 1;
    for (auto it = value.crbegin(); it != value.crend(); ++it, digit_mul *= 10_ri)
    {
        if (!std::isdigit(*it))
        {
            throw std::invalid_argument(std::string{"Invalid string to parse: "} + value.to_string());
        }
        result += (*it - static_cast<r_int>('0')) * digit_mul;
    }

    return result;
}

//
// Value interface
//
template <typename T> struct Value
{
    using value_type = T;

    const value_type& Get() const REZ_NOEXCEPT { return value_; }
    value_type& Get() REZ_NOEXCEPT { return value_; }

protected:
    explicit Value(value_type v)
        : value_{std::move(v)}
    {
    }
    ~Value() = default;

    value_type value_;
};


} // namespace rez

#endif // VALUE_HPP
