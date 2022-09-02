/**
 *	@file	unique_ptr_polymorphic.cpp
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

namespace unique_ptr_polymorphic_test
{

template <typename Stream, typename OArchive, typename IArchive>
void UniquePtrPolymorphicTest()
{
	EXPECT_EQ(0, Base::instance_count);
	EXPECT_EQ(0, Derived::instance_count);

	{
		std::unique_ptr<Base>    p1(new Base());
		std::unique_ptr<Base>    p2(new Derived());
		std::unique_ptr<Derived> p3(new Derived());

		EXPECT_EQ(3, Base::instance_count);
		EXPECT_EQ(2, Derived::instance_count);

		Stream str;
		{
			OArchive oa(str);
			oa << p1 << p2 << p3;

			EXPECT_EQ(3, Base::instance_count);
			EXPECT_EQ(2, Derived::instance_count);
		}

		EXPECT_EQ(3, Base::instance_count);
		EXPECT_EQ(2, Derived::instance_count);

		{
			std::unique_ptr<Base>    a;
			std::unique_ptr<Base>    b;
			std::unique_ptr<Derived> c;

			IArchive ia(str);

			EXPECT_EQ(3, Base::instance_count);
			EXPECT_EQ(2, Derived::instance_count);

			ia >> a >> b >> c;

			EXPECT_EQ(6, Base::instance_count);
			EXPECT_EQ(4, Derived::instance_count);

			EXPECT_EQ(p1->GetValue(), a->GetValue());
			EXPECT_EQ(p2->GetValue(), b->GetValue());
			EXPECT_EQ(p3->GetValue(), c->GetValue());
			EXPECT_TRUE(p1 != a);
			EXPECT_TRUE(p2 != b);
			EXPECT_TRUE(p3 != c);
		}

		EXPECT_EQ(3, Base::instance_count);
		EXPECT_EQ(2, Derived::instance_count);
	}

	EXPECT_EQ(0, Base::instance_count);
	EXPECT_EQ(0, Derived::instance_count);
}

GTEST_TEST(SerializationTest, UniquePtrPolymorphicTest)
{
	UniquePtrPolymorphicTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	UniquePtrPolymorphicTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	UniquePtrPolymorphicTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	//UniquePtrPolymorphicTest<
	//	std::wstringstream,
	//	serialization::binary_oarchive,
	//	serialization::binary_iarchive
	//>();
	UniquePtrPolymorphicTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	UniquePtrPolymorphicTest<
		std::wstringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace unique_ptr_polymorphic_test

}	// namespace serialization_test
