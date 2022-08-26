﻿/**
 *	@file	export.hpp
 *
 *	@brief	export の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_EXPORT_HPP
#define SERIALIZATION_EXPORT_HPP

#include <serialization/detail/pointer_loader.hpp>
#include <serialization/detail/pointer_saver.hpp>

namespace serialization
{

template <typename T, typename OArchive, typename IArchive>
struct class_exporter
{
	class_exporter()
	{
		serialization::detail::pointer_saver<OArchive>::get_instance().register_class<T>();
		serialization::detail::pointer_loader<IArchive>::get_instance().register_class<T>();
	}
};

}	// namespace serialization

#endif // SERIALIZATION_EXPORT_HPP
