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

	//test : °úÁ¦¿ë
	void Idle();
	void Rotate(float deltaY);
	void Move(D3DXVECTOR3 direction, float velocity);
	void Attack();
	void Damage();

	bool isAttack = false;
	bool isDamage = false;
	DWORD time;
	weak_ptr<cCollider> GetCollider(UINT num) const;
	void PostRender(int i);
	void ControlTransform(string name, weak_ptr<struct sTransform> transform);
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

	void GetAction(eAction actionType);
	void SetAction(eAction actionType, function<void()> func);

	UINT GetCurrentAnim() const;
private:
	cActor();
private:
	wstring _name;
	shared_ptr<sTransform> _transform;
	shared_ptr<cModel> _model;
	shared_ptr<cAnimator> _animator;
	unordered_map<eAction, function<void()>> _actions;
	unique_ptr<class cBehaviorTree> _behaviorTree;

	shared_ptr<cActorColliders> _colliders;
};