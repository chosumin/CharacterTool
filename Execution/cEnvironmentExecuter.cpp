#include "stdafx.h"
#include "cEnvironmentExecuter.h"
#include "./Environment/cWorldGrid.h"

cEnvironmentExecuter::cEnvironmentExecuter()
{
}

cEnvironmentExecuter::~cEnvironmentExecuter()
{
}

void cEnvironmentExecuter::Init()
{
	_grid = make_unique<cWorldGrid>();
}

void cEnvironmentExecuter::Update()
{
}

void cEnvironmentExecuter::PreRender()
{
}

void cEnvironmentExecuter::Render()
{
	_grid->Render();
}

void cEnvironmentExecuter::PostRender()
{
}

void cEnvironmentExecuter::ResizeScreen()
{
}
