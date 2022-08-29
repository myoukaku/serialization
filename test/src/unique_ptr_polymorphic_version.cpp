/**
 *	@file	unique_ptr_polymorphic_version.cpp
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

namespace unique_ptr_polymorphic_version_test
{

class Base
{
public:
	virtual ~Base() {}

	virtual float GetValue() const { return a; }

private:
	float a = get_random_value<float>(0.0f, 10.0f);

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Base& o, serialization::version_t version)
	{
		EXPECT_EQ(3u, version);
		ar & o.a;
	}

	friend constexpr serialization::version_t get_version(Base const&)
	{
		return 3;
	}

public:
	static const char* static_class_name() { return "unique_ptr_polymorphic_version_test::Base"; }
	virtual const char* get_class_name() const { return static_class_name(); }
};

class Derived : public Base
{
public:
	float GetValue() const override
	{
		return Base::GetValue() + b;
	}

private:
	int b = get_random_value<int>(-1000, 1000);

private:
	template <typename Archive>
	friend void serialize(Archive& ar, Derived& o, serialization::version_t version)
	{
		EXPECT_EQ(2u, version);
		ar & serialization::base_object<Base>(o);
		ar & o.b;
	}

	friend constexpr serialization::version_t get_version(Derived const&)
	{
		return 2;
	}

public:
	static const char* static_class_name() { return "unique_ptr_polymorphic_version_test::Derived"; }
	virtual const char* get_class_name() const { return static_class_name(); }
};

static const serialization::class_exporter<
	Derived, serialization::text_oarchive, serialization::text_iarchive
> dummy1{};

static const serialization::class_exporter<
	Derived, serialization::binary_oarchive, serialization::binary_iarchive
> dummy2{};

template <typename Stream, typename OArchive, typename IArchive>
void UniquePtrPolymorphicVersionTest()
{
	std::unique_ptr<Base>    p1(new Base());
	std::unique_ptr<Base>    p2(new Derived());
	std::unique_ptr<Derived> p3(new Derived());

	Stream str;
	{
		OArchive oa(str);
		oa << p1 << p2 << p3;
	}
	{
		std::unique_ptr<Base>    a;
		std::unique_ptr<Base>    b;
		std::unique_ptr<Derived> c;

		IArchive ia(str);

		ia >> a >> b >> c;

		EXPECT_EQ(p1->GetValue(), a->GetValue());
		EXPECT_EQ(p2->GetValue(), b->GetValue());
		EXPECT_EQ(p3->GetValue(), c->GetValue());
	}
}

GTEST_TEST(SerializationTest, UniquePtrPolymorphicVersionTest)
{
	UniquePtrPolymorphicVersionTest<
		std::stringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	UniquePtrPolymorphicVersionTest<
		std::wstringstream,
		serialization::text_oarchive,
		serialization::text_iarchive
	>();
	UniquePtrPolymorphicVersionTest<
		std::stringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
	//UniquePtrPolymorphicVersionTest<
	//	std::wstringstream,
	//	serialization::binary_oarchive,
	//	serialization::binary_iarchive
	//>();
}

}	// namespace unique_ptr_polymorphic_version_test

}	// namespace serialization_test
