/**
 *	@file	object_version.cpp
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

namespace object_version_test
{

// get_version のオーバーロードなし
class Object1
{
public:
	Object1() {}
	~Object1() {}

private:
	unsigned char a = get_random_value<unsigned char>();
	float         b = get_random_value<float>();
	int           c = get_random_value<int>();

private:
	friend bool operator==(Object1 const& lhs, Object1 const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			true;
	}

	friend bool operator!=(Object1 const& lhs, Object1 const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Object1& o, serialization::version_t version)
	{
		EXPECT_EQ(0u, version);
		ar	& o.a
			& o.b
			& o.c;
	}
};

// 非メンバ関数の get_version
class Object2
{
public:
	Object2() {}
	~Object2() {}

private:
	unsigned char a = get_random_value<unsigned char>();
	float         b = get_random_value<float>();
	int           c = get_random_value<int>();

private:
	friend bool operator==(Object2 const& lhs, Object2 const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			true;
	}

	friend bool operator!=(Object2 const& lhs, Object2 const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Object2& o, serialization::version_t version)
	{
		EXPECT_EQ(1u, version);
		ar	& o.a
			& o.b
			& o.c;
	}
};

inline constexpr serialization::version_t get_version(Object2 const&)
{
	return 1;
}

// Hidden friends の get_version
class Object3
{
public:
	Object3() {}
	~Object3() {}

private:
	unsigned char a = get_random_value<unsigned char>();
	float         b = get_random_value<float>();
	int           c = get_random_value<int>();

private:
	friend bool operator==(Object3 const& lhs, Object3 const& rhs)
	{
		return
			lhs.a == rhs.a &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c &&
			true;
	}

	friend bool operator!=(Object3 const& lhs, Object3 const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Object3& o, serialization::version_t version)
	{
		EXPECT_EQ(2u, version);
		ar	& o.a
			& o.b
			& o.c;
	}

	friend constexpr serialization::version_t get_version(Object3 const&)
	{
		return 2;
	}
};

template <typename Stream, typename OArchive, typename IArchive>
void ObjectVersionTest2()
{
	Object1 t1;
	Object2 t2;
	Object3 t3;
	Stream str;
	{
		OArchive oa(str);
		oa << t1;
		oa << t2;
		oa << t3;
	}
	{
		Object1 tmp1;
		Object2 tmp2;
		Object3 tmp3;
		EXPECT_NE(t1, tmp1);
		EXPECT_NE(t2, tmp2);
		EXPECT_NE(t3, tmp3);

		IArchive ia(str);
		ia >> tmp1;
		ia >> tmp2;
		ia >> tmp3;

		EXPECT_EQ(t1, tmp1);
		EXPECT_EQ(t2, tmp2);
		EXPECT_EQ(t3, tmp3);
	}
}

template <typename OArchive, typename IArchive>
void ObjectVersionTest()
{
	ObjectVersionTest2<std::stringstream,  OArchive, IArchive>();
	ObjectVersionTest2<std::wstringstream, OArchive, IArchive>();
}

GTEST_TEST(SerializationTest, ObjectVersionTest)
{
	ObjectVersionTest<
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	ObjectVersionTest<
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace object_version_test

}	// namespace serialization_test
