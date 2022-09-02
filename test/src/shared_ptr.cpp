/**
 *	@file	shared_ptr.cpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#include <serialization.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "serialization_test_utility.hpp"

namespace serialization_test
{

namespace shared_ptr_test
{

template <typename Stream, typename OArchive, typename IArchive>
void SharedPtrTest()
{
	Stream str;
	{
		std::shared_ptr<int>  p1;
		std::shared_ptr<int>  p2(new int(10));
		std::shared_ptr<int>  p3(new int(20));
		std::shared_ptr<int>  p4(new int(30));
		std::shared_ptr<int>  p5(p3);
		std::shared_ptr<int>  p6(p2);
		std::shared_ptr<int>  p7(p5);

		OArchive oa(str);
		oa << p1 << p2 << p3 << p4 << p5 << p6 << p7;
	}
	{
		std::shared_ptr<int>  a;
		std::shared_ptr<int>  b;
		std::shared_ptr<int>  c;
		std::shared_ptr<int>  d;
		std::shared_ptr<int>  e;
		std::shared_ptr<int>  f;
		std::shared_ptr<int>  g;

		IArchive ia(str);

		ia >> a >> b >> c >> d >> e >> f >> g;

		EXPECT_EQ(nullptr, a);
		EXPECT_EQ(10, *b);
		EXPECT_EQ(20, *c);
		EXPECT_EQ(30, *d);
		EXPECT_EQ(20, *e);
		EXPECT_EQ(10, *f);
		EXPECT_EQ(20, *g);

		EXPECT_TRUE(b != c);
		EXPECT_TRUE(b != d);
		EXPECT_TRUE(b != e);
		EXPECT_TRUE(b == f);
		EXPECT_TRUE(b != g);
		EXPECT_TRUE(c != d);
		EXPECT_TRUE(c == e);
		EXPECT_TRUE(c != f);
		EXPECT_TRUE(c == g);
		EXPECT_TRUE(d != e);
		EXPECT_TRUE(d != f);
		EXPECT_TRUE(d != g);
		EXPECT_TRUE(e != f);
		EXPECT_TRUE(e == g);
		EXPECT_TRUE(f != g);
	}
}

GTEST_TEST(SerializationTest, SharedPtrTest)
{
	SharedPtrTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	SharedPtrTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	SharedPtrTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	SharedPtrTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	SharedPtrTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace shared_ptr_test

}	// namespace serialization_test
