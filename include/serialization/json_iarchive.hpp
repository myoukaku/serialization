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
		drop();	// "{"
	}

	template <typename T>
	json_iarchive& operator>>(nvp<T> const& t)
	{
		if (!m_first)
		{
			drop();	// ","
		}
		m_first = false;

		std::string name;
		detail::load_dispatch::invoke(*this, name);
		drop();		// ":"
		detail::load_dispatch::invoke(*this, t.value());
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
	friend void start_object(json_iarchive& oa)
	{
		oa.drop();	// "{"
		oa.m_first = true;
	}

	friend void end_object(json_iarchive& oa)
	{
		oa.drop();	// "}"
	}

private:
	template <typename T>
	friend void load_array(json_iarchive& ia, T& t)
	{
		ia.drop();	// "["
		for (std::size_t i = 0; i < std::extent_v<T>; ++i)
		{
			if (i != 0)
			{
				ia.drop();	// ","
			}
			detail::load_dispatch::invoke(ia, t[i]);
		}
		ia.drop();	// "]"
	}

private:
	bool m_first = true;
};

}	// namespace serialization

#endif // SERIALIZATION_JSON_IARCHIVE_HPP
