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
#include <serialization/detail/pointer_saver.hpp>
#include <serialization/detail/pointer_loader.hpp>
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
		detail::pointer_saver<Archive>::get_instance().save_ptr<T>(oa, s, t.get());
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

		auto p = detail::pointer_loader<Archive>::get_instance().load_ptr<T>(ia, s);
		t.reset(p);
	}
}

}	// namespace serialization

#endif // SERIALIZATION_UNIQUE_PTR_HPP
