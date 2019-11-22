# WIP: compiled `version`

## Common type

To avoid dynamic dispatching I decided to go with one common type `ComparableValue<T, bool>`. 
With partial specialization of `ComparableValue<T, true>`. `bool` flag reverses comparison behaviour.

First template argument `T` is used to define underlying type.
Second template argument `bool` is used to control behaviour of comparisons operators. For instance:

```c++
auto a = ComparableValue<int, false>{10}; // comparison:  < 
auto b = ComparableValue<int, true>{11};  // comparison: !<

auto ra = a < b; // true - normal comparison
auto rb = b < a; // true - reversed comparison
```

`ComparableValue<int, false>` can be easily converted to `ComparableValue<int, true>` and vice versa, but that requires underlying data to be copied.
`ComparableValue<T, bool>` carries a set of `explicit` constructors that allow conversion from normal and reversed types.

Function `reverse_sort_key` provides functionality to flip `bool` template argument and return a copy of the object with reversed comparision flag:

```c++
template<typename T, bool R> ComparableValue<T, !R> reverse_sort_key(const ComparableValue<T, R>& other)
{
    return ComparableValue<T,!R>{other};
}
```

## Type aliases

There are bunch of alias helpers to define common types. 

```c++
using NumericValue = rez_int;

template<bool _Rev> using NumericTokenT = ComparableValue<NumericValue, _Rev>;
using NumericToken = NumericTokenT<false>;
using ReversedNumericToken = NumericTokenT<true>;
```

## Initialization

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

Partial specialization for `ComparableValue<T, false>`:

```c++
template<bool _> bool operator<(const ComparableValue<value_type, _>& other ) const REZ_NOEXCEPT
{
    return (Get() < other.Get());
}
```

Partial specialization for `ComparableValue<T, true>`:

```c++
template<bool _> bool operator<(const ComparableValue<value_type, _>& other ) const REZ_NOEXCEPT
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