/**
 *	@file	pointer_loader.hpp
 *
 *	@brief	pointer_loader の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_pointer_loader_HPP
#define SERIALIZATION_pointer_loader_HPP

#include <map>
#include <string>
#include <functional>

namespace serialization
{

namespace detail
{

template <typename Archive>
class pointer_loader
{
public:
	static pointer_loader& get_instance()
	{
		static pointer_loader s_instance;
		return s_instance;
	}

private:
	template <typename T>
	struct load_func
	{
		void* operator()(Archive& ia) const
		{
			auto p = new T();
			ia >> make_nvp("value", *p);
			return p;
		}
	};

public:
	template <typename T>
	void register_class(void)
	{
		auto name = T::static_class_name();
		m_f[name] = load_func<T>{};
	}

	template <typename T>
	T* load_ptr(Archive& ia, std::string const& name)
	{
		if (m_f.find(name) != m_f.end())
		{
			return (T*)m_f[name](ia);
		}

		if constexpr (std::is_abstract_v<T>)
		{
			return nullptr;
		}
		else
		{
			return (T*)load_func<T>{}(ia);
		}
	}

private:
	pointer_loader()
	{
	}

	std::map<std::string, std::function<void*(Archive&)>> m_f;
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_pointer_loader_HPP
