/**
 *	@file	serialize_dispatch.hpp
 *
 *	@brief	serialize_dispatch の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SERIALIZE_DISPATCH_HPP
#define SERIALIZATION_SERIALIZE_DISPATCH_HPP

#include <serialization/access.hpp>
#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

namespace detail
{

class serialize_dispatch
{
private:
	template <typename Archive, typename T>
	struct is_serialize_v_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			serialize(std::declval<A2&>(), std::declval<T2&>(), std::declval<serialization::version_t>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	struct is_serialize_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			serialize(std::declval<A2&>(), std::declval<T2&>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename>
	struct always_false { static const bool value = false; };

public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T const& t, serialization::version_t version)
	{
		if constexpr (access::is_serialize_v_invocable<Archive, T>::value)
		{
			access::serialize_v(ar, t, version);
		}
		else if constexpr (access::is_serialize_invocable<Archive, T>::value)
		{
			access::serialize(ar, t);
		}
		else if constexpr (is_serialize_v_invocable<Archive, T>::value)
		{
			serialize(ar, const_cast<T&>(t), version);
		}
		else if constexpr (is_serialize_invocable<Archive, T>::value)
		{
			serialize(ar, const_cast<T&>(t));
		}
		else
		{
			static_assert(always_false<T>::value, "This type is not serializable.");
		}
	}
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_SERIALIZE_DISPATCH_HPP
