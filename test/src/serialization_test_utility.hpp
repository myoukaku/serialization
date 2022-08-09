/**
 *	@file	serialization_test_utility.hpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_TEST_UTILITY_HPP
#define SERIALIZATION_TEST_UTILITY_HPP

#include <random>
#include <limits>

namespace
{

inline std::mt19937& get_random_engine()
{
	static std::mt19937 mt;
	return mt;
}

template <typename T>
inline T get_random_value()
{
	if constexpr (std::is_floating_point_v<T>)
	{
		std::uniform_real_distribution<T> dist(
			std::numeric_limits<T>::lowest() / 2,
			std::numeric_limits<T>::max() / 2);
		return dist(get_random_engine());
	}
	else if constexpr (std::is_unsigned_v<T>)
	{
		std::uniform_int_distribution<std::uintmax_t> dist(
			std::numeric_limits<T>::min(),
			std::numeric_limits<T>::max());
		return dist(get_random_engine());
	}
	else
	{
		std::uniform_int_distribution<std::intmax_t> dist(
			std::numeric_limits<T>::min(),
			std::numeric_limits<T>::max());
		return dist(get_random_engine());
	}
}

}	// namespace

#endif // SERIALIZATION_TEST_UTILITY_HPP
