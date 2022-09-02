/**
 *	@file	json_oarchive.hpp
 *
 *	@brief	json_oarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_JSON_OARCHIVE_HPP
#define SERIALIZATION_JSON_OARCHIVE_HPP

#include <serialization/text_oarchive.hpp>
#include <sstream>

namespace serialization
{

class json_oarchive : public text_oarchive
{
public:
	template <typename OStream>
	explicit json_oarchive(OStream& os)
		: text_oarchive(os)
	{
		start_object();
	}

	~json_oarchive()
	{
		end_object();
	}

	template <typename T>
	json_oarchive& operator<<(nvp<T> const& t)
	{
		if (!m_first)
		{
			output(",\n");
		}
		m_first = false;

		output(get_indent_string());
		detail::save_dispatch::invoke(*this, t.name());
		output(": ");
		detail::save_dispatch::invoke(*this, t.value());
		return *this;
	}

	template <typename T>
	json_oarchive& operator<<(T const& t)
	{
		std::stringstream ss;
		ss << "value" << m_value_index;
		m_value_index++;
		return *this << make_nvp(ss.str().c_str(), t);
	}

	template <typename T>
	json_oarchive& operator&(T const& t)
	{
		return *this << t;
	}

private:
	void start_object()
	{
		output("{\n");
		m_indent++;
		m_first = true;
	}

	void end_object()
	{
		output("\n");
		m_indent--;
		output(get_indent_string());
		output("}");
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
	friend void start_object(json_oarchive& oa)
	{
		oa.start_object();
	}

	friend void end_object(json_oarchive& oa)
	{
		oa.end_object();
	}

	template <typename T>
	friend void save_array(json_oarchive& oa, T const& t)
	{
		oa.output("[\n");
		oa.m_indent++;
		for (std::size_t i = 0; i < std::extent_v<T>; ++i)
		{
			if (i != 0)
			{
				oa.output(",\n");
			}
			oa.output(oa.get_indent_string());
			detail::save_dispatch::invoke(oa, t[i]);
		}
		oa.output("\n");
		oa.m_indent--;
		oa.output(oa.get_indent_string());
		oa.output("]");
	}

private:
	int m_indent = 0;
	int m_value_index = 0;
	bool m_first = true;
};


}	// namespace serialization

#endif // SERIALIZATION_JSON_OARCHIVE_HPP
