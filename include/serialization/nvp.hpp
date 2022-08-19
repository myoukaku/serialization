/**
 *	@file	nvp.hpp
 *
 *	@brief	nvp の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_NVP_HPP
#define SERIALIZATION_NVP_HPP

#include <type_traits>

namespace serialization
{

template <typename T>
class nvp
{
public:
	explicit nvp(char const* name, T& t)
		: m_name(name)
		, m_value(t)
	{}

	char const* name() const
	{
		return m_name;
	}

	T& value() const
	{
		return m_value;
	}

private:
	char const*		m_name;
	T&				m_value;
};

template <typename T>
inline nvp<T> const
make_nvp(char const* name, T& t)
{
	return nvp<T>(name, t);
}

template <typename T>
struct is_nvp : public std::false_type {};

template <typename T>
struct is_nvp<nvp<T>> : public std::true_type {};

template <typename T>
struct is_nvp<nvp<T> const> : public std::true_type {};

template <typename T>
struct is_nvp<nvp<T> volatile> : public std::true_type {};

template <typename T>
struct is_nvp<nvp<T> const volatile> : public std::true_type {};

}	// namespace serialization

#endif // SERIALIZATION_NVP_HPP
