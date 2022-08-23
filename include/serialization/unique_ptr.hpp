/**
 *	@file	unique_ptr.hpp
 *
 *	@brief	unique_ptr の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_UNIQUE_PTR_HPP
#define SERIALIZATION_UNIQUE_PTR_HPP

#include <serialization/nvp.hpp>
#include <serialization/detail/get_class_name.hpp>
#include <serialization/detail/class_table.hpp>
#include <memory>

namespace serialization
{

template <typename Archive, typename T>
void save(Archive& oa, std::unique_ptr<T> const& t)
{
	auto const has_value = bool(t);
	oa << make_nvp("has_value", has_value);
	if (has_value)
	{
		std::string s = detail::get_class_name(*t);
		oa << make_nvp("class_id", s);
		oa << make_nvp("value", *t);
	}
}

template <typename Archive, typename T>
void load(Archive& ia, std::unique_ptr<T>& t)
{
	bool has_value;
	ia >> make_nvp("has_value", has_value);
	if (has_value)
	{
		std::string s;
		ia >> make_nvp("class_id", s);

		auto p = detail::class_table::get_instance().construct<T>(s);
		ia >> make_nvp("value", *p);
		t.reset(p);
	}
}

}	// namespace serialization

#endif // SERIALIZATION_UNIQUE_PTR_HPP
