/**
 *	@file	access.hpp
 *
 *	@brief	access の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_ACCESS_HPP
#define SERIALIZATION_ACCESS_HPP

#include <serialization/version.hpp>
#include <type_traits>
#include <utility>

namespace serialization
{

class access
{
public:
	template <typename Archive, typename T>
	struct is_save_v_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			std::declval<T2>().save(
				std::declval<A2&>(),
				std::declval<serialization::version_t>()),
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
			std::declval<T2>().save(
				std::declval<A2&>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	struct is_load_v_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			std::declval<std::remove_const_t<T2>>().load(
				std::declval<A2&>(),
				std::declval<serialization::version_t>()),
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
			std::declval<std::remove_const_t<T2>>().load(
				std::declval<A2&>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	template <typename Archive, typename T>
	struct is_serialize_v_invocable
	{
	private:
		template <typename A2, typename T2>
		static auto test(int) -> decltype(
			std::declval<T2>().serialize(
				std::declval<A2&>(),
				std::declval<serialization::version_t>()),
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
			std::declval<T2>().serialize(
				std::declval<A2&>()),
			std::true_type());

		template <typename A2, typename T2>
		static auto test(...)->std::false_type;

		using type = decltype(test<Archive, T>(0));

	public:
		static const bool value = type::value;
	};

	struct save_v
	{
		template <typename Archive, typename T>
		static void invoke(Archive& ar, T const& t, serialization::version_t version)
		{
			t.save(ar, version);
		}
	};

	struct save
	{
		template <typename Archive, typename T>
		static void invoke(Archive& ar, T const& t)
		{
			t.save(ar);
		}
	};

	struct load_v
	{
		template <typename Archive, typename T>
		static void invoke(Archive& ar, T& t, serialization::version_t version)
		{
			t.load(ar, version);
		}
	};

	struct load
	{
		template <typename Archive, typename T>
		static void invoke(Archive& ar, T& t)
		{
			t.load(ar);
		}
	};

	template <typename Archive, typename T>
	static void serialize_v(Archive& ar, T const& t, serialization::version_t version)
	{
		const_cast<std::remove_const_t<T>&>(t).serialize(ar, version);
	}

	template <typename Archive, typename T>
	static void serialize(Archive& ar, T const& t)
	{
		const_cast<std::remove_const_t<T>&>(t).serialize(ar);
	}
};

}	// namespace serialization

#endif // SERIALIZATION_ACCESS_HPP
