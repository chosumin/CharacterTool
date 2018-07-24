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
public:
	/*******************
		Getter Setter
	********************/

	int GetIndex() { return _index; }

	int GetParentIndex() { return _parentIndex; }

	wstring GetName() { return _name; }

	weak_ptr<cModelBone> GetParent() { return _parent; }

	UINT GetChildCount() { return _children.size(); }
	weak_ptr<cModelBone> GetChild(UINT index) { return _children[index]; }
	vector<weak_ptr<cModelBone>>& GetChildren();

	weak_ptr<sTransform> GetAbsoluteTransform();
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelBone> Clone() const override;
private:
	wstring					_name;
	int						_index;
	int						_parentIndex;
	weak_ptr<cModelBone>	_parent;

	shared_ptr<sTransform> _transform;
	shared_ptr<sTransform> _absoluteTransform;

	vector<weak_ptr<cModelBone>> _children;
};