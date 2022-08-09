/**
 *	@file	binary_oarchive.hpp
 *
 *	@brief	binary_oarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_binary_oarchive_HPP
#define SERIALIZATION_binary_oarchive_HPP

#include <cstdint>
#include <limits>
#include <memory>
#include <ios>

namespace serialization
{

class binary_oarchive_impl_base
{
public:
	virtual ~binary_oarchive_impl_base()
	{}

	virtual void save(std::uintmax_t, std::streamsize) = 0;
};

template <typename OStream>
class binary_oarchive_impl
	: public binary_oarchive_impl_base
{
public:
	using char_type = typename OStream::char_type;

	explicit binary_oarchive_impl(OStream& os)
		: m_os(os)
	{}

	void save(std::uintmax_t t, std::streamsize size) override
	{
		auto p = reinterpret_cast<char_type const*>(&t);
		m_os.write(p, (size + (sizeof(char_type) - 1)) / sizeof(char_type));
	}

private:
	binary_oarchive_impl& operator=(binary_oarchive_impl const&) = delete;

	OStream&	m_os;
};

class binary_oarchive
{
public:
	template <typename OStream>
	explicit binary_oarchive(OStream& os)
		: m_impl(new binary_oarchive_impl<OStream>(os))
	{
	}

	template <typename T>
	binary_oarchive& operator<<(T const& t)
	{
		save(t);
		return *this;
	}

	template <typename T>
	binary_oarchive& operator&(T const& t)
	{
		return *this << t;
	}

private:
	template <typename T>
	void save(T const& t)
	{
		m_impl->save(static_cast<std::uintmax_t>(t), sizeof(T));
	}

	std::unique_ptr<binary_oarchive_impl_base>	m_impl;
};

}	// namespace serialization

#endif // SERIALIZATION_binary_oarchive_HPP
