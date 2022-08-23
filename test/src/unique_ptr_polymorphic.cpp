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
#include "serialization_test_utility.hpp"

namespace serialization_test
{

namespace unique_ptr_polymorphic_test
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
	friend void serialize(Archive& ar, Base& o)
	{
		ar & o.a;
	}

public:
    friend const char* static_class_name(Base const*) { return "Base"; }
    virtual const char* get_class_name() const { return static_class_name(this); }
    inline static serialization::detail::class_exporter<Base> reg;
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
	friend void serialize(Archive& ar, Derived& o)
	{
		ar & serialization::base_object<Base>(o);
		ar & o.b;
	}

public:
    friend const char* static_class_name(Derived const*) { return "Derived"; }
    virtual const char* get_class_name() const { return static_class_name(this); }
    inline static serialization::detail::class_exporter<Derived> reg;
};


template <typename Stream, typename OArchive, typename IArchive>
void UniquePtrPolymorphicTest()
{
	std::unique_ptr<Base>    p2(new Derived());

	Stream str;
	{
		OArchive oa(str);
		oa << p2;
	}
	{
		std::unique_ptr<Base>    b;

		IArchive ia(str);

		ia >> b;

		EXPECT_EQ(p2->GetValue(), b->GetValue());
	}
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
	UniquePtrPolymorphicTest<
		std::wstringstream,
		serialization::binary_oarchive,
		serialization::binary_iarchive
	>();
}

}	// namespace unique_ptr_polymorphic_test

}	// namespace serialization_test
