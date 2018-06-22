#pragma once
#include "./Interface/iClonable.h"

class cModelBone;
class cModelMeshPart;
class cModelMesh : public iClonable<cModelMesh>
{
private:
	friend class cModel;
	friend class cModelFactory;
public:
	cModelMesh();
	~cModelMesh();
private:
	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelMesh> Clone() const override;
private:
	/*******************
		Getter Setter
	********************/
	wstring				GetName() const { return _name; }
	int					GetParentBoneIndex() const { return _parentBoneIndex; }

	void SetWorld(const D3DXMATRIX& world) { _worldBuffer->SetMatrix(world); }
private:
	void Binding();
	void Render();
private:
	wstring _name;

	int _parentBoneIndex;

	unique_ptr<cWorldBuffer> _worldBuffer;

	vector<shared_ptr<cModelMeshPart>> _meshParts;
};