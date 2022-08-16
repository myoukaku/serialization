/**
 *	@file	object_non_intrusive_split.cpp
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

namespace object_non_intrusive_split_test
{

class Object
{
public:
	enum class Enum { V1, V2, V3, V4 };

	Object() {}
	~Object() {}

public:
	unsigned long long a[3] =
	{
		get_random_value<unsigned long long>(),
		get_random_value<unsigned long long>(),
		get_random_value<unsigned long long>(),
	};
	unsigned int   b = get_random_value<unsigned int>();
	unsigned short c = get_random_value<unsigned short>();
	unsigned char  d = get_random_value<unsigned char>();
	Enum           e = Enum(get_random_value<unsigned int>() % 4);
	float          f = get_random_value<float>();

private:
	friend bool operator==(Object const& lhs, Object const& rhs)
	{
		return
			lhs.a[0] == rhs.a[0] &&
			lhs.a[1] == rhs.a[1] &&
			lhs.a[2] == rhs.a[2] &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			lhs.d == rhs.d &&
			lhs.e == rhs.e &&
			lhs.f == rhs.f &&
			true;
	}

	friend bool operator!=(Object const& lhs, Object const& rhs)
	{
		return !(lhs == rhs);
	}
};

template <typename Archive>
void save(Archive& ar, Object const& o)
{
	ar << o.a;
	ar << o.b;
	ar << o.c;
	ar << o.d;
	ar << o.e;
	ar << o.f;
}

template <typename Archive>
void load(Archive& ar, Object& o)
{
	ar >> o.a;
	ar >> o.b;
	ar >> o.c;
	ar >> o.d;
	ar >> o.e;
	ar >> o.f;
}

template <typename Stream, typename OArchive, typename IArchive>
void ObjectNonIntrusiveSplitTest2()
{
	Object t;
	Stream str;
	{
		OArchive oa(str);
		oa << t;
	}
	{
		Object tmp;
		EXPECT_NE(t, tmp);

		IArchive ia(str);
		ia >> tmp;

		EXPECT_EQ(t, tmp);
	}
}

template <typename OArchive, typename IArchive>
void ObjectNonIntrusiveSplitTest()
{
	ObjectNonIntrusiveSplitTest2<std::stringstream,  OArchive, IArchive>();
	ObjectNonIntrusiveSplitTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, ObjectNonIntrusiveSplitTest)
{
	ObjectNonIntrusiveSplitTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	ObjectNonIntrusiveSplitTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace object_non_intrusive_split_test

}	// namespace serialization_test
