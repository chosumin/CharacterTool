#include "stdafx.h"
#include "cActor.h"
#include "./Concrete/Message/eIdGroup.h"
#include "./Concrete/Message/eMessageType.h"
#include "./Model/cModel.h"
#include "./Transform/sTransform.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Collider/cActorColliders.h"
#include "./Animator/cAnimator.h"
#include "./Blackboard/cBlackboard.h"

cActor::~cActor()
{
	cEntityManager::Get()->RemoveEntity(GetID());
}

void cActor::Init()
{
	cEntityManager::Get()->RegisterEntity(eIdGroup::ACTOR, downcasted_shared_from_this<cActor>());

	_transform = make_shared<sTransform>();

	_colliders = make_shared<cActorColliders>(downcasted_shared_from_this<cActor>());

	_animator = make_shared<cAnimator>(_model);

	_blackboard = make_shared<cBlackboard>(downcasted_shared_from_this<cActor>());
	_blackboard->Init();

	_behaviorTree = make_shared<cBehaviorTree>(downcasted_shared_from_this<cActor>());
	_behaviorTree->Init();
}

void cActor::Update()
{
	if (_transform)
		_transform->Update();
	
	//�����尡 ��ȭ�Ǹ� �ൿƮ�� ����
	if (_behaviorTree && _blackboard)
	{
		if(_blackboard->IsChange())
			_behaviorTree->Run();
		else
			_behaviorTree->Update();
	}

	if (_model)
		_model->Update(_transform);

	if (_animator)
		_animator->Update();

	if (_colliders)
		_colliders->Update();
}

void cActor::Render()
{
	if (_transform)
		_transform->SetVSBuffer(1);

	if (_model)
	{
		_model->SetPlayedBuffer(false);

		if (_animator &&
			_animator->GetMode() != cAnimator::Mode::STOP)
			_model->SetPlayedBuffer(true);

		_model->Render();
	}
		
	//�� Ŭ�������� ���� �ݶ��̴� ������ ó��
	/*if (_colliders)
		_colliders->Render();*/
}

void cActor::RenderShadow()
{
	if (_model)
	{
		_model->SetPlayedBuffer(false);

		if (_animator &&
			_animator->GetMode() != cAnimator::Mode::STOP)
			_model->SetPlayedBuffer(true);

		_model->Render(false);
	}
}

void cActor::TestUpdate()
{
	if (_transform)
		_transform->Update();

	if (_model)
		_model->Update(_transform);

	//frame : 100������ �Ҹ�
	if (_animator)
		_animator->Update();

	if (_colliders)
		_colliders->Update();
}

void cActor::ClearEnemies()
{
	_blackboard->ClearEnemies();
}

void cActor::SetEnemy(const weak_ptr<cActor>& actor)
{
	_blackboard->SetEnemy(actor);
}

void cActor::StartScene()
{
	_behaviorTree->Run();
}

void cActor::StopScene()
{
	_animator->SetMode(cAnimator::Mode::STOP);
	_blackboard->ResetData();
}

void cActor::GetPosition(D3DXVECTOR3 & position)
{
	position = _transform->Position;
}

void cActor::SetPosition(D3DXVECTOR3 & position)
{
	_transform->Position = position;
}

bool cActor::Collide(const weak_ptr<class cCollider>& collider)
{
	if (_colliders->Collide(collider) == true)
	{
		D3DXVECTOR3 direction;
		_transform->GetDirection(direction);

		_transform->Move(_transform->Velocity, -direction);
		return true;
	}

	return false;
}

bool cActor::Attack()
{
	auto& enemies = _blackboard->GetEnemies();
	for (auto&& enemy : enemies)
	{
		auto enemyPtr = enemy.lock();

		if (!enemyPtr)
			return false;

		auto enemyCols = enemyPtr->GetColliders();
		if (_colliders->Attack(enemyCols))
		{
			//������ �޽��� ����
			enemyPtr->Damage(downcasted_shared_from_this<cActor>());
			return true;
		}
	}
	
	return false;
}

void cActor::Damage(const weak_ptr<cActor>& opponent)
{
	auto opponentPtr = opponent.lock();

	auto& transformPtr = opponentPtr->GetTransform();
	
	_blackboard->SetVector3("AttackerPosition", transformPtr->Position);
	_blackboard->SetBool("Damage", true);
	_blackboard->SetFloat("HP", _blackboard->GetFloat("HP") - 1);
	_behaviorTree->Run();
}

void cActor::HandleMessage(const sTelegram & msg)
{
}

void cActor::FunctionInitialize()
{
}

const shared_ptr<cModel> & cActor::GetModel() const
{
	return _model;
}

void cActor::SetModel(weak_ptr<cModel> model)
{
	if (model.expired())
		return;

	_model = model.lock();
}

cActor::cActor()
	:_name(L"New Actor")
{
}
