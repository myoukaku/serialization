/**
 *	@file	save_dispatch.hpp
 *
 *	@brief	save_dispatch の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SAVE_DISPATCH_HPP
#define SERIALIZATION_SAVE_DISPATCH_HPP

#include <serialization/detail/serialize_dispatch.hpp>
#include <serialization/detail/get_version.hpp>
#include <serialization/detail/has_adl_save.hpp>
#include <serialization/nvp.hpp>
#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

namespace detail
{

template <typename Archive, typename T>
void save_array(Archive& oa, T const& t)
{
	for (std::size_t i = 0; i < std::extent_v<T>; ++i)
	{
		oa << t[i];
	}
}

template <typename Archive, typename T>
void save_object(Archive& ar, T const& t)
{
	serialization::version_t const version = serialization::detail::get_version(t);

	// version_t を save
	ar << make_nvp("version", version);

	if constexpr (has_adl_save<Archive&, T const&, serialization::version_t>::value)
	{
		save(ar, t, version);
	}
	else if constexpr (has_adl_save<Archive&, T const&>::value)
	{
		save(ar, t);
	}
	else
	{
		serialize_dispatch::invoke(ar, t, version);
	}
}

class save_dispatch
{
public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T const& t)
	{
		if constexpr (std::is_array<T>::value)
		{
			save_array(ar, t);
		}
		else if constexpr (std::is_arithmetic<T>::value)
		{
			save_arithmetic(ar, t);
		}
		else if constexpr (std::is_enum<T>::value)
		{
			save_arithmetic(ar, static_cast<std::underlying_type_t<T>>(t));
		}
		else
		{
			save_object(ar, t);
		}
	}
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_SAVE_DISPATCH_HPP
