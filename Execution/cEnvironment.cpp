#include "stdafx.h"
#include "cEnvironment.h"
#include "./Environment/cWorldGrid.h"
#include "./Environment/cSky.h"

cEnvironment::cEnvironment(weak_ptr<sGlobalVariable> global)
	: _global(global)
{
}

cEnvironment::~cEnvironment()
{
}

void cEnvironment::Init()
{
	_grid = make_shared<cWorldGrid>(_global);
	_grid->Init();

	_sky = make_unique<cSky>(_global);
}

void cEnvironment::Update()
{
	_grid->Update();
	_sky->Update();
}

void cEnvironment::PreRender()
{
	_grid->PreRender();
}

void cEnvironment::Render()
{
	_sky->Render();
	_grid->Render();
}

void cEnvironment::PostRender()
{
	_sky->PostRender();
}

void cEnvironment::ResizeScreen()
{
}
