#include "stdafx.h"
#include "cEnvironment.h"
#include "./Environment/cWorldGrid.h"
#include "./Environment/cHeightMap.h"

cEnvironment::cEnvironment()
{
}

cEnvironment::~cEnvironment()
{
}

void cEnvironment::Init()
{
	_grid = make_unique<cWorldGrid>();
	_map = make_unique<cHeightMap>(Content + L"Landscape/HeightMap.png");
}

void cEnvironment::Update()
{
}

void cEnvironment::PreRender()
{
}

void cEnvironment::Render()
{
	_grid->Render();
	//_map->Render();
}

void cEnvironment::PostRender()
{
}

void cEnvironment::ResizeScreen()
{
}
