#ifndef TOKEN_HPP
#define TOKEN_HPP

#include "exceptions.hpp"
#include "value.hpp"

#include <regex>
#include <vector>

#include <fmt/format.h>

namespace rez
{

template<typename T> using Token = Value<T>;

//
// NumericToken impl
//
class NumericToken : public Value<rez_int>,
                     public Comparable<NumericToken, rez_int>,
                     public Comparable<NumericToken, NumericToken>
{
public:
    using value_type = rez_int;

    explicit NumericToken(string_view token)
        : Value{to_int(token)}
    {
    }

    explicit operator std::string() const { return std::to_string(Get()); }
    friend std::ostream& operator<<(std::ostream& os, const NumericToken& v) { return (os << v.Get()); }
};

//struct SubTokenHelper
//{
//    string_view s;
//    rez_int  n;
//};

//
// weak reference does not throw
//
class SubToken
{
public:
    explicit SubToken(string_view v) REZ_NOEXCEPT
    {
        s = v;
        n = is_digit(v) ? to_int(v) : REZ_INT_INVALID;
    }

    bool operator<(const SubToken& other) const REZ_NOEXCEPT
    {
        if (n == REZ_INT_INVALID)
        {
            return other.n == REZ_INT_INVALID ? s < other.s : true;
        }

        return other.n == REZ_INT_INVALID ? false : std::tie(n, s) < std::tie(other.n, other.s);
    }

    bool operator==(const SubToken& other) const REZ_NOEXCEPT { return (s == other.s) && (n == other.n); }

    string_view s;
    rez_int n;
};

inline std::ostream& operator<<(std::ostream& os, const SubToken& other)
{
    return (other.n == REZ_INT_INVALID ? os << other.s : os << other.n);
}

//
// AlphanumericToken impl
//
class AlphanumericToken : public ValueArray<SubToken>,
                          public Comparable<AlphanumericToken, std::vector<SubToken>>,
                          public Comparable<AlphanumericToken, AlphanumericToken>
{
public:
    explicit AlphanumericToken(string_view token)
        : AlphanumericToken(token, true)
    {
    }

    AlphanumericToken(string_view token, bool deep_copy)
        : ValueArray<SubToken>({})
    {
        if (token.empty() && !is_alphanumeric(token))
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

        // split by numbers
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

    explicit operator std::string() const
    {
        std::string str;
        for(const auto& sub_token : Get() )
        {
            str += sub_token.s.to_string();
            // TODO fix copying std string
            //fmt::format_to(buff, sub_token.s.to_string());
        }
        return str;
    }

    friend std::ostream& operator<<(std::ostream& os, const AlphanumericToken& v)
    {
        for (const auto& sub_token : v.Get())
        {
            os << sub_token << ", ";
        }
        return os;
    }

private:
    std::string cached_;
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
