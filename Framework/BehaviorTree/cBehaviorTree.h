#pragma once
#include "BaseTasks.h"

class cActor;
class cBlackboard;
class cBehaviorTree : public enable_shared_from_this<cBehaviorTree>
{
public:
	cBehaviorTree(weak_ptr<cActor> actor);
	~cBehaviorTree();

	void Init();

	//트리 순회하며 태스크 검색, 이벤트
	void Run();

	//Running 상태의 태스크 매 틱마다 처리
	void Update();

	void ReadBinary(wstring file);

	weak_ptr<cRootTask> GetRoot() const;

	bool IsSameCurrentTask(const weak_ptr<cTask>& task)
	{
		return _currentTask.owner_before(task);
	}

	void SetCurrentTask(const weak_ptr<cTask>& task);
private:
	shared_ptr<cRootTask> _root;
	weak_ptr<cActor> _actor;

	weak_ptr<cTask> _currentTask;
};