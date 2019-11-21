#ifndef VALUE_HPP
#define VALUE_HPP

#include <cassert>
#include <limits>
#include <string>
#include <vector>

#include <nonstd/string_view.hpp>

#define REZ_NOEXCEPT noexcept

namespace rez
{

using rez_int = int64_t;
using string_view = nonstd::string_view;

inline rez_int operator"" _ri(unsigned long long int v) REZ_NOEXCEPT { return static_cast<rez_int>(v); }
inline std::string operator"" _rs(const char* v) REZ_NOEXCEPT { return {v}; }

static constexpr rez_int REZ_INT_INVALID = static_cast<rez_int>(-1);

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

//
// Value interface
//
template <typename _Type> struct Value
{
    using value_type = _Type;

    const value_type& Get() const REZ_NOEXCEPT { return value_; }
    value_type& Get() REZ_NOEXCEPT { return value_; }


//protected:
    explicit Value(value_type v)
        : value_{std::move(v)}
    {
    }
    ~Value() = default;

    value_type value_;
};

//
// Value comparators used to be passed as functors
//
template<typename _Lt, typename _Rt>
struct less
{
    bool operator()(const Value<_Lt>& lhs, const Value<_Rt>& rhs) const REZ_NOEXCEPT
    {
        return lhs.Get() < rhs.Get();
    }
};

template<typename _Lt, typename _Rt>
struct greater
{
    bool operator()(const Value<_Lt>& lhs, const Value<_Rt>& rhs) const REZ_NOEXCEPT
    {
        return lhs.Get() > rhs.Get();
    }
};

template<typename _Lt, typename _Rt>
struct equal_to
{
    bool operator()(const Value<_Lt>& lhs, const Value<_Rt>& rhs) const REZ_NOEXCEPT
    {
        return lhs.Get() == rhs.Get();
    }
};

// global compare operators

//template <typename _Lt, typename _Rt> bool operator<(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//    return less<_Lt, _Rt>{}(lhs, rhs);
//}
//
//template <typename _Lt, typename _Rt> bool operator>(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//    return greater<_Lt, _Rt>{}(lhs, rhs);
//}
//
//template <typename _Lt, typename _Rt> bool operator==(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//    return equal_to<_Lt, _Rt>{}(lhs, rhs);
//}

template <typename _Lt, typename _Rt> struct Comparable
{
    friend bool operator<(const _Lt& lhs, const _Rt& rhs) REZ_NOEXCEPT { return less<typename _Lt::value_type, typename _Rt::value_type>{}(lhs, rhs); }
    friend bool operator>(const _Lt& lhs, const _Rt& rhs) REZ_NOEXCEPT { return greater<typename _Lt::value_type, typename _Rt::value_type>{}(lhs, rhs); }
    friend bool operator==(const _Lt& lhs, const _Rt& rhs) REZ_NOEXCEPT { return equal_to<typename _Lt::value_type, typename _Rt::value_type>{}(lhs, rhs); }

protected:
    Comparable() = default;

private:
    _Lt& ThisT() { return static_cast<_Lt&>(*this); }
    const _Lt& ThisT() const { return static_cast<const _Lt&>(*this); }
};

template <typename _Lt, typename _Rt> struct ReverseComparable
{
    friend bool operator<(const _Lt& lhs, const _Rt& rhs) REZ_NOEXCEPT { return greater<typename _Lt::value_type, typename _Rt::value_type>{}(lhs, rhs); }
    friend bool operator>(const _Lt& lhs, const _Rt& rhs) REZ_NOEXCEPT { return less<typename _Lt::value_type, typename _Rt::value_type>{}(lhs, rhs); }
    friend bool operator==(const _Lt& lhs, const _Rt& rhs) REZ_NOEXCEPT { return equal_to<typename _Lt::value_type, typename _Rt::value_type>{}(lhs, rhs); }

protected:
    ReverseComparable() = default;

private:
    _Lt& ThisT() { return static_cast<_Lt&>(*this); }
    const _Lt& ThisT() const { return static_cast<const _Lt&>(*this); }
};

//template<typename T>
//struct SimComparable
//{
//    SimComparable(Value<T>&& value)
//        : v(std::move(value))
//    {
//    }
//
//    ~SimComparable() = default;
//
//    bool operator<(const T& r) const { return v.Get() < r; }
//    bool operator<(const SimComparable<T>& r) const { return v.Get() < r.v.Get(); }
//    bool operator<(const Value<T>& r) const { return v.Get() < r.Get(); }
//
//    Value<T> v;
//};
//
//
//template<typename T>
//struct _ReversedComparable
//{
//    bool operator<(const T& r) const { return !(v.Get() < r); }
//    bool operator<(const _ReversedComparable<T>& r) const { return !(v.Get() < r.v.Get()); }
//    bool operator<(const Value<T>& r) const { return !(v.Get() < r.Get()); }
//
//    Value<T> v;
//};



//template <typename _Lt, typename _Rt> bool operator<=(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//
//}

//template <typename _Lt, typename _Rt> bool less_equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//    return lhs.Get() <= rhs.Get();
//}
//
//template <typename _Lt, typename _Rt> bool greater_equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//    return lhs.Get() >= rhs.Get();
//}
//
//template <typename _Lt, typename _Rt> bool not_equal_to(const Value<_Lt>& lhs, const Value<_Rt>& rhs) REZ_NOEXCEPT
//{
//    return lhs.Get() != rhs.Get();
//}

//
// Value Array interface
//
template<typename _Type>
class ValueArray : public Value<std::vector<_Type>>
{
public:
    using value_type = std::vector<_Type>;
    using Value<std::vector<value_type>>::Get;

    ValueArray()
        : Value<std::vector<value_type>>({})
    {
    }

    value_type& operator[](size_t index) REZ_NOEXCEPT {return Get()[index];}
    const value_type& operator[](size_t index) const REZ_NOEXCEPT { return Get()[index]; }

    size_t Size() const REZ_NOEXCEPT { return Get().size(); }
    void Append(value_type&& v) REZ_NOEXCEPT {Get().push_back(std::move(v));}
};



} // namespace rez

#endif // VALUE_HPP
