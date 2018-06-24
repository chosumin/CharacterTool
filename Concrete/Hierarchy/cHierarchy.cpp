#include "stdafx.h"
#include "cHierarchy.h"
#include "./Model/cModel.h"

cHierarchy::cHierarchy()
{
}

cHierarchy::~cHierarchy()
{
}

void cHierarchy::Init()
{
}

void cHierarchy::Update()
{
}

void cHierarchy::PostRender()
{
	if (_model.expired())
		return;
	
	ImGui::Begin("Hierarchy");
	{
		_model.lock()->ShowHierarchy();
	}
	ImGui::End();
}

void cHierarchy::PicksModel(weak_ptr<cModel> model)
{
	_model = model;
}