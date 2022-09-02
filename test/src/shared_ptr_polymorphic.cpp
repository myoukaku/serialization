/**
 *	@file	shared_ptr_polymorphic.cpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#include <serialization.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include <memory>
#include "base.hpp"
#include "derived.hpp"

namespace serialization_test
{

namespace shared_ptr_polymorphic_test
{

template <typename Stream, typename OArchive, typename IArchive>
void SharedPtrPolymorphicTest()
{
	EXPECT_EQ(0, Base::instance_count);
	EXPECT_EQ(0, Derived::instance_count);

	{
		std::shared_ptr<Base>    p1(new Base());
		std::shared_ptr<Base>    p2(new Derived());
		std::shared_ptr<Derived> p3(new Derived());
		std::shared_ptr<Base>    p4(p2);
		std::shared_ptr<Base>    p5(p3);
		std::shared_ptr<Derived> p6(p3);

		EXPECT_EQ(3, Base::instance_count);
		EXPECT_EQ(2, Derived::instance_count);

		Stream str;
		{
			OArchive oa(str);
			oa << p1 << p2 << p3 << p4 << p5 << p6;

			EXPECT_EQ(3, Base::instance_count);
			EXPECT_EQ(2, Derived::instance_count);
		}

		EXPECT_EQ(3, Base::instance_count);
		EXPECT_EQ(2, Derived::instance_count);

		{
			std::shared_ptr<Base>    a;
			std::shared_ptr<Base>    b;
			std::shared_ptr<Derived> c;
			std::shared_ptr<Base>    d;
			std::shared_ptr<Base>    e;
			std::shared_ptr<Derived> f;

			IArchive ia(str);

			EXPECT_EQ(3, Base::instance_count);
			EXPECT_EQ(2, Derived::instance_count);

			ia >> a;
			ia >> b;
			ia >> c;
			ia >> d;
			ia >> e;
			ia >> f;

			EXPECT_EQ(6, Base::instance_count);
			EXPECT_EQ(4, Derived::instance_count);

			EXPECT_EQ(p1->GetValue(), a->GetValue());
			EXPECT_EQ(p2->GetValue(), b->GetValue());
			EXPECT_EQ(p3->GetValue(), c->GetValue());
			EXPECT_EQ(p4->GetValue(), d->GetValue());
			EXPECT_EQ(p5->GetValue(), e->GetValue());
			EXPECT_EQ(p6->GetValue(), f->GetValue());
			EXPECT_TRUE(p1 != a);
			EXPECT_TRUE(p2 != b);
			EXPECT_TRUE(p3 != c);
			EXPECT_TRUE(p4 != d);
			EXPECT_TRUE(p5 != e);
			EXPECT_TRUE(p6 != f);
			EXPECT_TRUE(b == d);
			EXPECT_TRUE(c == e);
			EXPECT_TRUE(c == f);
		}

		EXPECT_EQ(3, Base::instance_count);
		EXPECT_EQ(2, Derived::instance_count);
	}

	EXPECT_EQ(0, Base::instance_count);
	EXPECT_EQ(0, Derived::instance_count);
}

GTEST_TEST(SerializationTest, SharedPtrPolymorphicTest)
{
	SharedPtrPolymorphicTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	SharedPtrPolymorphicTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	SharedPtrPolymorphicTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	//SharedPtrPolymorphicTest<
	//	std::wstringstream,
	//	serialization::binary_oarchive,
	//	serialization::binary_iarchive
	//>();
	SharedPtrPolymorphicTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	SharedPtrPolymorphicTest<
		std::wstringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace shared_ptr_polymorphic_test

}	// namespace serialization_test
