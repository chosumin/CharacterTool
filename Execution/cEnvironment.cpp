#include "stdafx.h"
#include "cEnvironment.h"
#include "./Environment/cWorldGrid.h"
#include "./Environment/cHeightMap.h"
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
	_grid = make_unique<cWorldGrid>();
	_map = make_unique<cHeightMap>(Content + L"Landscape/HeightMap.png");

	_sky = make_unique<cSky>(_global);
}

void cEnvironment::Update()
{
	_sky->Update();
}

void cEnvironment::PreRender()
{
}

void cEnvironment::Render()
{
	_sky->Render();
	_grid->Render();
	//_map->Render();
}

void cEnvironment::PostRender()
{
	_sky->PostRender();
}

void cEnvironment::ResizeScreen()
{
}
