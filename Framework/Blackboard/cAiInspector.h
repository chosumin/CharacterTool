#pragma once
#include "cBlackboard.h"

class cAiInspector : public iChangeInspector
{
public:
	cAiInspector(weak_ptr<cBlackboard> blackboard);
	~cAiInspector();

	virtual bool IsChange() override;
private:
	bool DiscoverTarget();
	bool Damage();
private:
	weak_ptr<cBlackboard> _blackboard;
};