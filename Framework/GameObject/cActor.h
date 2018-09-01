#pragma once

enum class eAction
{
	Idle, Move
};

struct sTransform;
class cCollider;
class cModel;
class cActorColliders;
class cAnimator;
class cBehaviorTree;
class cBlackboard;
class cActor
	: public enable_shared_from_this<cActor>
{
private:
	friend class cActorFactory;
public:
	~cActor();

	void Init();
	void Update();
	void Render();
public:
	/*******************
		Getter Setter
	********************/
	const wstring& GetName() { return _name; }
	void SetName(wstring&& name) { _name = name; }

	weak_ptr<class cModel> GetModel() const;
	void SetModel(weak_ptr<cModel> model);

	weak_ptr<struct sTransform> GetTransform() const;

	weak_ptr<cActorColliders> GetColliders() const;

	weak_ptr<cAnimator> GetAnimator() const;

	weak_ptr<cBehaviorTree> GetBehaviorTree() const;

	void GetAction(eAction actionType);
	void SetAction(eAction actionType, function<void()> func);

	UINT GetCurrentAnim() const;
	
	weak_ptr<cBlackboard> GetBlackboard() const 
	{ return _blackboard; }
private:
	cActor();
private:
	wstring _name;
	shared_ptr<sTransform> _transform;
	shared_ptr<cModel> _model;
	shared_ptr<cAnimator> _animator;
	shared_ptr<cBlackboard> _blackboard;
	shared_ptr<cBehaviorTree> _behaviorTree;
	unordered_map<eAction, function<void()>> _actions;

	shared_ptr<cActorColliders> _colliders;
};