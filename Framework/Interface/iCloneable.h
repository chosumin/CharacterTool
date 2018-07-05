#pragma once
#include <memory>

template<typename T>
class iCloneable
{
public:
	virtual ~iCloneable() {}
	virtual std::unique_ptr<T> Clone() const = 0;
};