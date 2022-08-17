/**
 *	@file	get_version.hpp
 *
 *	@brief	get_version の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_GET_VERSION_HPP
#define SERIALIZATION_GET_VERSION_HPP

#include <serialization/version.hpp>

namespace serialization
{

template <typename T>
inline constexpr version_t get_version(T const& t)
{
	return 0;
}

namespace detail
{

struct get_version_fn
{
	template <typename T>
	inline constexpr version_t operator()(T const& t) const
	{
		return get_version(t);
	}
};

inline namespace cpo
{

inline constexpr get_version_fn get_version{};

}	// inline namespace cpo

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_GET_VERSION_HPP
