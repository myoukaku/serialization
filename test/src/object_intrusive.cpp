/**
 *	@file	object_intrusive.cpp
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

namespace object_intrusive_test
{

class Object
{
public:
	enum class Enum { V1, V2, V3, V4 };

	Object() {}
	~Object() {}

private:
	char           a = get_random_value<char>();
	unsigned short b = get_random_value<unsigned short>();
	bool           c = get_random_value<bool>();
	int            d = get_random_value<int>();
	float          e[3] =
	{
		get_random_value<float>(),
		get_random_value<float>(),
		get_random_value<float>(),
	};
	Enum           f = Enum(get_random_value<unsigned int>() % 4);

private:
	friend bool operator==(Object const& lhs, Object const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			lhs.d == rhs.d &&
			lhs.e[0] == rhs.e[0] &&
			lhs.e[1] == rhs.e[1] &&
			lhs.e[2] == rhs.e[2] &&
			lhs.f == rhs.f &&
			true;
	}

	friend bool operator!=(Object const& lhs, Object const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	friend class serialization::access;

	template <typename Archive>
	void serialize(Archive& ar)
	{
		ar	& a
			& b
			& c
			& d
			& e
			& f;
	}
};

template <typename Stream, typename OArchive, typename IArchive>
void ObjectIntrusiveTest2()
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
void ObjectIntrusiveTest()
{
	ObjectIntrusiveTest2<std::stringstream,  OArchive, IArchive>();
	ObjectIntrusiveTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, ObjectIntrusiveTest)
{
	ObjectIntrusiveTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	ObjectIntrusiveTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace object_intrusive_test

}	// namespace serialization_test
