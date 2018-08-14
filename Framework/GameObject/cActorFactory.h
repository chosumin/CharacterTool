#pragma once

class cActor;
class cActorFactory
{
public:
	cActorFactory();
	~cActorFactory();

	shared_ptr<cActor> CreateActor();
	shared_ptr<cActor> CreateActor(wstring jsonPath);
private:
	void CreateTransform(Json::Value& root);
	void CreateModel(Json::Value& root);
	void CreateCollider(Json::Value& root);
	void CreateAnimator(Json::Value& root);
	void CreateBehaviorTree(Json::Value& root);
	void CreateAction();
private:
	shared_ptr<cActor> _actor;
};