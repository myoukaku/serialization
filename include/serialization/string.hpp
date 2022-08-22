/**
 *	@file	string.hpp
 *
 *	@brief	string の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_STRING_HPP
#define SERIALIZATION_STRING_HPP

#include <string>

namespace serialization
{

template <typename Archive, typename CharT>
void save_object(Archive& oa, std::basic_string<CharT> const& t)
{
	save_string(oa, t);
}

template <typename Archive, typename CharT>
void load_object(Archive& ia, std::basic_string<CharT>& t)
{
	load_string(ia, t);
}

}	// namespace serialization

#endif // SERIALIZATION_STRING_HPP
