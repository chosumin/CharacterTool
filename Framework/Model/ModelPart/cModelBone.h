#pragma once
#include "./Interface/iCloneable.h"

struct sTransform;
class cModelBone : public iCloneable<cModelBone>
{
private:
	friend class cModel;
	friend class cModelFactory;
public:
	cModelBone();
	~cModelBone();
	
	void Animate(const D3DXMATRIX& animated, const D3DXMATRIX* root);
public:
	/*******************
		Getter Setter
	********************/

	int GetIndex() { return _index; }

	int GetParentIndex() { return _parentIndex; }

	const wstring& GetName() { return _name; }

	const weak_ptr<cModelBone> & GetParent() { return _parent; }

	UINT GetChildCount() { return _children.size(); }
	const weak_ptr<cModelBone> & GetChild(UINT index) { return _children[index]; }
	vector<weak_ptr<cModelBone>>& GetChildren()
	{
		return _children;
	}

	const shared_ptr<sTransform> & GetAbsoluteTransform() const
	{
		return _absoluteTransform;
	}

	const D3DXMATRIX& GetInvAbsoluteMatrix() const
	{
		return _invAbsoluteMatrix;
	}
	const shared_ptr<sTransform> & GetAnimatedTransform() const
	{
		return _animatedTransform;
	}

	const D3DXMATRIX& GetSkinnedMatrix() const;

	const vector<shared_ptr<cCollider>>& GetColliders() const
	{
		return _colliders;
	}

	void AddCollider(weak_ptr<cCollider> collider);
	void DeleteCollider(weak_ptr<cCollider> collider);
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelBone> Clone() const override;
private:
	wstring					_name;
	int						_index;
	int						_parentIndex;
	weak_ptr<cModelBone>	_parent;

	D3DXMATRIX				_invAbsoluteMatrix;
	shared_ptr<sTransform>	_absoluteTransform;
	shared_ptr<sTransform>	_animatedTransform;
	shared_ptr<sTransform>	_skinnedTransform;

	vector<weak_ptr<cModelBone>> _children;

	vector<shared_ptr<cCollider>> _colliders;
};