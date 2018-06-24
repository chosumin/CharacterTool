#include "stdafx.h"
#include "cEnvironment.h"
#include "./Environment/cWorldGrid.h"

cEnvironment::cEnvironment()
{
}

cEnvironment::~cEnvironment()
{
}

void cEnvironment::Init()
{
	_grid = make_unique<cWorldGrid>();
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
}

void cEnvironment::PostRender()
{
}

void cEnvironment::ResizeScreen()
{
}
