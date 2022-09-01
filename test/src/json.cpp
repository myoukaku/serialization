/**
 *	@file	json.cpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#include <serialization.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include "serialization_test_utility.hpp"

namespace serialization_test
{

namespace json_test
{

template <typename Stream, typename OArchive, typename IArchive>
void JsonTest()
{
	Stream str;
	{
		int a = 42;
		float b = 1.5f;
		bool c = true;
		std::string d = "hello world!";
		int e[5] = {1, 1, 2, 3, 5};
		OArchive oa(str);
		oa << a;
		oa << serialization::make_nvp("b", b);
		oa << c;
		oa << serialization::make_nvp("d", d);
		oa << e;
	}
	std::string expected =
R"({
    "value0" : 42,
    "b" : 1.5,
    "value1" : true,
    "d" : "hello world!",
    "value2" : [
        1,
        1,
        2,
        3,
        5
    ]
})";
	EXPECT_EQ(expected, str.str());
	{
		int a;
		float b;
		bool c;
		std::string d;
		int e[5];
		IArchive ia(str);
		ia >> a;
		ia >> b;
		ia >> c;
		ia >> d;
		ia >> e;
		EXPECT_EQ(42, a);
		EXPECT_EQ(1.5f, b);
		EXPECT_EQ(true, c);
		EXPECT_EQ("hello world!", d);
		EXPECT_EQ(1, e[0]);
		EXPECT_EQ(1, e[1]);
		EXPECT_EQ(2, e[2]);
		EXPECT_EQ(3, e[3]);
		EXPECT_EQ(5, e[4]);
	}
}

GTEST_TEST(SerializationTest, JsonTest)
{
	JsonTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	//JsonTest<
	//	std::wstringstream,
	//	serialization::json_oarchive,
	//	serialization::json_iarchive
	//>();
}

}	// namespace json_test

}	// namespace serialization_test
