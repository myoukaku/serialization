/**
 *	@file	array.cpp
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

namespace array_test
{

enum class Eum
{
	Value1, Value2, Value3,
};

template <typename Stream, typename OArchive, typename IArchive>
void ArrayTest2()
{
	int const a1[] = {1,2,3,4,5};
	float const a2[2][3] =
	{
		{10,11,12},
		{20,21,22},
	};
	Eum const a3[] = {Eum::Value1,Eum::Value3,Eum::Value1,Eum::Value2,};

	Stream str;
	{
		OArchive oa(str);
		oa  << a1
		    << a2
			<< a3;
	}
	{
		int a[5];
		float b[2][3];
		Eum c[4];

		IArchive ia(str);

		ia  >> a
		    >> b
		    >> c;
		EXPECT_EQ(a[0], 1);
		EXPECT_EQ(a[1], 2);
		EXPECT_EQ(a[2], 3);
		EXPECT_EQ(a[3], 4);
		EXPECT_EQ(a[4], 5);
		EXPECT_EQ(b[0][0], 10);
		EXPECT_EQ(b[0][1], 11);
		EXPECT_EQ(b[0][2], 12);
		EXPECT_EQ(b[1][0], 20);
		EXPECT_EQ(b[1][1], 21);
		EXPECT_EQ(b[1][2], 22);
		EXPECT_EQ(c[0], Eum::Value1);
		EXPECT_EQ(c[1], Eum::Value3);
		EXPECT_EQ(c[2], Eum::Value1);
		EXPECT_EQ(c[3], Eum::Value2);
	}
}

template <typename OArchive, typename IArchive>
void ArrayTest()
{
	ArrayTest2<std::stringstream,  OArchive, IArchive>();
	ArrayTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, ArrayTest)
{
	ArrayTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	ArrayTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace array_test

}	// namespace serialization_test
