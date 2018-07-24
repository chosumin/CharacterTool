#include "stdafx.h"
#include "cModels.h"

cModels::cModels(weak_ptr<sGlobalVariable> global)
	:_global(global)
{
	cDebug::SetDebugMode(true);
}

cModels::~cModels()
{
}

void cModels::Init()
{
}

void cModels::Update()
{
}

void cModels::Render()
{
}

void cModels::PostRender()
{
}