/**
 *	@file	xml.cpp
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

namespace xml_test
{

template <typename Stream, typename OArchive, typename IArchive>
void XmlTest()
{
	Stream str;
	{
		int a = 42;
		float b = 1.5f;
		bool c = true;
		OArchive oa(str);
		oa << a;
		oa << serialization::make_nvp("b", b);
		oa << c;
	}
	std::string expected =
R"(<?xml version="1.0"?>
<serialization>
    <value0>42</value0>
    <b>1.5</b>
    <value1>true</value1>
</serialization>
)";
	EXPECT_EQ(expected, str.str());
	{
		int a;
		float b;
		bool c = true;
		IArchive ia(str);
		ia >> a;
		ia >> serialization::make_nvp("b", b);
		ia >> c;
		EXPECT_EQ(42, a);
		EXPECT_EQ(1.5f, b);
		EXPECT_EQ(true, c);
	}
}

GTEST_TEST(SerializationTest, XmlTest)
{
	XmlTest<
		std::stringstream,
		serialization::xml_oarchive,
		serialization::xml_iarchive
	>();
}

}	// namespace xml_test

}	// namespace serialization_test
