#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cAnimClip;
struct sTransform;
class cSingleAnimation : public cTask
{
public:
	cSingleAnimation(const weak_ptr<cActor> & actor,
					 const weak_ptr<cBehaviorTree> & tree,
					 const ImVec2& position,
					 weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cSingleAnimation();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	weak_ptr<cActor> _actor;
	weak_ptr<cAnimClip> _clip;
	bool _overwrite;
	float _blendTime;
	bool _isLoop;
	string _selectedClip;
};

/******************************************************************/

class cMultiAnimation : public cTask
{
private:
	enum eDirection
	{
		LEFT, RIGHT, FORWARD, BACKWARD
	};

	struct ClipStruct
	{
		string ClipName = "";
		weak_ptr<cAnimClip> Clip;

		ClipStruct() {}
		ClipStruct(const string& name, weak_ptr<cAnimClip> clip)
			:ClipName(name), Clip(clip) {}
	};
public:
	cMultiAnimation(const weak_ptr<cActor> & actor,
					 const weak_ptr<cBehaviorTree> & tree,
					 const ImVec2& position,
					 weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cMultiAnimation();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile 상속
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	string GetDirectionText(eDirection direction);
private:
	weak_ptr<cActor> _actor;
	bool _overwrite;
	float _blendTime;
private:
	static unordered_map<eDirection, ClipStruct> _clips;
};