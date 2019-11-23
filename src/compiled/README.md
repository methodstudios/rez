# WIP: compiled `version`

## Comparable type

To avoid dynamic dispatching I turned inheritance into a composition. Pythonic polymorphic base `_Comparable` was replaced with class template: `Comparable<T, bool>`. 
Through partial specialization of `bool` parameter `_ReversedComparable` is being implemented:

```none
Comparable<int, false> := _Comparable 
Comparable<int, true>  := _ReversedComparable
```

First template argument `T` is used to define underlying type. Second(`bool`) is used to define behaviour of the compare operators: 

```c++
auto a = Comparable<int, false>{10}; // comparison:  < 
auto b = Comparable<int, true>{11};  // comparison: !<

auto ra = a < b; // true - Comparable
auto rb = b < a; // true - _ReversedComparable
```

`Comparable<int, false>` can be easily converted to `Comparable<int, true>` and vice versa, but that requires underlying data to be copied.
`Comparable<T, bool>` carries a set of `explicit` constructors that allow conversion from `true`/`false` specializations.

Function `reverse_sort_key` provides functionality to flip `bool` template argument and return a copy of the object with reversed comparision flag:

```c++
template<typename T, bool R> Comparable<T, !R> reverse_sort_key(const Comparable<T, R>& other)
{
    return Comparable<T,!R>{other};
}
```

## Type aliases

There are bunch of alias helpers to define common types:

```c++
using NumericValue = rez_int;
template<bool _Rev> using NumericTokenT = Comparable<NumericValue, _Rev>;

using NumericToken = NumericTokenT<false>;
using ReversedNumericToken = NumericTokenT<true>;
```

## Initialization

**TODO: introduce constructors.**
 
Object construction does not happen through constructor, but through a function call:

```c++
template<bool _Rev = false> NumericTokenT<_Rev> create_numeric_token(string_view token)
{
    return NumericTokenT<_Rev>{to_int(token)};
}
```

## Comparisons

`bool` template argument is only used to match the specialization, and to call proper overloaded comparison operator, 
but during comparison itself, aforementioned `bool` value is discarded.

Partial specialization for `Comparable<T, false>`:

```c++
template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
{
    return (Get() < other.Get());
}
```

Partial specialization for `Comparable<T, true>`:

```c++
template<bool _> bool operator<(const Comparable<value_type, _>& other ) const REZ_NOEXCEPT
{
    return !(Get() < other.Get());
}
```

## To string conversion

Overloading `to_string` is a way to do it. Most common types have overloads included:

```c++
template<bool _Rev> std::string to_string(const NumericTokenT<_Rev>& other)
{
    return std::to_string(other.Get());
}
```

## Bugs

Currently `nonstd::string_view` used in `SubType<T>` is memory leaking and does not keep source object alive. 