/**
 *	@file	xml_oarchive.hpp
 *
 *	@brief	xml_oarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_XML_OARCHIVE_HPP
#define SERIALIZATION_XML_OARCHIVE_HPP

#include <serialization/text_oarchive.hpp>
#include <sstream>

namespace serialization
{

class xml_oarchive : public text_oarchive
{
public:
	template <typename OStream>
	explicit xml_oarchive(OStream& os)
		: text_oarchive(os)
	{
		output(R"(<?xml version="1.0"?>)");
		output("\n");
		start_node("serialization");
		output("\n");
	}

	~xml_oarchive()
	{
		end_node("serialization");
	}

	template <typename T>
	xml_oarchive& operator<<(nvp<T> const& t)
	{
		output(get_indent_string());
		start_node(t.name());
		detail::save_dispatch::invoke(*this, t.value());
		end_node(t.name());
		return *this;
	}

	template <typename T>
	xml_oarchive& operator<<(T const& t)
	{
		std::stringstream ss;
		ss << "value" << m_value_index;
		m_value_index++;
		return *this << make_nvp(ss.str().c_str(), t);
	}

	template <typename T>
	xml_oarchive& operator&(T const& t)
	{
		return *this << t;
	}

private:
	void start_node(std::string const& name)
	{
		output("<" + name + ">");
		m_indent++;
	}

	void end_node(std::string const& name)
	{
		m_indent--;
		output("</" + name + ">\n");
	}

	std::string get_indent_string() const
	{
		std::string s;
		for (int i = 0; i < m_indent; ++i)
		{
			s += "    ";
		}
		return s;
	}

private:
	//friend void start_object(xml_oarchive& oa)
	//{
	//	oa.start_object();
	//}

	//friend void end_object(xml_oarchive& oa)
	//{
	//	oa.end_object();
	//}

	template <typename T>
	friend void save_array(xml_oarchive& oa, T const& t)
	{
		oa.m_indent++;
		for (std::size_t i = 0; i < std::extent_v<T>; ++i)
		{
			oa << t[i];
		}
		oa.m_indent--;
	}

private:
	int m_indent = 0;
	int m_value_index = 0;
};


}	// namespace serialization

#endif // SERIALIZATION_XML_OARCHIVE_HPP
