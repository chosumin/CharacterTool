#pragma once

enum class eAction
{
	Idle, Move
};

struct sTransform;
class cCollider;
class cModel;
class cActorColliders;
class cActor
	: public enable_shared_from_this<cActor>
{
private:
	friend class cActorBuilder;
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
	weak_ptr<class cModel> GetModel() const;
	weak_ptr<struct sTransform> GetTransform() const;
	weak_ptr<cActorColliders> GetColliders() const;

	void GetAction(eAction actionType);
	void SetAction(eAction actionType, function<void()> func);

	UINT GetCurrentAnim() const;

	void SetModel(weak_ptr<cModel> model);
private:
	cActor();
private:
	shared_ptr<sTransform> _transform;
	shared_ptr<cModel> _model;
	unique_ptr<class cModelAnimPlayer> _anim;
	unordered_map<eAction, function<void()>> _actions;
	unique_ptr<class cBehaviorTree> _behaviorTree;

	shared_ptr<cActorColliders> _colliders;
};