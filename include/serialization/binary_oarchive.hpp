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

	virtual void save(void const* src, std::size_t size) = 0;
};

template <typename CharT, typename Traits>
void save_binary(std::basic_ostream<CharT, Traits>& os, void const* p, std::size_t size)
{
	if constexpr (sizeof(CharT) == 1)
	{
		os.write(static_cast<CharT const*>(p), size);
	}
	else
	{
		auto const count = (size + (sizeof(CharT) - 1)) / sizeof(CharT);
		std::vector<CharT> buf(count);	// TODO basic_stringを使ったほうが、countが小さい場合に高速かもしれない
		std::memcpy(buf.data(), p, size);
		os.write(buf.data(), count);
	}
}

template <typename OStream>
class binary_oarchive_impl
	: public binary_oarchive_impl_base
{
public:
	explicit binary_oarchive_impl(OStream& os)
		: m_os(os)
	{}

	void save(void const* src, std::size_t size) override
	{
		save_binary(m_os, src, size);
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
		m_impl->save(&t, sizeof(T));
	}

	std::unique_ptr<binary_oarchive_impl_base>	m_impl;
};

}	// namespace serialization

#endif // SERIALIZATION_binary_oarchive_HPP
