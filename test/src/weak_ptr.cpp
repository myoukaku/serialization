/**
 *	@file	weak_ptr.cpp
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

namespace weak_ptr_test
{

template <typename Stream, typename OArchive, typename IArchive>
void WeakPtrTest()
{
	Stream str;
	{
		std::weak_ptr<int>   p1;
		std::shared_ptr<int> p2(new int(10));
		std::weak_ptr<int>   p3(p2);
		std::weak_ptr<int>   p4;
		std::shared_ptr<int> p5(new int(20));
		std::weak_ptr<int>   p6(p5);
		p4 = p5;

		OArchive oa(str);
		oa << p1 << p2 << p3 << p4 << p5 << p6;
	}
	{
		std::weak_ptr<int>   a;
		std::shared_ptr<int> b;
		std::weak_ptr<int>   c;
		std::weak_ptr<int>   d;
		std::shared_ptr<int> e;
		std::weak_ptr<int>   f;

		IArchive ia(str);

		ia >> a >> b >> c >> d >> e >> f;

		EXPECT_TRUE(a.expired());

		EXPECT_EQ(10, *b);

		EXPECT_TRUE(!c.expired());
		EXPECT_EQ(10, *(c.lock()));
		EXPECT_TRUE(b == c.lock());

		EXPECT_TRUE(!d.expired());
		EXPECT_EQ(20, *(d.lock()));
		EXPECT_TRUE(e == d.lock());

		EXPECT_EQ(20, *e);

		EXPECT_TRUE(!f.expired());
		EXPECT_EQ(20, *(f.lock()));
		EXPECT_TRUE(e == f.lock());
	}
}

GTEST_TEST(SerializationTest, WeakPtrTest)
{
	WeakPtrTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	WeakPtrTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	WeakPtrTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	WeakPtrTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	WeakPtrTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	WeakPtrTest<
		std::wstringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace weak_ptr_test

}	// namespace serialization_test
