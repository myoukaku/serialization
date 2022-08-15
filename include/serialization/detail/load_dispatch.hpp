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
#include <serialization/access.hpp>
#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

namespace detail
{

class load_dispatch
{
private:
	template <typename Archive, typename T>
	struct is_load_v_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			load(std::declval<A2&>(), std::declval<T2&>(), std::declval<serialization::version_t>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	struct is_load_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			load(std::declval<A2&>(), std::declval<T2&>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	static void load_object(Archive& ar, T& t)
	{
		serialization::version_t version;
		ar >> version;

		if constexpr (access::is_load_v_invocable<Archive, T>::value)
		{
			access::load_v(ar, t, version);
		}
		else if constexpr (access::is_load_invocable<Archive, T>::value)
		{
			access::load(ar, t);
		}
		else if constexpr (is_load_v_invocable<Archive, T>::value)
		{
			load(ar, t, version);
		}
		else if constexpr (is_load_invocable<Archive, T>::value)
		{
			load(ar, t);
		}
		else
		{
			serialize_dispatch::invoke(ar, t, version);
		}
	}

public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T& t)
	{
		if constexpr (std::is_array<T>::value)
		{
			for (std::size_t i = 0; i < std::extent_v<T>; ++i)
			{
				ar >> t[i];
			}
		}
		else if constexpr (std::is_arithmetic_v<T>)
		{
			ar.load_arithmetic(t);
		}
		else if constexpr (std::is_enum<T>::value)
		{
			std::underlying_type_t<T> tmp;
			ar.load_arithmetic(tmp);
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
