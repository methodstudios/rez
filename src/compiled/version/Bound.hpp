#ifndef REZ_BOUND_HPP
#define REZ_BOUND_HPP

#include "Version.hpp"

template <typename _Typ> struct LowerBound
{
    static_assert(is_version<_Typ>::value, "Invalid argument, instance VersionT required!");

    using token_type = typename _Typ::value_type;
    using value_type = VersionT<token_type>;

    static const LowerBound<value_type>& Min() REZ_NOEXCEPT;

    LowerBound(const value_type& ver, bool inc)
        : version{ver}
        , inclusive{inc}
    {
    }

    LowerBound(const LowerBound<_Typ>&) = default;
    LowerBound(LowerBound<_Typ>&&) REZ_NOEXCEPT = default;

    bool operator==(const LowerBound<value_type>& other) const REZ_NOEXCEPT
    {
        return inclusive == other.inclusive && version == other.version;
    }

    bool operator<(const LowerBound<value_type>& other) const REZ_NOEXCEPT
    {
        return (version < other.version) || (version == other.version && inclusive && !other.inclusive);
    }

    bool contains_version(const value_type& ver) const REZ_NOEXCEPT
    {
        return (ver > version) || (inclusive and (ver == version));
    }

    size_t Hash() const REZ_NOEXCEPT
    {
        auto hash = version.Hash();
        return hash_combine(hash, inclusive);
    }

    explicit operator std::string() const
    {
        if (!version.IsEmpty())
        {
            return inclusive ? std::string(version) + "+" : ">" + std::string(version);
        }
        return inclusive ? "" : ">";
    }

    const value_type& version;
    const bool inclusive;
};

template <typename _Typ> struct UpperBound
{
    static_assert(is_version<_Typ>::value, "Invalid argument, instance VersionT required!");

    using value_type = _Typ;
    using token_type = typename value_type::value_type;

    UpperBound(const VersionT<token_type>& ver, bool inc)
        : version{ver}
        , inclusive{inc}
    {
        if (!version && !inclusive)
        {
            throw VersionError("Invalid upper bound:");
        }
    }

    bool operator==(const LowerBound<value_type>& other) const REZ_NOEXCEPT
    {
        return version == other.version && inclusive == other.inclusive;
    }
    //
    //    bool operator<(const LowerBound<value_type>& rhs) const REZ_NOEXCEPT
    //    {
    //        return *version < *rhs.version && inclusive != rhs.inclusive;
    //    }
    //
    //    bool contains_version(const value_type& ver) const REZ_NOEXCEPT
    //    {
    //        return (ver < *version) || (inclusive && (ver == *version));
    //    }

    const VersionT<token_type>& version;
    bool inclusive;
};

template <typename _Typ> struct Bound
{
    using value_type = _Typ;

    const LowerBound<value_type>* lower;
    const UpperBound<value_type>* upperr;
};

#endif // REZ_BOUND_HPP
