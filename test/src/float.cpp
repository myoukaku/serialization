/**
 *	@file	float.cpp
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

namespace float_test
{

template <typename Stream, typename OArchive, typename IArchive, typename T>
void FloatTestSub()
{
	T const rnd = get_random_value<T>();
	T const min = std::numeric_limits<T>::min();
	T const max = std::numeric_limits<T>::max();
	T const low = std::numeric_limits<T>::lowest();
	T const inf = std::numeric_limits<T>::infinity();
	T const nan = std::numeric_limits<T>::quiet_NaN();

	Stream str;
	{
		OArchive oa(str);
		oa << rnd
			<< min
			<< max
			<< low
			<< inf
			<< -inf
			<< nan
			<< -nan;
	}
	{
		T a;
		T b;
		T c;
		T d;
		T e;
		T f;
		T g;
		T h;

		IArchive ia(str);

		ia >> a >> b >> c >> d >> e >> f >> g >> h;
		EXPECT_EQ(a, rnd);
		EXPECT_EQ(b, min);
		EXPECT_EQ(c, max);
		EXPECT_EQ(d, low);
		EXPECT_EQ(e, inf);
		EXPECT_EQ(f, -inf);
		EXPECT_TRUE(std::isnan(g));
		EXPECT_TRUE(std::isnan(h));
	}
}

template <typename Stream, typename OArchive, typename IArchive>
void FloatTest2()
{
	FloatTestSub<Stream, OArchive, IArchive, float>();
	FloatTestSub<Stream, OArchive, IArchive, double>();
	FloatTestSub<Stream, OArchive, IArchive, long double>();
}

template <typename OArchive, typename IArchive>
void FloatTest()
{
	FloatTest2<std::stringstream,  OArchive, IArchive>();
	FloatTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, FloatTest)
{
	FloatTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	FloatTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace float_test

}	// namespace serialization_test
