/**
 *	@file	object_intrusive_split.cpp
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

namespace object_intrusive_split_test
{

class Object
{
public:
	enum class Enum { V1, V2, V3, V4 };

	Object() {}
	~Object() {}

private:
	int           a[2] =
	{
		get_random_value<int>(),
		get_random_value<int>(),
	};
	unsigned char b = get_random_value<unsigned char>();
	double        c = get_random_value<double>();
	long long     d = get_random_value<long long>();
	Enum          e = Enum(get_random_value<unsigned int>() % 4);
	wchar_t       f = get_random_value<wchar_t>();

private:
	friend bool operator==(Object const& lhs, Object const& rhs)
	{
		return
			lhs.a[0] == rhs.a[0] &&
			lhs.a[1] == rhs.a[1] &&
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

private:
	friend class serialization::access;

	template <typename Archive>
	void save(Archive& ar) const
	{
		ar << a;
		ar << b;
		ar << c;
		ar << d;
		ar << e;
		ar << f;
	}

	template <typename Archive>
	void load(Archive& ar)
	{
		ar >> a;
		ar >> b;
		ar >> c;
		ar >> d;
		ar >> e;
		ar >> f;
	}
};

template <typename Stream, typename OArchive, typename IArchive>
void ObjectIntrusiveSplitTest2()
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
void ObjectIntrusiveSplitTest()
{
	ObjectIntrusiveSplitTest2<std::stringstream,  OArchive, IArchive>();
	ObjectIntrusiveSplitTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, ObjectIntrusiveSplitTest)
{
	ObjectIntrusiveSplitTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	ObjectIntrusiveSplitTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace object_intrusive_split_test

}	// namespace serialization_test
