/**
 *	@file	object_non_intrusive.cpp
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

namespace object_non_intrusive_test
{

class Object
{
public:
	enum class Enum { V1, V2, V3 };

	Object() {}
	~Object() {}

public:
	bool  a = get_random_value<bool>();
	char  b = get_random_value<char>();
	short c = get_random_value<short>();
	int   d = get_random_value<int>();
	double e[2][3] =
	{
		{
			get_random_value<double>(),
			get_random_value<double>(),
			get_random_value<double>(),
		},
		{
			get_random_value<double>(),
			get_random_value<double>(),
			get_random_value<double>(),
		},
	};
	Enum f = Enum(get_random_value<unsigned int>() % 3);

private:
	friend bool operator==(Object const& lhs, Object const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			lhs.d == rhs.d &&
			lhs.e[0][0] == rhs.e[0][0] &&
			lhs.e[0][1] == rhs.e[0][1] &&
			lhs.e[0][2] == rhs.e[0][2] &&
			lhs.e[1][0] == rhs.e[1][0] &&
			lhs.e[1][1] == rhs.e[1][1] &&
			lhs.e[1][2] == rhs.e[1][2] &&
			lhs.f == rhs.f &&
			true;
	}

	friend bool operator!=(Object const& lhs, Object const& rhs)
	{
		return !(lhs == rhs);
	}
};

template <typename Archive>
void serialize(Archive& ar, Object& o)
{
	ar	& o.a
		& o.b
		& o.c
		& o.d
		& o.e
		& o.f;
}

template <typename Stream, typename OArchive, typename IArchive>
void ObjectNonIntrusiveTest2()
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
void ObjectNonIntrusiveTest()
{
	ObjectNonIntrusiveTest2<std::stringstream,  OArchive, IArchive>();
	ObjectNonIntrusiveTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, ObjectNonIntrusiveTest)
{
	ObjectNonIntrusiveTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	ObjectNonIntrusiveTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	ObjectNonIntrusiveTest<
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace object_non_intrusive_test

}	// namespace serialization_test
