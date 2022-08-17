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
#include <cstdint>
#include <memory>
#include <type_traits>
#include <charconv>

#pragma warning(disable : 4244)

namespace serialization
{

class text_iarchive_impl_base
{
public:
	virtual ~text_iarchive_impl_base()
	{
	}

	virtual void load(std::intmax_t&) = 0;
	virtual void load(std::uintmax_t&) = 0;
	virtual void load(float&) = 0;
	virtual void load(double&) = 0;
	virtual void load(long double&) = 0;
};

template <typename IStream>
class text_iarchive_impl
	: public text_iarchive_impl_base
{
public:
	using char_type = typename IStream::char_type;

	explicit text_iarchive_impl(IStream& is)
		: m_is(is)
	{}

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

private:
	template <typename T>
	void load_float(T& t)
	{
		std::basic_string<char_type> tmp;
		m_is >> tmp;
		std::string s(tmp.begin(), tmp.end());
		std::from_chars(s.data(), s.data() + s.length(), t);
	}

private:
	text_iarchive_impl& operator=(text_iarchive_impl const&) = delete;

	IStream&	m_is;
};

class text_iarchive
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
};

}	// namespace serialization

#endif // SERIALIZATION_TEXT_IARCHIVE_HPP
