#pragma once

class cActor;
class cAnimClip;
class cActorFactory
{
public:
	cActorFactory();
	~cActorFactory();

	shared_ptr<cActor> CreateActor();
	shared_ptr<cActor> CreateActor(wstring jsonPath);
private:
	void InitActor();

	void CreateTransform(Json::Value& root);
	void CreateModel(Json::Value& root);
	void CreateCollider(Json::Value& root);
	void CreateAnimator(Json::Value& root);
	void CreateBehaviorTree(Json::Value& root);
	void CreateBlackboard(Json::Value& root);
	void CreateAction();
private:
	void CreateEventFrames(Json::Value& clipJson,
						   shared_ptr<cAnimClip>& clip);
private:
	wstring _jsonPath;
	shared_ptr<cActor> _actor;
};