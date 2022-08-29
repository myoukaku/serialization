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

namespace detail
{

template <typename Archive, typename T>
class shared_ptr_table
{
public:
	static shared_ptr_table& get_instance()
	{
		static shared_ptr_table instance;
		return instance;
	}

	int get_index(std::shared_ptr<T> const& ptr) const
	{
		for (int i = 0; i < m_list.size(); ++i)
		{
			if (m_list[i] == ptr)
			{
				return i;
			}
		}
		return -1;
	}

	void register_(std::shared_ptr<T> const& ptr)
	{
		m_list.push_back(ptr);
	}

	std::shared_ptr<T> get(int index)
	{
		return m_list[index];
	}

private:
	shared_ptr_table() {}

	std::vector<std::shared_ptr<T>>	m_list;
};

}	// namespace detail

template <typename Archive, typename T>
void save(Archive& oa, std::shared_ptr<T> const& t)
{
	auto const has_value = bool(t);
	oa << make_nvp("has_value", has_value);
	if (has_value)
	{
		int index = detail::shared_ptr_table<Archive, T>::get_instance().get_index(t);
		oa << make_nvp("shared_index", index);
		if (index < 0)
		{
			std::string s = detail::get_class_name(*t);
			oa << make_nvp("class_id", s);
			detail::pointer_saver<Archive>::get_instance().save_ptr(oa, s, t.get());
			detail::shared_ptr_table<Archive, T>::get_instance().register_(t);
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
			detail::shared_ptr_table<Archive, T>::get_instance().register_(t);
		}
		else
		{
			t = detail::shared_ptr_table<Archive, T>::get_instance().get(index);
		}
	}
}

template <typename Archive, typename T>
void save(Archive& oa, std::shared_ptr<T[]> const& t) = delete;

template <typename Archive, typename T>
void load(Archive& ia, std::shared_ptr<T[]>& t) = delete;

}	// namespace serialization

#endif // SERIALIZATION_SHARED_PTR_HPP
