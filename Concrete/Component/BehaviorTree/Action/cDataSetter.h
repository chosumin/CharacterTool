#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;
class cDataSetter : public cTask
{
public:
	cDataSetter(const weak_ptr<cActor> & actor,
				const weak_ptr<cBehaviorTree>& tree,
				const ImVec2& position,
				weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cDataSetter();

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void ComboBox(string& data);
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cBlackboard> _blackboard;

	enum class eDataType
	{
		BOOL, FLOAT, VECTOR3, INT
	}_type;

	string _paramName;

	bool _compareBool;
	float _compareFloat;
	D3DXVECTOR3 _compareVector3;
	int _compareInt;
};
