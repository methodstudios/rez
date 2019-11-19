#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "exceptions.hpp"
#include "value.hpp"

#include <regex>
#include <vector>

namespace rez
{

//
// Value interface comparators
//
// TODO: instead of heaving 5 variations, we could have >=, <=, == only.
//
template <typename _Lt, typename _Rt> bool less(const Value<_Lt>& lhs, const Value<_Rt>& rhs)
{
    return lhs.Get() < rhs.Get();
}

template <typename _Lt, typename _Rt> bool greater(const Value<_Lt>& lhs, const Value<_Rt>& rhs)
{
    return lhs.Get() > rhs.Get();
}

template <typename _Lt, typename _Rt> bool less_equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs)
{
    return lhs.Get() <= rhs.Get();
}

template <typename _Lt, typename _Rt> bool greater_equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs)
{
    return lhs.Get() >= rhs.Get();
}

template <typename _Lt, typename _Rt> bool equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs)
{
    return lhs.Get() == rhs.Get();
}

template <typename _Lt, typename _Rt> bool not_equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs)
{
    return lhs.Get() != rhs.Get();
}

//
// Comparable
// https://en.wikipedia.org/wiki/Barton%E2%80%93Nackman_trick
template <typename _Lt, typename _Rt> struct Comparable
{
    friend bool operator<(const _Lt& lhs, const _Rt& rhs) { return less(lhs, rhs); }
    friend bool operator>(const _Lt& lhs, const _Rt& rhs) { return greater(lhs, rhs); }

    friend bool operator<=(const _Lt& lhs, const _Rt& rhs) { return less_equal_to(lhs, rhs); }
    friend bool operator>=(const _Lt& lhs, const _Rt& rhs) { return greater_equal_to(lhs, rhs); }

    friend bool operator==(const _Lt& lhs, const _Rt& rhs) { return equal_to(lhs, rhs); }
    friend bool operator!=(const _Lt& lhs, const _Rt& rhs) { return not_equal_to(lhs, rhs); }

protected:
    Comparable() = default;

private:
    _Lt& ThisT() { return static_cast<_Lt&>(*this); }
    const _Lt& ThisT() const { return static_cast<const _Lt&>(*this); }
};

//
// NumericToken impl
//
class NumericToken : public Value<r_int>,
                     public Comparable<NumericToken, r_int>,
                     public Comparable<NumericToken, NumericToken>
{
public:
    using value_type = r_int;

    explicit NumericToken(string_view token)
        : Value{string_to_int(token)}
    {
    }

    explicit operator r_str() const { return std::to_string(Get()); }
    friend std::ostream& operator<<(std::ostream& os, const NumericToken& v) { return (os << v.Get()); }
};

//
// weak reference does not throw
//
class SubToken
{
public:
    explicit SubToken(string_view v) REZ_NOEXCEPT
    {
        s = v;
        n = is_digit(v) ? string_to_int(v) : R_INT_INVALID;
    }

    bool operator<(const SubToken& other) const REZ_NOEXCEPT
    {
        if (n == R_INT_INVALID)
        {
            return other.n == R_INT_INVALID ? s < other.s : true;
        }

        return other.n == R_INT_INVALID ? false : std::tie(n, s) < std::tie(other.n, other.s);
    }

    bool operator==(const SubToken& other) const REZ_NOEXCEPT { return (s == other.s) && (n == other.n); }

    string_view s;
    r_int n;
};

inline std::ostream& operator<<(std::ostream& os, const SubToken& other)
{
    return (other.n == R_INT_INVALID ? os << other.s : os << other.n);
}

//
// AlphanumericToken impl
//
class AlphanumericToken : public Value<std::vector<SubToken>>,
                          public Comparable<AlphanumericToken, std::vector<SubToken>>,
                          public Comparable<AlphanumericToken, AlphanumericToken>
{
public:
    explicit AlphanumericToken(string_view token)
        : AlphanumericToken(token, true)
    {
    }

    AlphanumericToken(string_view token, bool deep_copy)
        : Value<std::vector<SubToken>>({})
    {
        if (token.empty() && !is_alnum(token))
        {
            // TODO:: prealocate string
            std::string message{"Invalid string to parse "};
            throw std::runtime_error(message);
        }

        string_view cached_token = token;
        if (deep_copy)
        {
            // cache input string for sub references
            cached_ = token.to_string();
            cached_token = {cached_.c_str(), cached_.size()};
        }

        // split by numers
        static const std::regex n_regex{R"([0-9]+)", std::regex::optimize};

        std::regex_iterator<string_view::const_iterator> it{cached_token.cbegin(), cached_token.cend(), n_regex};
        std::regex_iterator<string_view::const_iterator> last{};

        // no numbers
        if(it == last)
        {
            Get().emplace_back(cached_token);
            return;
        }

        // iterate number splits
        for (; it != last; ++it)
        {
            // convert to string value: 12_hello_world_34 -> [12(int), "_hello_world_"(str), 34(int)]
            if (it->prefix().length())
            {
                const std::cmatch::value_type& match = it->prefix();
                Get().emplace_back(string_view{match.first, static_cast<size_t>(match.length())});
            }

            // convert to int value
            const std::cmatch::value_type& match = (*it)[0];
            Get().emplace_back(string_view{match.first, static_cast<size_t>(match.length())});
        }
    }

    explicit operator r_str() const { return ""; }

    friend std::ostream& operator<<(std::ostream& os, const AlphanumericToken& v)
    {
        for (const auto& sub_token : v.Get())
        {
            os << sub_token << ", ";
        }
        return os;
    }

private:
    r_str cached_;
};

//
// User defined literals
//

inline NumericToken operator"" _nt(const char* v, size_t s)
{
    return NumericToken{string_view{v, s}};
}

inline SubToken operator "" _st(const char* v, size_t s)
{
    return SubToken{string_view{v, s}};
}

inline AlphanumericToken operator"" _at(const char* v, size_t s)
{
    return AlphanumericToken{string_view{v, s}};
}

} // namespace rez

#endif // TOKEN_HPP
