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

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void ComboBox(string& data);
private:
	weak_ptr<cBlackboard> _blackboard;

	string _boolParam;
	bool _boolean;

	int _compareType; //비교 대상 - 입력 값, 블랙보드 값
	string _compareParam; //비교할 블랙보드 변수 이름

	bool _isSame; //두 변수가 같으면 true, 아니면 false
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

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	void ComboBox(string& data);
private:
	weak_ptr<cBlackboard> _blackboard;

	string _valueParam; //데이터 이름

	int _type; //데이터 타입 float, vector3, int
	D3DXVECTOR3 _compareVec3;
	float _compareFloat;
	int _compareInt;
	int _inequalitySign; //부등호 >, >=, ==, !=, <=, <

	int _compareType; //비교 대상 - 입력 값, 블랙보드 값
	string _compareParam; //비교할 블랙보드 변수 이름
};

/******************************************************************/

class cPercentageCondition : public cTask
{

};