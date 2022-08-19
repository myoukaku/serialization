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

template <typename Archive, typename T>
void save_array(Archive& oa, T const& t)
{
	for (std::size_t i = 0; i < std::extent_v<T>; ++i)
	{
		oa << t[i];
	}
}

template <typename Archive, typename T>
void save_nvp(Archive& oa, nvp<T> const& t)
{
	oa << t.value();
}

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

		// version_t を save
		ar << make_nvp("version", version);

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
		else if constexpr (is_nvp<T>::value)
		{
			save_nvp(ar, t);
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
