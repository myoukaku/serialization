/**
 *	@file	integral.cpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#include <serialization.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <limits>
#include "serialization_test_utility.hpp"

namespace serialization_test
{

namespace integral_test
{

template <typename Stream, typename OArchive, typename IArchive, typename T>
void IntegralTestSub()
{
	T const rnd = get_random_value<T>();
	T const min = std::numeric_limits<T>::min();
	T const max = std::numeric_limits<T>::max();
	T const low = std::numeric_limits<T>::lowest();

	Stream str;
	{
		OArchive oa(str);
		oa << serialization::make_nvp("rnd", rnd)
		   << serialization::make_nvp("min", min)
		   << serialization::make_nvp("max", max)
		   << serialization::make_nvp("low", low);
	}
	{
		T a;
		T b;
		T c;
		T d;

		IArchive ia(str);

		ia >> serialization::make_nvp("rnd", a)
		   >> serialization::make_nvp("min", b)
		   >> serialization::make_nvp("max", c)
		   >> serialization::make_nvp("low", d);

		EXPECT_EQ(a, rnd);
		EXPECT_EQ(b, min);
		EXPECT_EQ(c, max);
		EXPECT_EQ(d, low);
	}
}

template <typename Stream, typename OArchive, typename IArchive>
void IntegralTest()
{
	IntegralTestSub<Stream, OArchive, IArchive, bool>();
	IntegralTestSub<Stream, OArchive, IArchive, char>();
	IntegralTestSub<Stream, OArchive, IArchive, signed char>();
	IntegralTestSub<Stream, OArchive, IArchive, unsigned char>();
	IntegralTestSub<Stream, OArchive, IArchive, wchar_t>();
	IntegralTestSub<Stream, OArchive, IArchive, short>();
	IntegralTestSub<Stream, OArchive, IArchive, unsigned short>();
	IntegralTestSub<Stream, OArchive, IArchive, int>();
	IntegralTestSub<Stream, OArchive, IArchive, unsigned int>();
	IntegralTestSub<Stream, OArchive, IArchive, long>();
	IntegralTestSub<Stream, OArchive, IArchive, unsigned long>();
	IntegralTestSub<Stream, OArchive, IArchive, long long>();
	IntegralTestSub<Stream, OArchive, IArchive, unsigned long long>();
	IntegralTestSub<Stream, OArchive, IArchive, char8_t>();
	IntegralTestSub<Stream, OArchive, IArchive, char16_t>();
	IntegralTestSub<Stream, OArchive, IArchive, char32_t>();
}

GTEST_TEST(SerializationTest, IntegralTest)
{
	IntegralTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	IntegralTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	IntegralTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	IntegralTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace integral_test

}	// namespace serialization_test
