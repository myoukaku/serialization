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
#include <serialization/nvp.hpp>
#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

template <typename Archive, typename T>
void load_array(Archive& ia, T& t)
{
	for (std::size_t i = 0; i < std::extent_v<T>; ++i)
	{
		ia >> t[i];
	}
}

namespace detail
{

class load_dispatch
{
private:
	template <typename... Args>
	struct is_load_invocable
	{
	private:
		template <typename... Args2>
		static auto test(int) -> decltype(
			load(std::declval<Args2>()...),
			std::true_type());

		template <typename... Args2>
		static auto test(...) -> std::false_type;

		using type = decltype(test<Args...>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	static void load_object(Archive& ar, T& t)
	{
		serialization::version_t version;

		// nvp<version_t> を load
		auto version_nvp = make_nvp("version", version);
		if constexpr (!std::is_same_v<decltype(version_nvp), T>)	// 無限ループ防止
		{
			ar >> version_nvp;
		}

		if constexpr (is_load_invocable<Archive&, T&, serialization::version_t>::value)
		{
			load(ar, t, version);
		}
		else if constexpr (is_load_invocable<Archive&, T&>::value)
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
			load_array(ar, t);
		}
		else if constexpr (std::is_arithmetic_v<T>)
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
