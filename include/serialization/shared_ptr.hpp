/**
 *	@file	shared_ptr.hpp
 *
 *	@brief	shared_ptr の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SHARED_PTR_HPP
#define SERIALIZATION_SHARED_PTR_HPP

#include <serialization/nvp.hpp>
#include <serialization/detail/get_class_name.hpp>
#include <serialization/detail/pointer_saver.hpp>
#include <serialization/detail/pointer_loader.hpp>
#include <memory>

namespace serialization
{

template <typename Archive, typename T>
void save(Archive& oa, std::shared_ptr<T> const& t)
{
	auto const has_value = bool(t);
	oa << make_nvp("has_value", has_value);
	if (has_value)
	{
		int index = oa.get_shared_ptr_index(t);
		oa << make_nvp("shared_index", index);
		if (index < 0)
		{
			std::string s = detail::get_class_name(*t);
			oa << make_nvp("class_id", s);
			detail::pointer_saver<Archive>::get_instance().save_ptr(oa, s, t.get());
			oa.register_shared_ptr(t);
		}
	}
}

template <typename Archive, typename T>
void load(Archive& ia, std::shared_ptr<T>& t)
{
	bool has_value;
	ia >> make_nvp("has_value", has_value);
	if (has_value)
	{
		int index;
		ia >> make_nvp("shared_index", index);
		if (index < 0)
		{
			std::string s;
			ia >> make_nvp("class_id", s);
			auto p = detail::pointer_loader<Archive>::get_instance().load_ptr<T>(ia, s);
			t.reset(p);
			ia.register_shared_ptr(t);
		}
		else
		{
			t = ia.get_shared_ptr<T>(index);
		}
	}
}

template <typename Archive, typename T>
void save(Archive& oa, std::shared_ptr<T[]> const& t) = delete;

template <typename Archive, typename T>
void load(Archive& ia, std::shared_ptr<T[]>& t) = delete;

}	// namespace serialization

#endif // SERIALIZATION_SHARED_PTR_HPP
