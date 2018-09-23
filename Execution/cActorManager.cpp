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
#include "./Model/cModel.h"
#include "./Graphic/cMaterial.h"
#include "./Helper/cMath.h"

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
	FunctionInitialize();

	_collisionManager = make_unique<cCollisionManager>();
}

void cActorManager::Update()
{
	if (_isStart)
	{
		if (_editedActor)
			_editedActor->Update();

		for (auto&& enemy : _enemies)
		{
			if (enemy)
				enemy->Update();
		}
	}
	else
	{
		for (auto&& enemy : _enemies)
		{
			if (enemy)
				enemy->TestUpdate();
		}
	}

	_collisionManager->Update();
	
	StopScene();
}

void cActorManager::PreRender()
{
}

void cActorManager::Render()
{
	if (_editedActor)
	{
		_editedActor->Render();
	}

	for (auto&& enemy : _enemies)
	{
		if (enemy)
			enemy->Render();
	}
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
			
			SetActorsPosition();
			SetCollision();

			//플레이어 3인칭 시점 카메라 전환
			if (_editedActor)
			{
				_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_editedActor->GetTransform());

				_editedActor->StartScene();
			}

			for (auto&& enemy : _enemies)
			{
				enemy->StartScene();
			}

			SendStartMessage();

			SetOpponent();
		}
		ImGui::SameLine();
	}
	ImGui::End();
}

void cActorManager::HandleMessage(const sTelegram & msg)
{
	if(msgMap[msg.message])
		msgMap[msg.message](msg);
}

void cActorManager::FunctionInitialize()
{
	msgMap[eMessageType::RECIEVE_ACTOR] = [this](const sTelegram& msg)
	{
		auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
		SetEditedActor(actor);
	};

	msgMap[eMessageType::LOAD_ACTOR] = [this](const sTelegram& msg)
	{
		auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
		SetEditedActor(actor);
	};

	msgMap[eMessageType::LOAD_ENEMY] = [this](const sTelegram& msg)
	{
		auto actor = (weak_ptr<cActor>*)(msg.extraInfo);
		SetEnemy(actor);

	};

	msgMap[eMessageType::SET_FREE_POINT] = [this](const sTelegram& msg)
	{
		_global.lock()->MainCamera = cCameraFactory::Get()->GetFreePointCamera(D3DXVECTOR3{ 0,0,20 }, D3DXVECTOR2{ 0,0 });
	};

	msgMap[eMessageType::SET_EDITED_ACTOR_THIRD_POINT] = [this](const sTelegram& msg)
	{
		if (!_editedActor)
			return;
		_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_editedActor->GetTransform());
	};

	msgMap[eMessageType::SET_ENEMY_ACTOR_THIRD_POINT] = [this](const sTelegram& msg)
	{
		//todo : 적 시점으로 전환
		/*if (!_enemy)
			return;
		_global.lock()->MainCamera = cCameraFactory::Get()->GetThirdPersonCamera(_enemy->GetTransform());*/
	};
}

void cActorManager::SendStartMessage()
{
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::CHARACTER_TOOL, receivers);

	//시작 메시지 전달
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::CLICK_START, (void*)&_isStart);
}

void cActorManager::SendRegisterActorMessage(const shared_ptr<cActor> & actor)
{
	vector<UINT> receivers;
	cEntityManager::Get()->GetIdentityGroup(eIdGroup::TERRAIN, receivers);

	//시작 메시지 전달
	cMessageDispatcher::Get()->DispatchMessages(0, GetID(), receivers, eMessageType::REGISTER_ACTOR, (void*)&actor);
}

void cActorManager::StopScene()
{
	if (cKeyboard::Get()->Down(VK_ESCAPE))
	{
		_isStart = false;

		_global.lock()->MainCamera = cCameraFactory::Get()->GetFreePointCamera(D3DXVECTOR3{ 0,30,20 }, D3DXVECTOR2{ 0,0 });

		if (_editedActor)
		{
			_editedActor->StopScene();
		}
		for (auto&& enemy : _enemies)
		{
			enemy->StopScene();
		}

		SendStartMessage();
	}
}

void cActorManager::SetEditedActor(const weak_ptr<cActor>* actor)
{
	_editedActor = actor->lock();
	SendRegisterActorMessage(_editedActor);
}

void cActorManager::SetEnemy(const weak_ptr<cActor>* actor)
{
	auto& enemy = actor->lock();
	
	_enemies.emplace_back(enemy);

	SendRegisterActorMessage(enemy);
}

void cActorManager::SetOpponent()
{
	if (_editedActor)
	{
		_editedActor->ClearEnemies();
		for (auto&& enemy : _enemies)
		{
			_editedActor->SetEnemy(enemy);
		}
	}

	for (auto&& enemy : _enemies)
	{
		enemy->ClearEnemies();
		enemy->SetEnemy(_editedActor);
	}
}

void cActorManager::SetActorsPosition()
{
	if (_editedActor)
	{
		float x = cMath::Random(-150.0f, 150.0f);
		float z = cMath::Random(-150.0f, 150.0f);
		_editedActor->SetPosition(D3DXVECTOR3{ x,0,z });
	}
	for (auto&& enemy : _enemies)
	{
		float x = cMath::Random(-150.0f, 150.0f);
		float z = cMath::Random(-150.0f, 150.0f);
		enemy->SetPosition(D3DXVECTOR3{ x, 0, z });
	}
}

void cActorManager::SetCollision()
{
	_collisionManager->ClearActors();
	
	if (_editedActor)
		_collisionManager->SetActor(_editedActor);

	for (auto&& enemy : _enemies)
	{
		if (enemy)
			_collisionManager->SetActor(enemy);
	}
}
