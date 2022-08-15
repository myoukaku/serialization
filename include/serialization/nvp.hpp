/**
 *	@file	nvp.hpp
 *
 *	@brief	nvp の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_NVP_HPP
#define SERIALIZATION_NVP_HPP

#include <serialization/access.hpp>

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

private:
	friend class serialization::access;

	template <typename Archive>
	void serialize(Archive& ar)
	{
		ar & m_value;
	}
};

template <typename T>
inline nvp<T> const
make_nvp(char const* name, T& t)
{
	return nvp<T>(name, t);
}

}	// namespace serialization

#endif // SERIALIZATION_NVP_HPP
