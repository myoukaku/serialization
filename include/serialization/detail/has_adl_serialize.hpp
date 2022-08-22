/**
 *	@file	has_adl_serialize.hpp
 *
 *	@brief	has_adl_serialize の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_HAS_ADL_SERIALIZE_HPP
#define SERIALIZATION_HAS_ADL_SERIALIZE_HPP

#include <type_traits>
#include <utility>

namespace serialization
{

namespace detail
{

template <typename... Args>
struct has_adl_serialize
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

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_HAS_ADL_SERIALIZE_HPP
