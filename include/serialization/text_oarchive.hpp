/**
 *	@file	text_oarchive.hpp
 *
 *	@brief	text_oarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_TEXT_OARCHIVE_HPP
#define SERIALIZATION_TEXT_OARCHIVE_HPP

#include <serialization/detail/save_dispatch.hpp>
#include <serialization/detail/shared_ptr_table.hpp>
#include <cstdint>
#include <limits>
#include <memory>
#include <ios>		// std::scientific
#include <iomanip>	// std::setprecision
#include <array>
#include <string>
#include <iomanip>
#include <charconv>

namespace serialization
{

class text_oarchive_impl_base
{
public:
	virtual ~text_oarchive_impl_base()
	{}

	virtual void save(bool) = 0;
	virtual void save(std::intmax_t) = 0;
	virtual void save(std::uintmax_t) = 0;
	virtual void save(float) = 0;
	virtual void save(double) = 0;
	virtual void save(long double) = 0;
	virtual void save(std::string const&) = 0;
	virtual void save(std::wstring const&) = 0;
	virtual void output(std::string const&) = 0;
};

template <typename CharT, typename Traits>
inline void output_quoted_string(
	std::basic_ostream<CharT, Traits>& os,
	std::basic_string<CharT, Traits> const& s)
{
	os << std::quoted(s);
}

inline void output_quoted_string(
	std::basic_ostream<char>& os,
	std::basic_string<wchar_t> const& s)
{
	os << "\"";
	auto const old_flags = os.flags();
	os << std::hex;
	for (auto c : s)
	{
		os << "\\x";
		os << std::right << std::setw(4) << std::setfill('0') << std::uint16_t(c);
	}
	os.flags(old_flags);
	os << "\"";
}

inline void output_quoted_string(
	std::basic_ostream<wchar_t>& os,
	std::basic_string<char> const& s)
{
	os << "\"";
	auto const old_flags = os.flags();
	os << std::hex;
	for (auto c : s)
	{
		os << "\\x";
		os << std::right << std::setw(2) << std::setfill(L'0') << std::uint8_t(c);
	}
	os.flags(old_flags);
	os << "\"";
}

template <typename OStream>
class text_oarchive_impl
	: public text_oarchive_impl_base
{
public:
	explicit text_oarchive_impl(OStream& os)
		: m_os(os)
	{}

	void save(bool t) override
	{
		auto const old_flags = m_os.flags();
		m_os << std::boolalpha << t;
		m_os.flags(old_flags);
	}

	void save(std::intmax_t t) override
	{
		m_os << t;
	}

	void save(std::uintmax_t t) override
	{
		m_os << t;
	}

	void save(float t) override
	{
		save_float(t);
	}

	void save(double t) override
	{
		save_float(t);
	}

	void save(long double t) override
	{
		save_float(t);
	}

	void save(std::string const& t) override
	{
		output_quoted_string(m_os, t);
	}

	void save(std::wstring const& t) override
	{
		output_quoted_string(m_os, t);
	}

	void output(std::string const& s) override
	{
		m_os << s.c_str();
	}

private:
	template <typename T>
	void save_float(T t)
	{
#if 0
		auto const digits = std::numeric_limits<T>::max_digits10;
		m_os << std::setprecision(digits) << std::scientific << t << " ";
#else
		auto constexpr digits =
			4 +	// sign, decimal point, "e+" or "e-"
			std::numeric_limits<T>::max_digits10 +
			3;	// log10(max_exponent10)
		std::array<char, digits + 1> buf{};
		auto result = std::to_chars(buf.data(), buf.data() + buf.size(), t);
		//std::basic_string<char_type> s(buf.data(), result.ptr);
		m_os << buf.data();
#endif
	}

private:
	text_oarchive_impl& operator=(text_oarchive_impl const&) = delete;

	OStream&	m_os;
};

class text_oarchive : public detail::shared_ptr_table
{
public:
	template <typename OStream>
	explicit text_oarchive(OStream& os)
		: m_impl(new text_oarchive_impl<OStream>(os))
	{
	}

	template <typename T>
	text_oarchive& operator<<(T const& t)
	{
		serialization::detail::save_dispatch::invoke(*this, t);
		m_impl->output(" ");
		return *this;
	}

	template <typename T>
	text_oarchive& operator&(T const& t)
	{
		return *this << t;
	}

protected:
	void output(std::string const& s)
	{
		m_impl->output(s);
	}

private:
	std::unique_ptr<text_oarchive_impl_base>	m_impl;

private:
	template <typename T>
	friend void save_arithmetic(text_oarchive& oa, T const& t)
	{
		if constexpr (std::is_floating_point_v<T>)
		{
			oa.m_impl->save(t);
		}
		else if constexpr (std::is_same_v<T, bool>)
		{
			oa.m_impl->save(t);
		}
		else if constexpr (std::is_unsigned_v<T>)
		{
			oa.m_impl->save(static_cast<std::uintmax_t>(t));
		}
		else
		{
			oa.m_impl->save(static_cast<std::intmax_t>(t));
		}
	}

	template <typename CharT>
	friend void save_string(text_oarchive& oa, std::basic_string<CharT> const& t)
	{
		oa.m_impl->save(t);
	}
};

}	// namespace serialization

#endif // SERIALIZATION_TEXT_OARCHIVE_HPP
