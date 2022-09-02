/**
 *	@file	derived.hpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#ifndef DERIVED_HPP
#define DERIVED_HPP

#include <serialization.hpp>
#include "base.hpp"
#include "serialization_test_utility.hpp"

namespace serialization_test
{

class Derived : public Base
{
public:
	Derived() { ++Derived::instance_count; }
	~Derived() { --Derived::instance_count; }

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
	static const char* static_class_name() { return "unique_ptr_polymorphic_test::Derived"; }
	virtual const char* get_class_name() const { return static_class_name(); }

public:
	static inline int instance_count = 0;
};

static const serialization::class_exporter<
	Derived, serialization::text_oarchive, serialization::text_iarchive
> dummy1{};

static const serialization::class_exporter<
	Derived, serialization::binary_oarchive, serialization::binary_iarchive
> dummy2{};

static const serialization::class_exporter<
	Derived, serialization::json_oarchive, serialization::json_iarchive
> dummy3{};

}	// namespace serialization_test

#endif // DERIVED_HPP
