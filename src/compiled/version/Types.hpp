#ifndef REZ_TYPES_HPP
#define REZ_TYPES_HPP

#include <memory>
#include <type_traits>

#include <nonstd/string_view.hpp>

#define REZ_NOEXCEPT noexcept

using rez_int = int64_t;
using string_view = nonstd::string_view;

inline rez_int operator"" _ri(unsigned long long int v) REZ_NOEXCEPT { return static_cast<rez_int>(v); }
inline string_view operator"" _rs(const char* v, size_t s) REZ_NOEXCEPT { return {v, s}; }

constexpr auto REZ_INT_INVALID = static_cast<rez_int>(-1);
constexpr auto INDEX_INVALID = static_cast<size_t>(-1);

// rez specific functions
inline bool is_alpha(const char c) REZ_NOEXCEPT{ return std::isalpha(c) || c == '_';}
inline bool is_numeric(const char c) REZ_NOEXCEPT { return std::isdigit(c); }
inline bool is_alphanumeric(const char c) REZ_NOEXCEPT { return is_numeric(c) || is_alpha(c); }
inline bool is_version_separator(const char c) REZ_NOEXCEPT { return c == '.' || c == '-'; }

inline size_t is_numeric(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!std::isdigit(c)) return false;
    }
    return true;
}

inline size_t is_alphanumeric(string_view str) REZ_NOEXCEPT
{
    for (const auto& c : str)
    {
        if (!is_alphanumeric(c)) return false;
    }
    return true;
}

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
// Creation Policies for the Factory
//
template <typename _Typ> struct StackCreator
{
    using value_type = _Typ;
    using return_type = value_type;

    template <typename... Args> static return_type New(Args&&... args) { return value_type{std::forward<Args>(args)...}; }

    static value_type& Get(return_type& value) { return value; }
};

template <typename _Typ> struct NewCreator
{
    using value_type = _Typ;
    using return_type = value_type*;

    template <typename... Args> static return_type New(Args&&... args)
    {
        return new value_type{std::forward<Args>(args)...};
    }

    static value_type& Get(return_type value) { return *value; }
};

template <typename _Typ> struct UniquePtrCreator
{
    using value_type = _Typ;
    using return_type = std::unique_ptr<value_type>;

    template <typename... Args> static return_type New(Args&&... args)
    {
        return return_type{new value_type{std::forward<Args>(args)...}};
    }

    static value_type& Get(return_type& value) { return *value; }
};

//
//
//
template <typename _Typ, template <typename> class CreationPolicy> struct PolicyTypeFactory : CreationPolicy<_Typ>
{
    using value_type = _Typ;
    using return_type = typename CreationPolicy<_Typ>::return_type;

    template <typename... Args> static return_type Create(Args&&... args)
    {
        return CreationPolicy<_Typ>::New(std::forward<Args>(args)...);
    }
};

template <typename _Typ, template <typename> class CreationPolicy = StackCreator>
using Factory = PolicyTypeFactory<_Typ, CreationPolicy>;

#endif // REZ_TYPES_HPP
