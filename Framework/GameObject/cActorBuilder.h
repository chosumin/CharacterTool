#pragma once

class cActor;
class cActorBuilder
{
public:
	cActorBuilder();
	~cActorBuilder();

	shared_ptr<cActor> CreateActor();

	cActorBuilder& CreateTransform();
	cActorBuilder& CreateBehaviorTree(int i);
	cActorBuilder& CreateModel();
	cActorBuilder& CreateAction();
	cActorBuilder& CreateCollider();
private:
	shared_ptr<cActor> _actor;
};