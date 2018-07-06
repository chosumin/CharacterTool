#pragma once

enum class eAction
{
	Idle, Move
};

class cCollider;
class cActor
	: public enable_shared_from_this<cActor>
{
private:
	friend class cActorBuilder;
public:
	~cActor();

	void Update();
	void Render();

	void Idle();
	void Rotate(float deltaY);
	void Move(D3DXVECTOR3 direction, float velocity);
	void Attack();
	void Damage();

	//test : 과제용
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

	weak_ptr<struct sTransform> GetTransform() const;

	void GetAction(eAction actionType);
	void SetAction(eAction actionType, function<void()> func);

	UINT GetCurrentAnim() const;
private:
	cActor();
private:
	shared_ptr<struct sTransform> _transform;
	shared_ptr<class cModel> _model;
	unique_ptr<class cModelAnimPlayer> _anim;
	unordered_map<eAction, function<void()>> _actions;
	unique_ptr<class cBehaviorTree> _behaviorTree;

	//todo : 피격, 공격 구분하기
	vector<shared_ptr<cCollider>> _colliders;
};