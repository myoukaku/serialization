/**
 *	@file	binary_iarchive.hpp
 *
 *	@brief	binary_iarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_binary_iarchive_HPP
#define SERIALIZATION_binary_iarchive_HPP

#include <cstdint>
#include <memory>
#include <type_traits>

namespace serialization
{

class binary_iarchive_impl_base
{
public:
	virtual ~binary_iarchive_impl_base()
	{
	}

	virtual void load(std::uintmax_t&, std::streamsize) = 0;
};

template <typename IStream>
class binary_iarchive_impl
	: public binary_iarchive_impl_base
{
public:
	using char_type = typename IStream::char_type;

	explicit binary_iarchive_impl(IStream& is)
		: m_is(is)
	{}

	void load(std::uintmax_t& t, std::streamsize size) override
	{
		auto p = reinterpret_cast<char_type*>(&t);
		m_is.read(p, (size + (sizeof(char_type) - 1)) / sizeof(char_type));
	}

private:
	binary_iarchive_impl& operator=(binary_iarchive_impl const&) = delete;

	IStream&	m_is;
};

class binary_iarchive
{
public:
	template <typename IStream>
	explicit binary_iarchive(IStream& is)
		: m_impl(new binary_iarchive_impl<IStream>(is))
	{
	}

	template <typename T>
	binary_iarchive& operator>>(T& t)
	{
		load(t);
		return *this;
	}

	template <typename T>
	binary_iarchive& operator&(T& t)
	{
		return *this >> t;
	}

private:
	template <typename T>
	void load(T& t)
	{
		std::uintmax_t i = 0;
		m_impl->load(i, sizeof(T));
		t = static_cast<T>(i);
	}

	std::unique_ptr<binary_iarchive_impl_base>	m_impl;
};

}	// namespace serialization

#endif // SERIALIZATION_binary_iarchive_HPP
