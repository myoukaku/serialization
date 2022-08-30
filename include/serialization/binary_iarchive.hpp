/**
 *	@file	binary_iarchive.hpp
 *
 *	@brief	binary_iarchive の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_binary_iarchive_HPP
#define SERIALIZATION_binary_iarchive_HPP

#include <serialization/detail/load_dispatch.hpp>
#include <serialization/detail/shared_ptr_table.hpp>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <type_traits>
#include <vector>
#include <string>

namespace serialization
{

class binary_iarchive_impl_base
{
public:
	virtual ~binary_iarchive_impl_base()
	{
	}

	virtual void load(void* dst, std::size_t size) = 0;
};

template <typename CharT, typename Traits>
void load_binary(std::basic_istream<CharT, Traits>& is, void* p, std::size_t size)
{
	if constexpr (sizeof(CharT) == 1)
	{
		is.read(static_cast<CharT*>(p), size);
	}
	else
	{
		auto const count = (size + (sizeof(CharT) - 1)) / sizeof(CharT);
		std::vector<CharT> buf(count);	// TODO basic_stringを使ったほうが、countが小さい場合に高速かもしれない
		is.read(buf.data(), count);
		std::memcpy(p, buf.data(), size);
	}
}

template <typename IStream>
class binary_iarchive_impl
	: public binary_iarchive_impl_base
{
public:
	explicit binary_iarchive_impl(IStream& is)
		: m_is(is)
	{}

	void load(void* dst, std::size_t size) override
	{
		load_binary(m_is, dst, size);
	}

private:
	binary_iarchive_impl& operator=(binary_iarchive_impl const&) = delete;

	IStream&	m_is;
};

class binary_iarchive : public detail::shared_ptr_table
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
		serialization::detail::load_dispatch::invoke(*this, t);
		return *this;
	}

	template <typename T>
	binary_iarchive& operator&(T& t)
	{
		return *this >> t;
	}

private:
	std::unique_ptr<binary_iarchive_impl_base>	m_impl;

private:
	template <typename T>
	friend void load_arithmetic(binary_iarchive& ia, T& t)
	{
		ia.m_impl->load(&t, sizeof(T));
	}

	template <typename CharT>
	friend void load_string(binary_iarchive& ia, std::basic_string<CharT>& t)
	{
		std::size_t len;
		ia.m_impl->load(&len, sizeof(len));

		t.resize(len);
		ia.m_impl->load(t.data(), len * sizeof(CharT));
	}
};

}	// namespace serialization

#endif // SERIALIZATION_binary_iarchive_HPP
