/**
 *	@file	base_object.cpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#include <serialization.hpp>
#include <gtest/gtest.h>
#include <sstream>
#include "serialization_test_utility.hpp"

namespace serialization_test
{

namespace base_object_test
{

class Base
{
public:
	virtual ~Base(){}

private:
	int a = get_random_value<int>();

private:
	friend bool operator==(Base const& lhs, Base const& rhs)
	{
		return lhs.a == rhs.a;
	}

	friend bool operator!=(Base const& lhs, Base const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Base& o, serialization::version_t version)
	{
		EXPECT_EQ(0u, version);
		ar & o.a;
	}
};

class Derived1 : public Base
{
public:
private:
	float b = get_random_value<float>();

private:
	friend bool operator==(Derived1 const& lhs, Derived1 const& rhs)
	{
		return
			(Base&)lhs == (Base&)rhs &&
			lhs.b == rhs.b;
	}

	friend bool operator!=(Derived1 const& lhs, Derived1 const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Derived1& o, serialization::version_t version)
	{
		EXPECT_EQ(17u, version);
		ar & serialization::base_object<Base>(o);
		ar & o.b;
	}

	friend constexpr serialization::version_t get_version(Derived1 const&)
	{
		return 17;
	}
};

class Derived2 : public Base	// TODO private継承に対応
{
public:
private:
	unsigned char b = get_random_value<unsigned char>();
	long long     c = get_random_value<long long>();

private:
	friend bool operator==(Derived2 const& lhs, Derived2 const& rhs)
	{
		return
			(Base&)lhs == (Base&)rhs &&
			lhs.b == rhs.b &&
			lhs.c == rhs.c;
	}

	friend bool operator!=(Derived2 const& lhs, Derived2 const& rhs)
	{
		return !(lhs == rhs);
	}

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Derived2& o, serialization::version_t version)
	{
		EXPECT_EQ(42u, version);
		ar & serialization::base_object<Base>(o);
		ar & o.b & o.c;
	}

	friend constexpr serialization::version_t get_version(Derived2 const&)
	{
		return 42;
	}
};

template <typename Stream, typename OArchive, typename IArchive>
void BaseObjectTest()
{
	Base     t1;
	Derived1 t2;
	Derived2 t3;
	Stream str;
	{
		OArchive oa(str);
		oa << t1;
		oa << t2;
		oa << t3;
	}
	{
		Base     tmp1;
		Derived1 tmp2;
		Derived2 tmp3;
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

GTEST_TEST(SerializationTest, BaseObjectTest)
{
	BaseObjectTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	BaseObjectTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	BaseObjectTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	BaseObjectTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	BaseObjectTest<
		std::stringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
	BaseObjectTest<
		std::wstringstream,
		serialization::json_oarchive,
		serialization::json_iarchive
	>();
}

}	// namespace base_object_test

}	// namespace serialization_test
