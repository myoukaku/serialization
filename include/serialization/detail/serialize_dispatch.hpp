/**
 *	@file	serialize_dispatch.hpp
 *
 *	@brief	serialize_dispatch の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SERIALIZE_DISPATCH_HPP
#define SERIALIZATION_SERIALIZE_DISPATCH_HPP

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
	template <typename... Args>
	struct is_serialize_invocable
	{
	private:
		template <typename... Args2>
		static auto test(int) -> decltype(
			serialize(std::declval<Args2>()...),
			std::true_type());

		template <typename... Args2>
		static auto test(...) -> std::false_type;

		using type = decltype(test<Args...>(0));

	public:
		static const bool value = type::value;
	};

	template <typename>
	struct always_false { static const bool value = false; };

public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T const& t, serialization::version_t version)
	{
		if constexpr (is_serialize_invocable<Archive&, T&, serialization::version_t>::value)
		{
			serialize(ar, const_cast<T&>(t), version);
		}
		else if constexpr (is_serialize_invocable<Archive&, T&>::value)
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
