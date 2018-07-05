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

	void AbsoluteTransform(D3DXMATRIX& transform);

	D3DXMATRIX& Transform();

	D3DXMATRIX& AbsoluteTransform();

	UINT ChildCount() { return _children.size(); }
	shared_ptr<cModelBone> Child(UINT index) { return _children[index]; }

	void ShowHierarchy();

	//test : �ݶ��̴� �׽�Ʈ
	weak_ptr<struct sTransform> GetTransform() const;
	void SetTransform(D3DXMATRIX& matrix);
private:
	// iClonable��(��) ���� ��ӵ�
	virtual unique_ptr<cModelBone> Clone() const override;
private:
	wstring					_name;
	int						_index;
	int						_parentIndex;
	weak_ptr<cModelBone>	_parent;

	vector<shared_ptr<cModelBone>> _children;

	shared_ptr<struct sTransform> _transform;
	shared_ptr<struct sTransform> _absoluteTransform;
	//D3DXMATRIX				_transform;
	//D3DXMATRIX				_absoluteTransform;

	//test : �ݶ��̴� �θ� �׽�Ʈ
	shared_ptr<struct sTransform> _matrix;
};