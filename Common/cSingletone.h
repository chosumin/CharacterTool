#pragma once
#include "stdafx.h"

class cWindow;
template<typename T>
class cSingletone
{
public:
	static T* Get()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new T();
			cWindow::m_uSingletoneCount++;
		}
		return m_pInstance;
	}

	static void Delete()
	{
		cWindow::m_uSingletoneCount--;
		SAFE_DELETE(m_pInstance);
	}
protected:
	virtual ~cSingletone() {}
private:
	static T*	m_pInstance;
};

template<typename T>
T* cSingletone<T>::m_pInstance = nullptr;