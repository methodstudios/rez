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

    static const VersionT<value_type>& Inf() REZ_NOEXCEPT;

    VersionT() = default;

    // copy constructor is explicit to avoid unintentional copying
    explicit VersionT(const VersionT<value_type>& other)
        : tokens{other.tokens}
        , seps{other.seps}
        , _str{new std::string{*other._str}}
        , _hash{other._hash}
    {
    }

    VersionT(VersionT<value_type>&& other) REZ_NOEXCEPT
        : tokens{std::move(other.tokens)}
        , seps{std::move(other.seps)}
        , _str{std::move(other._str)}
        , _hash(other._hash)
    {
    }

    explicit VersionT(const std::vector<value_type>& other_tokens)
        : tokens{other_tokens}
    {
    }

    explicit VersionT(std::vector<value_type>&& other_tokens)
        : tokens{std::move(other_tokens)}
    {
    }

    explicit operator bool() const REZ_NOEXCEPT { return !IsEmpty(); }

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
            size_t seed{};
            _hash = hash_combine(seed, std::string(*this));
        }
        return _hash;
    }

    explicit operator std::string() const
    {
        if(_str)
        {
            return *_str;
        }

        if(IsEmpty())
        {
            _str = std::unique_ptr<std::string>{new std::string{""}};
            return *_str;
        }

        if (IsInfinity())
        {
            _str = std::unique_ptr<std::string>{new std::string{"[INF]"}};
            return *_str;
        }

        _str = std::unique_ptr<std::string>{new std::string{}};
        size_t index{};
        for (; index < seps.size(); ++index)
        {
            _str->append(to_string(tokens[index]));
            _str->append(1, seps[index]);
        }
        _str->append(to_string(tokens[index]));
        return *_str;
    }

    bool IsEmpty() const REZ_NOEXCEPT { return tokens.empty(); }
    bool IsInfinity() const REZ_NOEXCEPT { return *this == Inf(); }

    // TODO benchmark if using > directly yields better results
    template <typename T> bool operator<(const VersionT<T>& other) const REZ_NOEXCEPT
    {
        if (IsInfinity()) return false;
        if (other.IsInfinity()) return true;
        return tokens < other.tokens;
    }
    template <typename T> bool operator>(const VersionT<T>& other) const REZ_NOEXCEPT
    {
        return !(*this < other || *this == other);
    }

    template <typename T> bool operator==(const VersionT<T>& other) const REZ_NOEXCEPT { return tokens == other.tokens; }
    template <typename T> bool operator<=(const VersionT<T>& other) const REZ_NOEXCEPT
    {
        return *this < other || *this == other;
    }
    template <typename T> bool operator>=(const VersionT<T>& other) const REZ_NOEXCEPT { return !(*this < other); }

    const std::vector<value_type>* operator->() const REZ_NOEXCEPT { return &tokens; }
    std::vector<value_type>* operator->() REZ_NOEXCEPT { return &tokens; }

    std::vector<value_type> tokens;
    std::vector<char> seps;

private:
    mutable std::unique_ptr<std::string> _str;
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
template <typename _Tok, template <typename> class CreationPolicy>
struct PolicyTypeFactory<VersionT<_Tok>, CreationPolicy> : CreationPolicy<VersionT<_Tok>>
{
    using version_type = VersionT<_Tok>;

    using value_type = typename CreationPolicy<version_type>::value_type;
    using return_type = typename CreationPolicy<version_type>::return_type;

    using CreationPolicy<version_type>::New;
    using CreationPolicy<version_type>::Get;

    static return_type Create(string_view version_str)
    {
        return_type new_result = New();
        value_type& result = Get(new_result);

        if (version_str.empty())
        {
            return new_result;
        }

        //
        std::vector<char> separators;
        std::vector<string_view> tokens = VersionSeparatorSplit(version_str, &separators);

        if (tokens.empty() || tokens.size() <= separators.size())
        {
            throw VersionError("Invalid version syntax: " + version_str.to_string());
        }
        result.seps = std::move(separators);

        // create tokens
        try
        {
            for (const auto& token : tokens)
            {
                result->push_back(Factory<_Tok>::Create(token));
            }
        }
        catch (const std::exception& e)
        {
            throw VersionError("Invalid version");
        }

        return new_result;
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

//
// Version traits
//

template <typename _Typ> struct is_version : std::false_type
{
};

template <typename _Typ> struct is_version<VersionT<_Typ>> : std::true_type
{
};

//
// to string
//
template <typename _Tok> std::string to_string(const VersionT<_Tok>& other) { return std::string{other}; }

#endif // REZ_VERSION_HPP
