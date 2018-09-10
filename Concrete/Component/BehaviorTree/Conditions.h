#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;

class cBoolCondition : public cTask
{
public:
	cBoolCondition(weak_ptr<cBlackboard> blackboard,
				   weak_ptr<cBehaviorTree> tree,
				   const ImVec2& position,
				   const weak_ptr<cTask>& parent = weak_ptr<cTask>());
	~cBoolCondition();

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
	weak_ptr<cBlackboard> _blackboard;

	string _boolParam;
	bool _boolean;

	int _compareType; //�� ��� - �Է� ��, ������ ��
	string _compareParam; //���� ������ ���� �̸�

	bool _isSame; //�� ������ ������ true, �ƴϸ� false
};

/******************************************************************/

class cValueCompareCondition : public cTask
{
public:
	cValueCompareCondition(weak_ptr<cBlackboard> blackboard,
						   weak_ptr<cBehaviorTree> tree,
						   const ImVec2& position,
						   const weak_ptr<cTask>& parent = weak_ptr<cTask>());
	~cValueCompareCondition();

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
	weak_ptr<cBlackboard> _blackboard;

	string _valueParam; //������ �̸�

	int _type; //������ Ÿ�� float, vector3, int
	D3DXVECTOR3 _compareVec3;
	float _compareFloat;
	int _compareInt;
	int _inequalitySign; //�ε�ȣ >, >=, ==, !=, <=, <

	int _compareType; //�� ��� - �Է� ��, ������ ��
	string _compareParam; //���� ������ ���� �̸�
};

/******************************************************************/

class cPercentageCondition : public cTask
{

};