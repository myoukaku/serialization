/**
 *	@file	string.cpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#include <serialization.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <limits>
#include <string>
#include "serialization_test_utility.hpp"

namespace serialization_test
{

namespace string_test
{

template <typename Stream, typename OArchive, typename IArchive>
void StringTest()
{
	std::string  s1_1;
	std::string  s1_2 = "Hello World";
	std::string  s1_3 = "0123 abcde \r\n\t\"\\\v\bABCDE\u3042\u3044\u3046";
	std::wstring ws1_1;
	std::wstring ws1_2 = L"Hello World";
	std::wstring ws1_3 = L"0123 abcde \r\n\t\"\\\v\bABCDE\u3042\u3044\u3046";

	Stream str;
	{
		OArchive oa(str);
		oa  << s1_1
			<< s1_2
			<< s1_3
			<< ws1_1
			<< ws1_2
			<< ws1_3;
	}
	{
		std::string  s2_1;
		std::string  s2_2;
		std::string  s2_3;
		std::wstring ws2_1;
		std::wstring ws2_2;
		std::wstring ws2_3;

		IArchive ia(str);

		ia  >> s2_1
		    >> s2_2
		    >> s2_3
		    >> ws2_1
		    >> ws2_2
		    >> ws2_3;
		EXPECT_EQ(s2_1, s1_1);
		EXPECT_EQ(s2_2, s1_2);
		EXPECT_EQ(s2_3, s1_3);
		EXPECT_EQ(ws2_1, ws1_1);
		EXPECT_EQ(ws2_2, ws1_2);
		EXPECT_EQ(ws2_3, ws1_3);
	}
}

GTEST_TEST(SerializationTest, StringTest)
{
	StringTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	StringTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	StringTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	StringTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	StringTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	StringTest<
		std::wstringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace string_test

}	// namespace serialization_test
