/**
 *	@file	pointer_loader.hpp
 *
 *	@brief	pointer_loader の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_pointer_loader_HPP
#define SERIALIZATION_pointer_loader_HPP

#include <serialization/binary_iarchive.hpp>
#include <serialization/json_iarchive.hpp>
#include <serialization/text_iarchive.hpp>
#include <serialization/xml_iarchive.hpp>
#include <map>
#include <string>
#include <functional>
#include <typeindex>

namespace serialization
{

namespace detail
{

class pointer_loader
{
public:
	static pointer_loader& get_instance()
	{
		static pointer_loader s_instance;
		return s_instance;
	}

private:
	template <typename Archive, typename T>
	struct load_func
	{
		void* operator()(void* ia) const
		{
			auto p = new T();
			*(static_cast<Archive*>(ia)) >> make_nvp("value", *p);
			return p;
		}
	};

public:
	template <typename T>
	void register_class(void)
	{
		auto name = T::static_class_name();
		m_f[typeid(text_iarchive)][name] = load_func<text_iarchive, T>{};
		m_f[typeid(binary_iarchive)][name] = load_func<binary_iarchive, T>{};
		m_f[typeid(json_iarchive)][name] = load_func<json_iarchive, T>{};
	}

	template <typename T, typename Archive>
	T* load_ptr(Archive& ia, std::string const& name)
	{
		auto& m = m_f[typeid(Archive)];
		if (m.find(name) != m.end())
		{
			return (T*)m[name](&ia);
		}

		if constexpr (std::is_abstract_v<T>)
		{
			return nullptr;
		}
		else
		{
			return (T*)load_func<Archive, T>{}(&ia);
		}
	}

private:
	pointer_loader()
	{
	}

	std::map<std::type_index, 
		std::map<std::string, std::function<void*(void*)>>> m_f;
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_pointer_loader_HPP
