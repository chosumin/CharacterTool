#pragma once
#include <memory>

template<typename T>
__interface iClonable
{
	std::unique_ptr<T> Clone() const;
};