#pragma once
#include "./Interface/iClonable.h"

class cModelBone : public iClonable<cModelBone>
{
public:
	cModelBone();
	~cModelBone();
private:
	friend class cModel;
	friend class cModelFactory;
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelBone> Clone() const override;
private:
	wstring					_name;
	int						_index;
	int						_parentIndex;
	weak_ptr<cModelBone>	_parent;
	D3DXMATRIX				_transform;
};