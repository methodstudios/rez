#ifndef VERSION_HPP
#define VERSION_HPP

#include "token.hpp"

#include <functional>

namespace rez
{

template <typename T> class Version
{
public:
    explicit Version(string_view ver_str, const T&)
    {
        if (!ver_str.empty())
        {
            // find tokens
        }
    }

private:
    std::string str_;
    std::hash<std::string> hash_;

    std::vector<T> tokens_;
    std::vector<string_view> seps_;
};

using NumericVersion = Version<NumericToken>;
using AlphanumericVersion = Version<AlphanumericToken>;

} // namespace rez

#endif // VERSION_HPP
