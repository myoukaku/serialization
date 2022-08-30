/**
 *	@file	shared_ptr_table.hpp
 *
 *	@brief	shared_ptr_table の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SHARED_PTR_TABLE_HPP
#define SERIALIZATION_SHARED_PTR_TABLE_HPP

#include <memory>
#include <vector>

namespace serialization
{

namespace detail
{

class shared_ptr_table
{
public:
	template <typename T>
	int get_shared_ptr_index(std::shared_ptr<T> const& ptr) const
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

	template <typename T>
	void register_shared_ptr(std::shared_ptr<T> const& ptr)
	{
		m_list.push_back(ptr);
	}

	template <typename T>
	std::shared_ptr<T> get_shared_ptr(int index)
	{
		return std::static_pointer_cast<T>(m_list[index]);
	}

private:
	std::vector<std::shared_ptr<void>>	m_list;
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_SHARED_PTR_TABLE_HPP
