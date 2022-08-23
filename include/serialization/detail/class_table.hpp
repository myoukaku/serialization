/**
 *	@file	class_table.hpp
 *
 *	@brief	class_table の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_CLASS_TABLE_HPP
#define SERIALIZATION_CLASS_TABLE_HPP

#include <map>
#include <string>
#include <functional>

namespace serialization
{

namespace detail
{

class class_table
{
public:
	static class_table& get_instance()
	{
		static class_table s_instance;
		return s_instance;
	}

	template <typename F>
	void export_(const char* name, F f)
	{
		m_f[name] = f;
	}

	template <typename T>
	T* construct(std::string const& name)
	{
		if (m_f.find(name) != m_f.end())
		{
			return (T*)m_f[name]();
		}
		return new T();
	}

private:
	class_table()
	{
	}

	std::map<std::string, std::function<void*(void)>> m_f;
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_CLASS_TABLE_HPP
