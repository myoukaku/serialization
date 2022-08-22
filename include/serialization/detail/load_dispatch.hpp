/**
 *	@file	load_dispatch.hpp
 *
 *	@brief	load_dispatch の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_LOAD_DISPATCH_HPP
#define SERIALIZATION_LOAD_DISPATCH_HPP

#include <serialization/detail/serialize_dispatch.hpp>
#include <serialization/detail/has_adl_load.hpp>
#include <serialization/nvp.hpp>
#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

namespace detail
{

template <typename Archive, typename T>
void load_array(Archive& ia, T& t)
{
	for (std::size_t i = 0; i < std::extent_v<T>; ++i)
	{
		ia >> t[i];
	}
}

template <typename Archive, typename T>
void load_object(Archive& ar, T& t)
{
	serialization::version_t version;

	// version_t を load
	ar >> make_nvp("version", version);

	if constexpr (has_adl_load<Archive&, T&, serialization::version_t>::value)
	{
		load(ar, t, version);
	}
	else if constexpr (has_adl_load<Archive&, T&>::value)
	{
		load(ar, t);
	}
	else
	{
		serialize_dispatch::invoke(ar, t, version);
	}
}

class load_dispatch
{
public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T& t)
	{
		if constexpr (std::is_array<T>::value)
		{
			load_array(ar, t);
		}
		else if constexpr (std::is_arithmetic<T>::value)
		{
			load_arithmetic(ar, t);
		}
		else if constexpr (std::is_enum<T>::value)
		{
			std::underlying_type_t<T> tmp;
			load_arithmetic(ar, tmp);
			t = static_cast<T>(tmp);
		}
		else
		{
			load_object(ar, t);
		}
	}
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_LOAD_DISPATCH_HPP
