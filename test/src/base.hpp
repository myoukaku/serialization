/**
 *	@file	base.hpp
 *
 *	@brief
 *
 *	@author	myoukaku
 */

#ifndef BASE_HPP
#define BASE_HPP

#include <serialization.hpp>
#include "serialization_test_utility.hpp"

namespace serialization_test
{

class Base
{
public:
	Base() { ++Base::instance_count; }
	virtual ~Base() { --Base::instance_count; }

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
	static const char* static_class_name() { return "serialization_test::Base"; }
	virtual const char* get_class_name() const { return static_class_name(); }

public:
	static inline int instance_count = 0;
};

}	// namespace serialization_test

#endif // BASE_HPP
