/**
 *	@file	text_iarchive.hpp
 *
 *	@brief	text_iarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_TEXT_IARCHIVE_HPP
#define SERIALIZATION_TEXT_IARCHIVE_HPP

#include <serialization/detail/load_dispatch.hpp>
#include <serialization/detail/shared_ptr_table.hpp>
#include <cstdint>
#include <memory>
#include <type_traits>
#include <charconv>
#include <iomanip>

#pragma warning(disable : 4244)

namespace serialization
{

class text_iarchive_impl_base
{
public:
	virtual ~text_iarchive_impl_base()
	{
	}

	virtual void load(bool&) = 0;
	virtual void load(std::intmax_t&) = 0;
	virtual void load(std::uintmax_t&) = 0;
	virtual void load(float&) = 0;
	virtual void load(double&) = 0;
	virtual void load(long double&) = 0;
	virtual void load(std::string&) = 0;
	virtual void load(std::wstring&) = 0;
	virtual void input(std::string&) = 0;
};

template <typename CharT, typename Traits>
inline void input_quoted_string(
	std::basic_istream<CharT, Traits>& is,
	std::basic_string<CharT, Traits>& s)
{
	is >> std::quoted(s);
}

inline void input_quoted_string(
	std::basic_istream<char>& is,
	std::basic_string<wchar_t>& s)
{
	char c;
	is >> c;
	for (;;)
	{
		is >> c;
		if (c == '"')
		{
			break;
		}
		if (c == '\\')
		{
			is >> c;
			continue;
		}

		char buf[5]{};
		buf[0] = c;
		is >> buf[1];
		is >> buf[2];
		is >> buf[3];
		auto ul = std::strtoul(buf, nullptr, 16);
		s += (wchar_t)(ul);
	}
}

inline void input_quoted_string(
	std::basic_istream<wchar_t>& is,
	std::basic_string<char>& s)
{
	wchar_t wc;
	is >> wc;
	for (;;)
	{
		is >> wc;
		if (wc == L'"')
		{
			break;
		}
		if (wc == L'\\')
		{
			is >> wc;
			continue;
		}

		wchar_t buf[3]{};
		buf[0] = wc;
		is >> buf[1];
		auto ul = std::wcstoul(buf, nullptr, 16);
		s += (char)(ul);
	}
}

template <typename IStream>
class text_iarchive_impl
	: public text_iarchive_impl_base
{
public:
	using char_type = typename IStream::char_type;

	explicit text_iarchive_impl(IStream& is)
		: m_is(is)
	{}

	void load(bool& t) override
	{
		m_is >> std::boolalpha >> t;
	}

	void load(std::intmax_t& t) override
	{
		m_is >> t;
	}

	void load(std::uintmax_t& t) override
	{
		m_is >> t;
	}

	void load(float& t) override
	{
		load_float(t);
	}

	void load(double& t) override
	{
		load_float(t);
	}

	void load(long double& t) override
	{
		load_float(t);
	}

	void load(std::string& t) override
	{
		input_quoted_string(m_is, t);
	}

	void load(std::wstring& t) override
	{
		input_quoted_string(m_is, t);
	}

	void input(std::string& s) override
	{
		std::basic_string<char_type> tmp;
		m_is >> tmp;
		s = std::string(tmp.begin(), tmp.end());
	}

private:
	template <typename T>
	void load_float(T& t)
	{
		std::basic_string<char_type> tmp;
		m_is >> tmp;
		std::string s(tmp.begin(), tmp.end());
		auto first = s.data();
		auto last  = s.data() + s.length();
		auto result = std::from_chars(first, last, t);
		m_is.seekg(result.ptr - last, std::ios_base::cur);	// 変換できなかったぶん戻す
	}

private:
	text_iarchive_impl& operator=(text_iarchive_impl const&) = delete;

	IStream&	m_is;
};

class text_iarchive : public detail::shared_ptr_table
{
public:
	template <typename IStream>
	explicit text_iarchive(IStream& is)
		: m_impl(new text_iarchive_impl<IStream>(is))
	{
	}

	template <typename T>
	text_iarchive& operator>>(T& t)
	{
		serialization::detail::load_dispatch::invoke(*this, t);
		return *this;
	}

	template <typename T>
	text_iarchive& operator&(T& t)
	{
		return *this >> t;
	}

protected:
	void input(std::string& s)
	{
		m_impl->input(s);
	}

private:
	std::unique_ptr<text_iarchive_impl_base>	m_impl;

private:
	template <typename T>
	friend void load_arithmetic(text_iarchive& ia, T& t)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			ia.m_impl->load(t);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			ia.m_impl->load(t);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			std::uintmax_t i;
			ia.m_impl->load(i);
			t = static_cast<T>(i);
		}
		else
		{
			std::intmax_t i;
			ia.m_impl->load(i);
			t = static_cast<T>(i);
		}
	}

	template <typename CharT>
	friend void load_string(text_iarchive& ia, std::basic_string<CharT>& t)
	{
		ia.m_impl->load(t);
	}
};

}	// namespace serialization

#endif // SERIALIZATION_TEXT_IARCHIVE_HPP
