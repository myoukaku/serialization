/**
 *	@file	weak_ptr.hpp
 *
 *	@brief	weak_ptr の定義
 *
 *	@author	myoukaku
 */

#ifndef SERIALIZATION_WEAK_PTR_HPP
#define SERIALIZATION_WEAK_PTR_HPP

#include <serialization/shared_ptr.hpp>
#include <memory>

namespace serialization
{

template <typename Archive, typename T>
void save(Archive& oa, std::weak_ptr<T> const& t)
{
	auto sp = t.lock();
	save(oa, sp);
}

template <typename Archive, typename T>
void load(Archive& ia, std::weak_ptr<T>& t)
{
	std::shared_ptr<T> sp;
	load(ia, sp);
	t = sp;
}

}	// namespace serialization

#endif // SERIALIZATION_WEAK_PTR_HPP
