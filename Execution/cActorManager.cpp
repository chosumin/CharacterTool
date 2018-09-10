#include "stdafx.h"
#include "cActorManager.h"
#include "./GameObject/cActor.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Message/cEntityManager.h"
#include "./Message/cMessageDispatcher.h"
#include "./Viewer/cCameraFactory.h"
#include "./Transform/sTransform.h"

cActorManager::cActorManager(weak_ptr<sGlobalVariable> global)
	: _global(global)
	, _isStart(false)
{
}

cActorManager::~cActorManager()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void cActorManager::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::CharacterTool, shared_from_this());
}

void cActorManager::Update()
{
	if (_isStart)
	{
		if (_actor)
			_actor->Update();
	}
}

void cActorManager::Render()
{
	if (_actor)
		_actor->Render();
}

void cActorManager::PostRender()
{
	bool open = true;
	
	ImGuiWindowFlags flag;
	flag = ImGuiWindowFlags_NoTitleBar;
	flag |= ImGuiWindowFlags_NoMove;
	//flag |= ImGuiWindowFlags_NoResize;
	ImGui::SetNextWindowBgAlpha(0.3f);

	auto pos = ImVec2(300.0f,
					  25.0f);
	ImGui::SetNextWindowPos(pos);
	ImGui::Begin("ActorManager",&open, flag);
	{
		//씬 시작 버튼
		if (ImGui::Button("Start"))
		{
			_isStart = true;
			//플레이어 3인칭 시점 카메라 전환

			if (_actor)
			{
				_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_actor->GetTransform());
			}

			SendStartMessage();
		}
		ImGui::SameLine();
		//씬 종료 버튼
		if (ImGui::Button("Stop"))
		{
			_isStart = false;
			if (_actor)
			{
				_global.lock()->MainCamera = cCameraFactory::Get()->GetFreePointCamera(D3DXVECTOR3{ 0,0,-5 }, D3DXVECTOR2{ 0,0 });

				//todo : 원래 위치로 되돌아오기
				_actor->GetTransform().lock()->Position = { 0,0,0 };
				_actor->Update();
			}

			SendStartMessage();
		}
	}
	ImGui::End();
}

void cActorManager::HandleMessage(const sTelegram & msg)
{
	switch (msg.message)
	{
		case eMessageType::RECIEVE_ACTOR:
		{
			auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
			_actor = actor->lock();
		}
		break;
		case eMessageType::LOAD_ACTOR:
		{
			auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
			_actor = actor->lock();
		}
		break;
	}
}

void cActorManager::FunctionInitialize()
{
}

void cActorManager::SendStartMessage()
{
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CharacterTool, receivers);

	//새 액터를 관여된 객체들에게 전달
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::CLICK_START, (void*)&_isStart);
}