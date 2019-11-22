#include "Version.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pybind11 { namespace detail {

template <> struct type_caster<string_view> : string_caster<string_view, true>
{
};

}} // detail::pybind11

namespace py = pybind11;

template<typename T, bool _Rev, typename P> py::class_<Comparable<T, _Rev>> define_comparable_value(py::module& m, const char* name, py::class_<P>& parent)
{
    // aliases
    using Cls = Comparable<T, _Rev>;
    using Rev = Comparable<T,!_Rev>;

    py::class_<Cls> token{m, name, parent, py::module_local()};

    // <
    token.def("__lt__", [](const Cls& l, const typename Cls::value_type& r){ return l < r;});
    token.def("__lt__", [](const Cls& l, const Cls& r){ return l < r;});
    token.def("__lt__", [](const Cls& l, const Rev& r){ return l < r;}); // !reversed

    // >

    // str
    auto to_string_ptr = static_cast<std::string (*)(const Cls&)>(&to_string);
    token.def("__str__", to_string_ptr);
    token.def("__repr__", to_string_ptr);

    // reverse must be bounded through a pointer, resolve ambiguity from r-value overload
    auto reverse_sort_key_ptr = static_cast<Rev (*)(const Cls&)>(&reverse_sort_key<typename Cls::value_type, _Rev>);
    m.def("reverse_sort_key", reverse_sort_key_ptr);

    return token;
}

PYBIND11_MODULE(_version, m)
{
    class VersionToken{};
    py::class_<VersionToken> version_token{m, "VersionToken", py::module_local()};

    class _ReversedComparable{};
    py::class_<_ReversedComparable> reversed_comparable{m, "_ReversedComparable", py::module_local()};

    // NumericToken
    auto nn_token = define_comparable_value<NumericValue, false>(m, "NumericToken", version_token);
    auto nr_token = define_comparable_value<NumericValue, true>(m, "ReversedNumericToken", version_token);
    m.def("create_numeric_token", &create_numeric_token<false>);

    auto an_token = define_comparable_value<AlphanumericValue , false>(m, "AlphanumericToken", version_token);
    auto ar_token = define_comparable_value<AlphanumericValue , true>(m, "ReversedAlphanumericToken", version_token);
    m.def("create_alphanumeric_token", &create_alphanumeric_token<false>);
}