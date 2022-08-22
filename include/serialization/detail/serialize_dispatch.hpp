/**
 *	@file	serialize_dispatch.hpp
 *
 *	@brief	serialize_dispatch の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_SERIALIZE_DISPATCH_HPP
#define SERIALIZATION_SERIALIZE_DISPATCH_HPP

#include <serialization/detail/has_adl_serialize.hpp>
#include <serialization/version.hpp>

namespace serialization
{

namespace detail
{

class serialize_dispatch
{
private:
	template <typename>
	struct always_false { static const bool value = false; };

public:
	template <typename Archive, typename T>
	static void invoke(Archive& ar, T const& t, serialization::version_t version)
	{
		if constexpr (has_adl_serialize<Archive&, T&, serialization::version_t>::value)
		{
			serialize(ar, const_cast<T&>(t), version);
		}
		else if constexpr (has_adl_serialize<Archive&, T&>::value)
		{
			serialize(ar, const_cast<T&>(t));
		}
		else
		{
			static_assert(always_false<T>::value, "This type is not serializable.");
		}
	}
};

}	// namespace detail

}	// namespace serialization

#endif // SERIALIZATION_SERIALIZE_DISPATCH_HPP
