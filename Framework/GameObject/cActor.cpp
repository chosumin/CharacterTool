#include "stdafx.h"
#include "cActor.h"
#include "./Model/cModel.h"
#include "./Transform/sTransform.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Collider/cActorColliders.h"
#include "./Animator/cAnimator.h"
#include "./Blackboard/cBlackboard.h"

cActor::~cActor()
{
}

void cActor::Init()
{
	_transform = make_shared<sTransform>();

	_colliders = make_shared<cActorColliders>(shared_from_this());

	_animator = make_shared<cAnimator>(_model);

	//hack : �������� ������ ����
	_blackboard = make_shared<cBlackboard>(shared_from_this());

	_behaviorTree = make_shared<cBehaviorTree>(shared_from_this());
	_behaviorTree->Init();

}

//frame : 200������ �Ҹ�
void cActor::Update()
{
	if (_transform)
		_transform->Update();
	
	//�����尡 ��ȭ�Ǹ� �ൿƮ�� ����
	if (_behaviorTree)
	{
		if(_blackboard->IsChange())
			_behaviorTree->Run();
		else
			_behaviorTree->Update();
	}

	if (_model)
		_model->Update(_transform);

	//frame : 100������ �Ҹ�
	if (_animator)
		_animator->Update();

	if (_colliders)
		_colliders->Update();
}

//=> ȣ��� 10������ �Ҹ�
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

weak_ptr<cModel> cActor::GetModel() const
{
	if(_model)
		return _model;

	return weak_ptr<cModel>();
}

weak_ptr<sTransform> cActor::GetTransform() const
{
	if(_transform)
		return _transform;

	return weak_ptr<sTransform>();
}

weak_ptr<cActorColliders> cActor::GetColliders() const
{
	return _colliders;
}

weak_ptr<cAnimator> cActor::GetAnimator() const
{
	return _animator;
}

weak_ptr<cBehaviorTree> cActor::GetBehaviorTree() const
{
	return _behaviorTree;
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

