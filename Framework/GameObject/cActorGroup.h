#pragma once
#include "./Message/cBaseGameEntity.h"

enum class eIdGroup;
class cActor;
class cActorGroup : public cBaseGameEntity
{
public:
	cActorGroup(eIdGroup groupType);
	~cActorGroup();

	void Init();
	void Update();
	void Render();
public:
	eIdGroup GetGroupType() { return _groupType; }
private:
	eIdGroup _groupType;
	list<shared_ptr<cActor>> _actors;
};