#ifndef REZ_VERSION_HPP
#define REZ_VERSION_HPP

#include "Exceptions.hpp"
#include "Token.hpp"

//
// Version class template
//
template <typename _Tok> struct VersionT
{
    using value_type = _Tok;

    explicit operator bool() const REZ_NOEXCEPT { return tokens.size() > 0; }

    value_type& operator[](size_t index) REZ_NOEXCEPT
    {
        assert(index < tokens.size());
        return tokens[index];
    }
    const value_type& operator[](size_t index) const REZ_NOEXCEPT
    {
        assert(index < tokens.size());
        return tokens[index];
    }

    const value_type& Major() const REZ_NOEXCEPT
    {
        assert(tokens.size() > 0);
        return tokens[0];
    }
    const value_type& Minor() const REZ_NOEXCEPT
    {
        assert(tokens.size() > 1);
        return tokens[1];
    }
    const value_type& Patch() const REZ_NOEXCEPT
    {
        assert(tokens.size() > 2);
        return tokens[2];
    }

    size_t Hash() const REZ_NOEXCEPT
    {
        if (_hash == REZ_INDEX_INVALID)
        {
            static std::hash<std::string> string_hash;
            _hash = string_hash(_str);
            return _hash;
        }
        return _hash;
    }

    template <typename T> bool operator<(const VersionT<T>& other) const REZ_NOEXCEPT { return tokens < other.tokens; }

    const std::vector<value_type>* operator->() const REZ_NOEXCEPT { return &tokens; }
    std::vector<value_type>* operator->() REZ_NOEXCEPT { return &tokens; }

    std::vector<value_type> tokens;
    std::vector<char> seps;

    mutable std::string _str;
    mutable size_t _hash{REZ_INDEX_INVALID};
};

//
// Version Aliases
//
using NumericVersion = VersionT<NumericToken>;
using AlphanumericVersion = VersionT<AlphanumericToken>;

//
// Factory
//
template <typename _Tok> struct Factory<VersionT<_Tok>>
{
    using value_type = _Tok;

    static VersionT<value_type> Create(string_view version_str)
    {
        VersionT<value_type> result;
        if (version_str.empty())
        {
            return result;
        }

        //
        std::vector<char> separators;
        std::vector<string_view> tokens = VersionSeparatorSplit(version_str, &separators);

        if (tokens.size() == 0 || tokens.size() <= separators.size() )
        {
            throw VersionError("Invalid version syntax: " + version_str.to_string());
        }
        result.seps = std::move(separators);

        // create tokens
        try
        {
            for (const auto& token : tokens)
            {
                result->push_back(Factory<value_type>::Create(token));
            }
        }
        catch (const std::exception& e)
        {
            throw VersionError("Invalid version");
        }

        return result;
    }

private:
    static std::vector<string_view> VersionSeparatorSplit(string_view str, std::vector<char>* separators = nullptr)
    {
        std::vector<string_view> result;

        size_t index = 0, count = 0;
        for (; index < str.size(); ++index)
        {
            const auto& c = str[index];
            if (!is_version_separator(c))
            {
                ++count;
                continue;
            }

            if (separators) separators->push_back(c);
            if (count) result.push_back(str.substr(index - count, count));

            count = 0;
        }

        if (count) result.push_back(str.substr(index - count, count));

        return result;
    }
};

inline NumericVersion operator"" _nv(const char* v, size_t s) { return Factory<NumericVersion>::Create(string_view{v, s}); }
inline AlphanumericVersion operator"" _anv(const char* v, size_t s)
{
    return Factory<AlphanumericVersion>::Create(string_view{v, s});
}

#endif // REZ_VERSION_HPP
