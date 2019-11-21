#include "version.hpp"

#include <pybind11/pybind11.h>

namespace py = pybind11;
using namespace rez;

template<typename T, typename P> py::class_<T> define_token(py::module& m, const char* name, py::class_<P>& parent)
{
    py::class_<T> token{m, name, parent, py::module_local()};
    token.def(py::init<const char*>());
    token.def("__repr__", [](const T& t) { return std::string(t); });
    token.def("__str__", [](const T& t) { return std::string(t); });
    token.def("__lt__", [](const T& lhs, const T& rhs)
    {
        return lhs < rhs;
    });
    return token;
}

// https://stackoverflow.com/questions/51058336/pybind-curiously-recurring-template
PYBIND11_MODULE(_version, m)
{
    // Version Value backward compatibility for clients who checked isinstance(obj, VersionToken)
    class VersionToken{};
    py::class_<VersionToken> version_token{m, "VersionToken", py::module_local()};

    auto numer_token = define_token<NumericToken, VersionToken>(m, "NumericToken", version_token);
    auto alnum_token = define_token<AlphanumericToken, VersionToken>(m, "AlphanumericVersionToken", version_token);
    alnum_token.def("__len__", &AlphanumericToken::Size);

//    static py::exception<VersionError> version_exception{m, "VersionError"};
//    py::register_exception_translator([](std::exception_ptr p) {
//        try
//        {
//            if (p)
//                std::rethrow_exception(p);
//        }
//        catch (const VersionError& e)
//        {
//            version_exception(e.what());
//        }
//    });
}