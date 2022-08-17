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
#include <cstdint>
#include <limits>
#include <memory>
#include <ios>		// std::scientific
#include <iomanip>	// std::setprecision
#include <array>

namespace serialization
{

class text_oarchive_impl_base
{
public:
	virtual ~text_oarchive_impl_base()
	{}

	virtual void save(std::intmax_t) = 0;
	virtual void save(std::uintmax_t) = 0;
	virtual void save(float) = 0;
	virtual void save(double) = 0;
	virtual void save(long double) = 0;
};

template <typename OStream>
class text_oarchive_impl
	: public text_oarchive_impl_base
{
public:
	using char_type = typename OStream::char_type;

	explicit text_oarchive_impl(OStream& os)
		: m_os(os)
	{}

	void save(std::intmax_t t) override
	{
		m_os << t << " ";
	}

	void save(std::uintmax_t t) override
	{
		m_os << t << " ";
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
		m_os << buf.data() << " ";
#endif
	}

private:
	text_oarchive_impl& operator=(text_oarchive_impl const&) = delete;

	OStream&	m_os;
};

class text_oarchive
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
		return *this;
	}

	template <typename T>
	text_oarchive& operator&(T const& t)
	{
		return *this << t;
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
		else if constexpr (std::is_unsigned_v<T>)
		{
			oa.m_impl->save(static_cast<std::uintmax_t>(t));
		}
		else
		{
			oa.m_impl->save(static_cast<std::intmax_t>(t));
		}
	}
};

}	// namespace serialization

#endif // SERIALIZATION_TEXT_OARCHIVE_HPP
