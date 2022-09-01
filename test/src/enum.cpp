/**
 *	@file	enum.cpp
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

namespace enum_test
{

enum Eum1
{
	Value1_1, Value1_2,
};

enum Eum2 : std::uint8_t
{
	Value2_1, Value2_2, Value2_3,
};

enum Eum3 : std::uint16_t
{
	Value3_1, Value3_2, Value3_3,
};

enum class Eum4
{
	Value1, Value2, Value3,
};

template <typename Stream, typename OArchive, typename IArchive>
void EnumTest2()
{
	Eum1 const e1 = Value1_1;
	Eum2 const e2 = Value2_3;
	Eum3 const e3 = Value3_2;
	Eum4 const e4 = Eum4::Value1;
	Eum4 const e5 = Eum4::Value2;

	Stream str;
	{
		OArchive oa(str);
		oa  << e1
		    << e2
		    << e3
		    << e4
		    << e5;
	}
	{
		Eum1 a;
		Eum2 b;
		Eum3 c;
		Eum4 d;
		Eum4 e;

		IArchive ia(str);

		ia  >> a
		    >> b
		    >> c
		    >> d
		    >> e;
		EXPECT_EQ(a, e1);
		EXPECT_EQ(b, e2);
		EXPECT_EQ(c, e3);
		EXPECT_EQ(d, e4);
		EXPECT_EQ(e, e5);
	}
}

template <typename OArchive, typename IArchive>
void EnumTest()
{
	EnumTest2<std::stringstream,  OArchive, IArchive>();
	EnumTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, EnumTest)
{
	EnumTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	EnumTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	EnumTest<
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace enum_test

}	// namespace serialization_test
