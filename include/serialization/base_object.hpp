/**
 *	@file	base_object.hpp
 *
 *	@brief	base_object の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_BASE_OBJECT_HPP
#define SERIALIZATION_BASE_OBJECT_HPP

#include <type_traits>

namespace serialization
{

template <typename Base, typename Derived,
	typename = std::enable_if_t<
		std::is_base_of_v<Base, Derived>
	>
>
Base& base_object(Derived& d)
{
	// TODO access::up_cast<Base&>(d);
	return static_cast<Base&>(d);
}

}	// namespace serialization

#endif // SERIALIZATION_BASE_OBJECT_HPP
