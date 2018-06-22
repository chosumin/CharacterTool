#pragma once
#include "./System/cWindow.h"

class cWindow;
template<typename T>
class cSingletone
{
public:
	static T* Get()
	{
		if (mInstance == nullptr)
		{
			mInstance = new T();
			cWindow::m_uSingletoneCount++;
		}
		return mInstance;
	}

	static void Delete()
	{
		cWindow::m_uSingletoneCount--;
		SAFE_DELETE(mInstance);
	}
protected:
	cSingletone() {}
	virtual ~cSingletone() {}
private:
	static T* mInstance;
};

template<typename T>
T* cSingletone<T>::mInstance = nullptr;