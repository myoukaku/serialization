/**
 *	@file	json_iarchive.hpp
 *
 *	@brief	json_iarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_JSON_IARCHIVE_HPP
#define SERIALIZATION_JSON_IARCHIVE_HPP

#include <serialization/text_iarchive.hpp>

namespace serialization
{

class json_iarchive : public text_iarchive
{
public:
	template <typename IStream>
	explicit json_iarchive(IStream& is)
		: text_iarchive(is)
	{
		std::string s;
		text_iarchive::input(s);	// "{"
	}

	template <typename T>
	json_iarchive& operator>>(nvp<T> const& t)
	{
		if (!m_first)
		{
			std::string s;
			text_iarchive::input(s);	// ","
		}
		m_first = false;

		std::string name;
		*((text_iarchive*)this) >> name;
		std::string s;
		text_iarchive::input(s);	// ": "
		*((text_iarchive*)this) >> t.value();
		return *this;
	}

	template <typename T>
	json_iarchive& operator>>(T& t)
	{
		return *this >> make_nvp("", t);
	}

	template <typename T>
	json_iarchive& operator&(T& t)
	{
		return *this >> t;
	}

private:
	bool m_first = true;
};

}	// namespace serialization

#endif // SERIALIZATION_JSON_IARCHIVE_HPP
