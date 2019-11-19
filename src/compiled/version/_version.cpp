#include "version.hpp"

#include <Python.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace rez;

namespace
{

template <typename _Cls> void _Comparable(py::class_<_Cls>& o)
{
    //    o.def("__lt__", &_Cls::operator<);
    //    o.def("__gt__", &_Cls::operator>);
    //    o.def("__le__", &_Cls::operator<=);
    //    o.def("__ge__", &_Cls::operator>=);
    //    o.def("__eq__", &_Cls::operator==);
    //    o.def("__ne__", &_Cls::operator!=);
}

} // namespace

//
// Module
//
PYBIND11_MODULE(_version, m)
{
    static py::exception<VersionError> version_exception{m, "VersionError"};
    py::register_exception_translator([](std::exception_ptr p) {
        try
        {
            if (p)
                std::rethrow_exception(p);
        }
        catch (const VersionError& e)
        {
            version_exception(e.what());
        }
    });

    // Version Value backward compatibility for clients who checked isinstance(obj, VersionToken)
    class VersionToken
    {
    };
    py::class_<VersionToken> version_token{m, "VersionToken"};

    // Numeric Value
    py::class_<NumericToken> numeric_token{m, "NumericToken", version_token};
    numeric_token.def(py::init<const char*>());
    numeric_token.def("__str__", [](const NumericToken& nt) { return std::string(nt); });
    _Comparable(numeric_token);

    // Alphanumeric Value
    py::class_<AlphanumericToken> alpha_token{m, "AlphanumericVersionToken", version_token};
    alpha_token.def(py::init<const char*>());
    alpha_token.def("__str__", [](const AlphanumericToken& at) { return std::string(at); });
    _Comparable(alpha_token);

    //    class Version{};
    //    py::class_<VersionToken> version{m, "Version"};
    //
    //    // proper constructor
    //    py::class_<NumericVersion> n_version{m, "NumericVersion", version};
    //    n_version.def(py::init<const NumericToken&>());
    //
    //    py::class_<AlphanumericVersion> a_version{m, "AlphanumericVersion", version};
    //    a_version.def(py::init<const AlphanumericToken&>());
}