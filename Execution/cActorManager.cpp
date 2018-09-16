#include "stdafx.h"
#include "cActorManager.h"
#include "./Concrete/Message/eIdGroup.h"
#include "./Concrete/Message/eMessageType.h"
#include "./GameObject/cActor.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Viewer/cCameraFactory.h"
#include "./Transform/sTransform.h"
#include "./Animator/cAnimator.h"
#include "./Command/cInputHandler.h"

#include "./Interaction/cCollisionManager.h"
#include "./Collider/cActorColliders.h"
#include "./Collider/cRayCollider.h"
#include "./UI/Gizmo/cGizmo.h"

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
	cEntityManager::Get()->RegisterEntity(eIdGroup::CHARACTER_TOOL, shared_from_this());

	_collisionManager = make_unique<cCollisionManager>();
}

void cActorManager::Update()
{
	if (_isStart)
	{
		if (_editedActor)
			_editedActor->Update();
		if(_enemy)
			_enemy->Update();
	}
	else
	{
		if(_enemy)
			_enemy->TestUpdate();
	}

	_collisionManager->Update();

	StopScene();
}

void cActorManager::Render()
{
	if (_editedActor)
		_editedActor->Render();

	if (_enemy)
		_enemy->Render();
}

void cActorManager::PostRender()
{
	bool open = true;
	
	ImGuiWindowFlags flag;
	flag = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	ImGui::SetNextWindowBgAlpha(0.3f);

	ImGui::Begin("ActorManager",&open, flag);
	{
		if (ImGui::ArrowButton("Start", ImGuiDir_Right))//씬 시작 버튼
		{
			_isStart = true;
			
			//플레이어 3인칭 시점 카메라 전환
			if (_editedActor)
			{
				_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_editedActor->GetTransform());

				_editedActor->StartScene();
				_editedActor->SetEnemy(_enemy);
				_collisionManager->SetPlayer(_editedActor);
			}

			if (_enemy)
			{
				_enemy->StartScene();
				_enemy->SetEnemy(_editedActor);
				_collisionManager->SetEnemy(_enemy);
			}

			SendStartMessage();
		}
		ImGui::SameLine();
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
			SetEditedActor(actor);
		}
		break;
		case eMessageType::LOAD_ACTOR:
		{
			auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
			SetEditedActor(actor);
		}
		break;
		case eMessageType::LOAD_ENEMY:
		{
			auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
			SetEnemy(actor);
		}
		break;
		case eMessageType::SET_FREE_POINT:
		{
			_global.lock()->MainCamera = cCameraFactory::Get()->GetFreePointCamera(D3DXVECTOR3{ 0,0,20 }, D3DXVECTOR2{ 0,0 });
		}
		break;
		case eMessageType::SET_EDITED_ACTOR_THIRD_POINT:
		{
			if (!_editedActor)
				return;
			_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_editedActor->GetTransform());
		}
		break;
		case eMessageType::SET_ENEMY_ACTOR_THIRD_POINT:
		{
			if (!_enemy)
				return;
			_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_enemy->GetTransform());
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
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

	//시작 메시지 전달
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::CLICK_START, (void*)&_isStart);
}

void cActorManager::StopScene()
{
	if (cKeyboard::Get()->Down(VK_ESCAPE))
	{
		_isStart = false;

		_global.lock()->MainCamera = cCameraFactory::Get()->GetFreePointCamera(D3DXVECTOR3{ 0,30,20 }, D3DXVECTOR2{ 0,0 });

		if (_editedActor)
			_editedActor->StopScene(D3DXVECTOR3{ 30,0,0 });

		if (_enemy)
			_editedActor->StopScene(D3DXVECTOR3{ -30,0,0 });

		SendStartMessage();
	}
}

void cActorManager::SetEditedActor(const weak_ptr<cActor>* actor)
{
	_editedActor = actor->lock();

	if(_editedActor)
		_editedActor->SetEnemy(_enemy);

	if (_enemy)
		_enemy->SetEnemy(_editedActor);
}

void cActorManager::SetEnemy(const weak_ptr<cActor>* actor)
{
	_enemy = actor->lock();

	if(_enemy)
		_editedActor->SetEnemy(_enemy);
}