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
#include <serialization/nvp.hpp>
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
	template <typename... Args>
	struct is_save_invocable
	{
	private:
		template <typename... Args2>
		static auto test(int) -> decltype(
			save(std::declval<Args2>()...),
			std::true_type());

		template <typename... Args2>
		static auto test(...) -> std::false_type;

		using type = decltype(test<Args...>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	static void save_object(Archive& ar, T const& t)
	{
		serialization::version_t const version = serialization::detail::get_version(t);

		// nvp<version_t> を save
		auto version_nvp = make_nvp("version", version);
		if constexpr (!std::is_same_v<decltype(version_nvp), T>)	// 無限ループ防止
		{
			ar << version_nvp;
		}

		if constexpr (is_save_invocable<Archive&, T const&, serialization::version_t>::value)
		{
			save(ar, t, version);
		}
		else if constexpr (is_save_invocable<Archive&, T const&>::value)
		{
			save(ar, t);
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
		if constexpr (std::is_array<T>::value)
		{
			for (std::size_t i = 0; i < std::extent_v<T>; ++i)
			{
				ar << t[i];
			}
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
