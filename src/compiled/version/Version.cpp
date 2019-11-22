#include "Version.hpp"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace pybind11 { namespace detail {

template <> struct type_caster<string_view> : string_caster<string_view, true>
{
//    PYBIND11_TYPE_CASTER(nonstd::string_view, _("nonstd::string_view"));

    // template <typename CharT, class Traits>
    // struct type_caster<nonstd::basic_string_view<CharT, Traits>, enable_if_t<is_std_char_type<CharT>::value>>
    //    : string_caster<nonstd::basic_string_view<CharT, Traits>, true>
    //{
    //};
};

}} // detail::pybind11

namespace py = pybind11;

// todo this might be more gneric to handle numeric and alphanumeric objects
template<bool _Rev, typename P> py::class_<NumericTokenT<_Rev>> define_token(py::module& m, const char* name, py::class_<P>& parent)
{
    py::class_<NumericTokenT<_Rev>> token{m, name, parent, py::module_local()};

    // <
    token.def("__lt__", [](const NumericTokenT<_Rev>& l, const typename NumericTokenT<_Rev>::value_type& r){ return l < r;});
    token.def("__lt__", [](const NumericTokenT<_Rev>& l, const NumericTokenT<_Rev>& r){ return l < r;});
    token.def("__lt__", [](const NumericTokenT<_Rev>& l, const NumericTokenT<!_Rev>& r){ return l < r;}); // !reversed

    // str
    token.def("__str__", [](const NumericTokenT<_Rev>& l){ return to_string(l);});
    token.def("__repr__", [](const NumericTokenT<_Rev>& l){ return to_string(l);});

    // reverse
    m.def("reverse_sort_key", &reverse_sort_key<typename NumericTokenT<_Rev>::value_type, _Rev>);

    return token;
}

template<typename T, bool _Rev, typename P> py::class_<ComparableValue<T, _Rev>> define_comparable_value(py::module& m, const char* name, py::class_<P>& parent)
{
    py::class_<ComparableValue<T, _Rev>> token{m, name, parent, py::module_local()};

    // <
    token.def("__lt__", [](const ComparableValue<T, _Rev>& l, const typename ComparableValue<T, _Rev>::value_type& r){ return l < r;});
    token.def("__lt__", [](const ComparableValue<T, _Rev>& l, const ComparableValue<T, _Rev>& r){ return l < r;});
    token.def("__lt__", [](const ComparableValue<T, _Rev>& l, const ComparableValue<T, _Rev>& r){ return l < r;}); // !reversed

    // str
    token.def("__str__", [](const ComparableValue<T, _Rev>& l){ return to_string(l);});
    token.def("__repr__", [](const ComparableValue<T, _Rev>& l){ return to_string(l);});

    // reverse
    m.def("reverse_sort_key", &reverse_sort_key<typename ComparableValue<T, _Rev>::value_type, _Rev>);

    return token;
}


PYBIND11_MODULE(_version, m)
{
    class VersionToken{};
    py::class_<VersionToken> version_token{m, "VersionToken", py::module_local()};

    // NumericToken
    define_comparable_value<NumericValue, false>(m, "NumericToken", version_token);
    define_comparable_value<NumericValue, true>(m, "ReversedNumericToken", version_token);
    m.def("create_numeric_token", &create_numeric_token<false>);

    auto an_token = define_comparable_value<AlphanumericValue , false>(m, "AlphanumericToken", version_token);
    auto ar_token = define_comparable_value<AlphanumericValue , true>(m, "ReversedAlphanumericToken", version_token);
    m.def("create_alphanumeric_token", &create_alphanumeric_token<false>);
}