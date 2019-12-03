#ifndef REZ_BOUND_HPP
#define REZ_BOUND_HPP

#include "Version.hpp"

#include <memory>

//
// Lower Bound
//
template <typename _Typ> struct LowerBound
{
    static_assert(is_version<_Typ>::value, "Invalid argument, instance VersionT required!");

    using token_type = typename _Typ::value_type;
    using value_type = VersionT<token_type>;

    static const LowerBound<value_type>& Min() REZ_NOEXCEPT;

    LowerBound(value_type&& ver, bool inc)
        : version{std::move(ver)}
        , inclusive{inc}
    {
    }

    LowerBound(string_view ver, bool inc)
        : LowerBound{Factory<value_type>::Create(ver), inc}
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

    value_type version;
    bool inclusive;
};

//
// Upper Bound
//
template <typename _Typ> struct UpperBound
{
    static_assert(is_version<_Typ>::value, "Invalid argument, instance VersionT required!");

    using value_type = _Typ;
    using token_type = typename value_type::value_type;

    static const UpperBound<value_type>& Inf() REZ_NOEXCEPT;

    UpperBound(VersionT<token_type>&& ver, bool inc)
        : version{std::move(ver)}
        , inclusive{inc}
    {
        if (!inclusive && !version)
        {
            throw VersionError("Invalid upper bound:" + std::string(*this));
        }
    }

    UpperBound(string_view ver, bool inc)
        : UpperBound{Factory<value_type>::Create(ver), inc}
    {
    }

    bool operator==(const UpperBound<value_type>& other) const REZ_NOEXCEPT
    {
        return inclusive == other.inclusive && version == other.version;
    }

    bool operator<(const UpperBound<value_type>& other) const REZ_NOEXCEPT
    {
        return (version < other.version) || ((version == other.version) && (!inclusive && other.inclusive));
    }

    size_t Hash() const REZ_NOEXCEPT
    {
        auto hash = version.Hash();
        return hash_combine(hash, inclusive);
    }

    bool contains_version(const value_type& ver) const REZ_NOEXCEPT
    {
        return (ver < version) || (inclusive && (ver == version));
    }

    explicit operator std::string() const
    {
        return inclusive ? "<=" + std::string(version) : "<" + std::string(version);
    }

    VersionT<token_type> version;
    bool inclusive;
};

//
// Upper Bound
//
template <typename _Typ> struct Bound
{
    using value_type = _Typ;

    using LowerBoundPtr = std::unique_ptr<const LowerBound<value_type>>;
    using UpperBoundPtr = std::unique_ptr<const UpperBound<value_type>>;

    static const Bound<value_type>& Any() REZ_NOEXCEPT;

    explicit Bound(const LowerBound<value_type>* l = nullptr, const UpperBound<value_type>* u = nullptr, bool invalid_bound_error = true)
    {
        // copies infinity
        lower = l ? LowerBoundPtr{l} : LowerBoundPtr{new LowerBound<value_type>{LowerBound<value_type>::Min()}};
        upper = u ? UpperBoundPtr{u} : UpperBoundPtr{new UpperBound<value_type>{UpperBound<value_type>::Inf()}};

        assert(lower != nullptr);
        assert(upper != nullptr);

        if(invalid_bound_error &&
            (lower->version > upper->version ||
            ((lower->version == upper->version) && !(lower->inclusive and upper->inclusive))))
        {
            throw VersionError("Invalid bound");
        }
    }

    // construct in place
    explicit Bound(value_type&& l, bool l_inc, value_type&& u, bool u_inc, bool invalid_bound_error = true)
        : Bound(new LowerBound<value_type>{std::move(l), l_inc}, new UpperBound<value_type>{std::move(u), u_inc}, invalid_bound_error)
    {
    }

    explicit Bound(string_view l, bool l_inc, string_view u, bool u_inc, bool invalid_bound_error = true)
        : Bound(Factory<value_type>::Create(l), l_inc, Factory<value_type>::Create(u), u_inc, invalid_bound_error)
    {
    }

    template<typename T> bool operator==(const Bound<T>& other) const REZ_NOEXCEPT
    {
        return (*lower == *other.lower) && (*upper == *other.upper);
    }

    template<typename T> bool operator<(const Bound<T>& other) const REZ_NOEXCEPT
    {
        return std::tie(*lower, *upper) < std::tie(*other.lower, *other.upper);
    }

    size_t Hash() const REZ_NOEXCEPT
    {
        return hash_combine(lower->Hash(), upper->Hash());
    }

    bool lower_bounded() const REZ_NOEXCEPT
    {
        return (*lower != LowerBound<value_type>::Min());
    }

    bool upper_bounded() const REZ_NOEXCEPT
    {
        return (*lower != UpperBound<value_type>::Inf());
    }

    bool contains_version(const value_type& ver) const REZ_NOEXCEPT
    {
        return version_containment(ver) == 0;
    }

    // <=>
    rez_int version_containment(const value_type& ver) const REZ_NOEXCEPT
    {
        if(!lower->contains_version(ver)) return -1;
        if(!upper->contains_version(ver)) return 1;
        return 0;
    }

    bool contains_bound(const Bound<value_type>& bound) const REZ_NOEXCEPT
    {
        return (*lower < *bound.lower) && (*upper < *bound.upper);
    }

    bool intersects(const Bound<value_type>& other) const REZ_NOEXCEPT
    {
        const LowerBound<value_type>* l = *lower < *other.lower ? other.lower.get() : lower.get();
        const UpperBound<value_type>* u = *upper < *other.upper ? upper.get() : other.upper.get();
        return (l->version < u->version) || ((l->inclusive && u->inclusive) && (l->version == u->version));
    }

    const Bound<value_type>* intersection(const Bound<value_type>& other) const REZ_NOEXCEPT
    {
        const LowerBound<value_type>* l = *lower < *other.lower ? other.lower.get() : lower.get();
        const UpperBound<value_type>* u = *upper < *other.upper ? upper.get() : other.upper.get();

        if((l->version < u->version) || ((l->inclusive && u->inclusive) && (l->version == u->version)))
        {
            return new Bound<value_type>{LowerBound<value_type>{*l}, UpperBound<value_type>{u}};
        }

        return nullptr;
    }

    LowerBoundPtr lower;
    UpperBoundPtr upper;
};

#endif // REZ_BOUND_HPP
