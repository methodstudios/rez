#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

namespace rez
{

class VersionError : public std::runtime_error
{
public:
    explicit VersionError(const std::string& message)
        : std::runtime_error(message)
    {
    }
};

} // namespace rez
#endif // EXCEPTIONS_HPP
