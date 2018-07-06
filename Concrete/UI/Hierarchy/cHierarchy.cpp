#include "stdafx.h"
#include "cHierarchy.h"
#include "./Model/cModel.h"
#include "./GameObject/cActor.h"

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
	ImGui::Begin("Hierarchy");
	{
		for (auto&& actor : _actor)
		{
			if (actor.expired())
				continue;

			//actor.lock()->g
		}
	}
	ImGui::End();
	
}

void cHierarchy::PicksModel(weak_ptr<cModel> model)
{
	//_model = model;
}