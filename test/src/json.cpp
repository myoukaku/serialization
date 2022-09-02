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

struct Point
{
	float x;
	float y;
};

template <typename Archive>
void serialize(Archive& ar, Point& o)
{
	ar & serialization::make_nvp("x", o.x);
	ar & serialization::make_nvp("y", o.y);
}

class Object
{
private:
	int         a = 10;
	float       b = 12.5f;
	std::string c = "The quick brown fox";
	Point		d = { 1, 2 };

private:
	friend bool operator==(Object const& lhs, Object const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			true;
	}

	friend bool operator!=(Object const& lhs, Object const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Object& o)
	{
		ar & o.a;
		ar & o.b;
		ar & o.c;
		ar & o.d;
	}
};

template <typename Stream, typename OArchive, typename IArchive>
void JsonTest()
{
	Object obj;

	Stream str;
	{
		int a = 42;
		float b = 1.5f;
		bool c = true;
		std::string d = "hello world!";
		int e[5] = {1, 1, 2, 3, 5};
		int f[2][3] =
		{
			{11, 12, 13},
			{21, 22, 23},
		};
		OArchive oa(str);
		oa << a;
		oa << serialization::make_nvp("b", b);
		oa << c;
		oa << serialization::make_nvp("d", d);
		oa << e;
		oa << f;
		oa << serialization::make_nvp("obj", obj);
	}
	std::string expected =
R"({
    "value0": 42,
    "b": 1.5,
    "value1": true,
    "d": "hello world!",
    "value2": [
        1,
        1,
        2,
        3,
        5
    ],
    "value3": [
        [
            11,
            12,
            13
        ],
        [
            21,
            22,
            23
        ]
    ],
    "obj": {
        "version": 0,
        "value4": 10,
        "value5": 12.5,
        "value6": "The quick brown fox",
        "value7": {
            "version": 0,
            "x": 1,
            "y": 2
        }
    }
})";
	EXPECT_EQ(expected, str.str());
	{
		int a;
		float b;
		bool c;
		std::string d;
		int e[5];
		int f[2][3];
		Object g;

		IArchive ia(str);
		ia >> a;
		ia >> b;
		ia >> c;
		ia >> d;
		ia >> e;
		ia >> f;
		ia >> g;
		EXPECT_EQ(42, a);
		EXPECT_EQ(1.5f, b);
		EXPECT_EQ(true, c);
		EXPECT_EQ("hello world!", d);
		EXPECT_EQ(1, e[0]);
		EXPECT_EQ(1, e[1]);
		EXPECT_EQ(2, e[2]);
		EXPECT_EQ(3, e[3]);
		EXPECT_EQ(5, e[4]);
		EXPECT_EQ(11, f[0][0]);
		EXPECT_EQ(12, f[0][1]);
		EXPECT_EQ(13, f[0][2]);
		EXPECT_EQ(21, f[1][0]);
		EXPECT_EQ(22, f[1][1]);
		EXPECT_EQ(23, f[1][2]);
		EXPECT_EQ(obj, g);
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
