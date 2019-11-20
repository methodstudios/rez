#ifndef VERSION_HPP
#define VERSION_HPP

#include "token.hpp"

namespace rez
{

struct VersionHelper
{
    std::string str_;
    std::hash<std::string> hash_;
};

template <typename T> class Version : public VersionHelper,
                                      public Value<std::vector<T>>
{
public:
    explicit Version(string_view ver_str, const T& value)
    {
        if (!ver_str.empty())
        {
            // find tokens
        }
    }

private:
    std::vector<string_view> seps_;
};

using NumericVersion = Version<NumericToken>;
using AlphanumericVersion = Version<AlphanumericToken>;

} // namespace rez

#endif // VERSION_HPP
