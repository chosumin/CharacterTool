#pragma once
#include <memory>

template<typename T>
__interface iCloneable
{
	std::unique_ptr<T> Clone() const;
};