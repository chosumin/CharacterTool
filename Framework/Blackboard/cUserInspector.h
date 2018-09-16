#pragma once
#include "cBlackboard.h"

class cUserInspector : public iChangeInspector
{
public:
	cUserInspector(weak_ptr<cBlackboard> blackboard);
	~cUserInspector();

	virtual bool IsChange() override;
private:
	bool OnMoveEvent();
	void OnMoveKeyEvent(D3DXVECTOR3& direction, float value, eKeyEventType type);
	bool OnDashEvent();
	bool OnAttackEvent();

	void SetPositionData();
private:
	weak_ptr<cBlackboard> _blackboard;
};