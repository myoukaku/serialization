/**
 *	@file	pointer_saver.hpp
 *
 *	@brief	pointer_saver の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_POINTER_SAVER_HPP
#define SERIALIZATION_POINTER_SAVER_HPP

#include <serialization/binary_oarchive.hpp>
#include <serialization/json_oarchive.hpp>
#include <serialization/text_oarchive.hpp>
#include <serialization/xml_oarchive.hpp>
#include <map>
#include <string>
#include <functional>
#include <typeindex>

namespace serialization
{

namespace detail
{

class pointer_saver
{
public:
	static pointer_saver& get_instance()
	{
		static pointer_saver s_instance;
		return s_instance;
	}

private:
	template <typename Archive, typename T>
	struct save_func
	{
		void operator()(void* oa, void const* p) const
		{
			*(static_cast<Archive*>(oa)) << make_nvp("value", *(static_cast<T const*>(p)));
		}
	};

public:
	template <typename T>
	void register_class(void)
	{
		auto name = T::static_class_name();
		m_f[typeid(text_oarchive)][name] = save_func<text_oarchive, T>{};
		m_f[typeid(binary_oarchive)][name] = save_func<binary_oarchive, T>{};
		m_f[typeid(json_oarchive)][name] = save_func<json_oarchive, T>{};
	}

	template <typename Archive, typename T>
	void save_ptr(Archive& oa, std::string const& name, T const* p)
	{
		auto& m = m_f[typeid(Archive)];
		if (m.find(name) != m.end())
		{
			m[name](&oa, p);
		}
		else
		{
			save_func<Archive, T>{}(&oa, p);
		}
	}

private:
	pointer_saver()
	{
	}

	std::map<std::type_index, 
		std::map<std::string, std::function<void(void*, void const*)>>> m_f;
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_POINTER_SAVER_HPP
