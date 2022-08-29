/**
 *	@file	pointer_saver.hpp
 *
 *	@brief	pointer_saver の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_POINTER_SAVER_HPP
#define SERIALIZATION_POINTER_SAVER_HPP

#include <map>
#include <string>
#include <functional>

namespace serialization
{

namespace detail
{

template <typename Archive>
class pointer_saver
{
public:
	static pointer_saver& get_instance()
	{
		static pointer_saver s_instance;
		return s_instance;
	}

private:
	template <typename T>
	struct save_func
	{
		void operator()(Archive& oa, void const* p) const
		{
			oa << make_nvp("value", *(static_cast<T const*>(p)));
		}
	};

public:
	template <typename T>
	void register_class(void)
	{
		auto name = T::static_class_name();
		m_f[name] = save_func<T>{};
	}

	template <typename T>
	void save_ptr(Archive& oa, std::string const& name, T const* p)
	{
		if (m_f.find(name) != m_f.end())
		{
			m_f[name](oa, p);
			return;
		}

		save_func<T>{}(oa, p);
	}

private:
	pointer_saver()
	{
	}

	std::map<std::string, std::function<void(Archive&, void const*)>> m_f;
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_POINTER_SAVER_HPP
