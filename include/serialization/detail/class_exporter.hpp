/**
 *	@file	class_exporter.hpp
 *
 *	@brief	class_exporter の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_CLASS_EXPORTER_HPP
#define SERIALIZATION_CLASS_EXPORTER_HPP

#include <serialization/detail/class_table.hpp>

namespace serialization
{

namespace detail
{

template <typename T>
class class_exporter
{
public:
	class_exporter()
	{
		class_table::get_instance().export_(static_class_name((T*)nullptr), [](){ return new T(); });
	}
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_CLASS_EXPORTER_HPP
