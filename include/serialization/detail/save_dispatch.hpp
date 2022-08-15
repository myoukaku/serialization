﻿/**
 *	@file	save_dispatch.hpp
 *
 *	@brief	save_dispatch の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SAVE_DISPATCH_HPP
#define SERIALIZATION_SAVE_DISPATCH_HPP

#include <serialization/detail/serialize_dispatch.hpp>
#include <serialization/access.hpp>
#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

namespace detail
{

class save_dispatch
{
private:
	template <typename Archive, typename T>
	struct is_save_v_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			save(std::declval<A2&>(), std::declval<T2 const&>(), std::declval<serialization::version_t>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	struct is_save_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			save(std::declval<A2&>(), std::declval<T2 const&>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	static void save_object(Archive& ar, T const& t)
	{
		serialization::version_t const version = 0;//detail::Version<T>::value;
		ar << version;

		if constexpr (access::is_save_v_invocable<Archive, T>::value)
		{
			access::save_version_memfun(ar, t, version);
		}
		else if constexpr (access::is_save_invocable<Archive, T>::value)
		{
			access::save_memfun(ar, t);
		}
		else if constexpr (is_save_v_invocable<Archive, T>::value)
		{
			save_version_free(ar, t, version);
		}
		else if constexpr (is_save_invocable<Archive, T>::value)
		{
			save_free(ar, t);
		}
		else
		{
			serialize_dispatch::invoke(ar, t, version);
		}
	}

public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T const& t)
	{
		//if constexpr (std::is_array<T>::value)
		//{
		//	return ar.save_array(t);
		//}
		if constexpr (std::is_arithmetic<T>::value)
		{
			ar.save_arithmetic(t);
		}
		else if constexpr (std::is_enum<T>::value)
		{
			ar.save_arithmetic(static_cast<std::underlying_type_t<T>>(t));
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
