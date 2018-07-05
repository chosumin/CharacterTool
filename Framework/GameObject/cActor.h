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

	//hack : 유닛 테스트용
	bool isAttack = false;
	DWORD time;
	weak_ptr<cCollider> GetCollider() const;
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
	vector<shared_ptr<cCollider>> _colliders;
	unique_ptr<class cBehaviorTree> _behaviorTree;
};