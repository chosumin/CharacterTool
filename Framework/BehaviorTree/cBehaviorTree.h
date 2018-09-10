#pragma once
#include "BaseTasks.h"
#include "./Interface/iFile.h"

class cActor;
class cBlackboard;
class cBehaviorTree :
	public enable_shared_from_this<cBehaviorTree>,
	public iFile
{
public:
	cBehaviorTree(weak_ptr<cActor> actor);
	~cBehaviorTree();

	void Init();

	//Ʈ�� ��ȸ�ϸ� �½�ũ �˻�, �̺�Ʈ
	void Run();

	//Running ������ �½�ũ �� ƽ���� ó��
	void Update();

	void ReadBinary(wstring file);

	weak_ptr<cRootTask> GetRoot() const;

	void SetCurrentTask(const weak_ptr<cTask>& task);

	const wstring& GetDefaultName() const { return _defaultName; }
	const wstring& GetName() const { return _name; }
	void SetName(const wstring& name) { _name = name; }

	//iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	wstring _defaultName;
	wstring _name;
	shared_ptr<cRootTask> _root;
	weak_ptr<cActor> _actor;

	weak_ptr<cTask> _currentTask;
};