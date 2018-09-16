#include "stdafx.h"
#include "cAiInspector.h"
#include "./GameObject/cActor.h"
#include "./Helper/cMath.h"

cAiInspector::cAiInspector(weak_ptr<cBlackboard> blackboard)
	: _blackboard(blackboard)
{
}

cAiInspector::~cAiInspector()
{
}

bool cAiInspector::IsChange()
{
	bool onEvent = false;
	onEvent |= DiscoverTarget();

	return onEvent;
}

bool cAiInspector::DiscoverTarget()
{
	auto boardPtr = _blackboard.lock();
	if (!boardPtr)
		return false;

	//적의 위치 세팅
	D3DXVECTOR3 targetPos;
	auto enemyPtr = boardPtr->GetEnemy().lock();
	if (enemyPtr)
	{
		enemyPtr->GetPosition(targetPos);
		boardPtr->SetVector3("TargetPos", targetPos);
	}

	//자신의 위치 세팅
	D3DXVECTOR3 myPos;
	auto mePtr = boardPtr->GetActor().lock();
	mePtr->GetPosition(myPos);
	boardPtr->SetVector3("MyPos", myPos);

	//거리 판단
	D3DXVECTOR3 gap = targetPos - myPos;
	float dist = D3DXVec3Length(&gap);
	boardPtr->SetFloat("DistanceGap", dist);

	//타겟과의 방향 계산
	D3DXVec3Normalize(&gap, &gap);
	boardPtr->SetVector3("TargetDirection", gap);

	return false;
}

bool cAiInspector::Damage()
{
	return false;
}
