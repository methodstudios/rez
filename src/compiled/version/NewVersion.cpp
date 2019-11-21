#include "NewVersion.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;

template<bool _Rev, typename P> py::class_<NumericTokenT<_Rev>> define_token(py::module& m, const char* name, py::class_<P>& parent)
{
    py::class_<NumericTokenT<_Rev>> token{m, name, parent, py::module_local()};
    token.def("__lt__", [](const NumericTokenT<_Rev>& l, const typename NumericTokenT<_Rev>::value_type& r){ return l < r;});
    token.def("__lt__", [](const NumericTokenT<_Rev>& l, const NumericTokenT<_Rev>& r){ return l < r;});
    token.def("__lt__", [](const NumericTokenT<_Rev>& l, const NumericTokenT<!_Rev>& r){ return l < r;});

    m.def("reverse_sort_key", &reverse_sort_key<typename NumericTokenT<_Rev>::value_type, _Rev>);

    return token;
}

PYBIND11_MODULE(_version, m)
{
    class VersionToken{};
    py::class_<VersionToken> version_token{m, "VersionToken", py::module_local()};

    define_token<false>(m, "NumericToken", version_token);
    define_token<true>(m, "ReversedNumericToken", version_token);

    m.def("create_numeric_token", &create_numeric_token<false>);

}