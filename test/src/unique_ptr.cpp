/**
 *	@file	unique_ptr.cpp
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

namespace unique_ptr_test
{

class Object
{
public:
	bool  a = get_random_value<bool>();
	char  b = get_random_value<char>();
	short c = get_random_value<short>();
	std::string d;

private:
	friend bool operator==(Object const& lhs, Object const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			lhs.d == rhs.d &&
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
void UniquePtrTest()
{
	std::unique_ptr<int> p1;
	std::unique_ptr<int> p2(new int(42));
	std::unique_ptr<Object> p3(new Object());
	p3->d = "Foo Bar";
	std::unique_ptr<Object> p4(std::move(p3));

	Stream str;
	{
		OArchive oa(str);
		oa << p1 << p2 << p3 << p4;
	}
	{
		std::unique_ptr<int>  a;
		std::unique_ptr<int>  b;
		std::unique_ptr<Object>  c;
		std::unique_ptr<Object>  d;

		IArchive ia(str);

		ia >> a >> b >> c >> d;

		EXPECT_EQ(p1, a);
		EXPECT_EQ(nullptr, a);

		EXPECT_NE(p2, b);
		EXPECT_EQ(*p2, *b);

		EXPECT_EQ(p3, c);
		EXPECT_EQ(nullptr, c);

		EXPECT_NE(p4, d);
		EXPECT_EQ(*p4, *d);
	}
}

GTEST_TEST(SerializationTest, UniquePtrTest)
{
	UniquePtrTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	UniquePtrTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	UniquePtrTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	UniquePtrTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	UniquePtrTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	UniquePtrTest<
		std::wstringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace unique_ptr_test

}	// namespace serialization_test
