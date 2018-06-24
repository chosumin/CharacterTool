#pragma once
#include "./Interface/iCloneable.h"

class cModelBone : public iCloneable<cModelBone>
{
private:
	friend class cModel;
	friend class cModelFactory;
public:
	cModelBone();
	~cModelBone();

	int GetIndex() { return _index; }
	int GetParentIndex() { return _parentIndex; }

	wstring GetName() { return _name; }

	weak_ptr<cModelBone> GetParent() { return _parent; }

	void GetTransform(D3DXMATRIX& transform);

	void AbsoluteTransform(D3DXMATRIX& transform)
	{
		_absoluteTransform = transform;
	}

	D3DXMATRIX& Transform();

	D3DXMATRIX& AbsoluteTransform()
	{
		return _absoluteTransform;
	}

	UINT ChildCount() { return _children.size(); }
	shared_ptr<cModelBone> Child(UINT index) { return _children[index]; }

	void ShowHierarchy();
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelBone> Clone() const override;
private:
	wstring					_name;
	int						_index;
	int						_parentIndex;
	weak_ptr<cModelBone>	_parent;

	vector<shared_ptr<cModelBone>> _children;

	shared_ptr<struct sTransform> _transform;
	//D3DXMATRIX				_transform;

	D3DXMATRIX				_absoluteTransform;
};