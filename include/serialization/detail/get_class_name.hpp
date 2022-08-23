/**
 *	@file	get_class_name.hpp
 *
 *	@brief	get_class_name の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_GET_CLASS_NAME_HPP
#define SERIALIZATION_GET_CLASS_NAME_HPP

namespace serialization
{

namespace detail
{

template <typename T>
concept has_member_get_class_name = requires(T const& t) { t.get_class_name(); };

template <typename T>
inline const char* get_class_name(T const& t)
{
	if constexpr (has_member_get_class_name<T>)
	{
		return t.get_class_name();
	}
	return "";
}

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_GET_CLASS_NAME_HPP
