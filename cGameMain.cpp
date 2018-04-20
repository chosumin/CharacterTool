#include "stdafx.h"
#include "cGameMain.h"

cGameMain::cGameMain()
{
}

cGameMain::~cGameMain()
{
	for (auto& i : m_vecObject)
		SAFE_DELETE(i);
}

void cGameMain::Init()
{
}

void cGameMain::Update()
{
	for (auto i : m_vecObject)
		i->Update();
}

void cGameMain::PreRender()
{
	for (auto& i : m_vecObject)
		i->PreRender();
}

void cGameMain::Render()
{
	for (auto& i : m_vecObject)
		i->Render();
}

void cGameMain::PostRender()
{
	for (auto& i : m_vecObject)
		i->PostRender();
}

void cGameMain::ResizeScreen()
{
	for (auto& i : m_vecObject)
		i->ResizeScreen();
}
