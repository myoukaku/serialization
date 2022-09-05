/**
 *	@file	xml_iarchive.hpp
 *
 *	@brief	xml_iarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_XML_IARCHIVE_HPP
#define SERIALIZATION_XML_IARCHIVE_HPP

#include <serialization/text_iarchive.hpp>

namespace serialization
{

class xml_iarchive : public text_iarchive
{
public:
	template <typename IStream>
	explicit xml_iarchive(IStream& is)
		: text_iarchive(is)
	{
		auto s = input_until('>');
		input_start_tag();
	}

	template <typename T>
	xml_iarchive& operator>>(nvp<T> const& t)
	{
		input_start_tag();
		detail::load_dispatch::invoke(*this, t.value());
		input_end_tag();
		return *this;
	}

	template <typename T>
	xml_iarchive& operator>>(T& t)
	{
		return *this >> make_nvp("", t);
	}

	template <typename T>
	xml_iarchive& operator&(T& t)
	{
		return *this >> t;
	}

private:
	std::string input_until(char c)
	{
		std::string result;
		for (;;)
		{
			auto t = input_one_char();
			if (t == c)
			{
				return result;
			}
			result += t;
		}
	}

	void input_start_tag()
	{
		auto s = input_until('>');
	}

	void input_end_tag()
	{
		auto s = input_until('>');
	}

private:
	//friend void start_object(xml_iarchive& oa)
	//{
	//	oa.drop();	// "{"
	//	oa.m_first = true;
	//}

	//friend void end_object(xml_iarchive& oa)
	//{
	//	oa.drop();	// "}"
	//}

private:
	//template <typename T>
	//friend void load_array(xml_iarchive& ia, T& t)
	//{
	//	ia.drop();	// "["
	//	for (std::size_t i = 0; i < std::extent_v<T>; ++i)
	//	{
	//		if (i != 0)
	//		{
	//			ia.drop();	// ","
	//		}
	//		detail::load_dispatch::invoke(ia, t[i]);
	//	}
	//	ia.drop();	// "]"
	//}

private:
	bool m_first = true;
};

}	// namespace serialization

#endif // SERIALIZATION_XML_IARCHIVE_HPP
