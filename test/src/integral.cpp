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
		oa << rnd << min << max << low;
	}
	{
		T a;
		T b;
		T c;
		T d;

		IArchive ia(str);

		ia >> a >> b >> c >> d;
		EXPECT_EQ(a, rnd);
		EXPECT_EQ(b, min);
		EXPECT_EQ(c, max);
		EXPECT_EQ(d, low);
	}
}

template <typename Stream, typename OArchive, typename IArchive>
void IntegralTest2()
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

template <typename OArchive, typename IArchive>
void IntegralTest()
{
	IntegralTest2<std::stringstream,  OArchive, IArchive>();
	IntegralTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, IntegralTest)
{
	IntegralTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	IntegralTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace integral_test

}	// namespace serialization_test
