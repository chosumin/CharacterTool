#pragma once
#include "./Interface/iCloneable.h"

class cMaterial;
class cModelMeshPart : public iCloneable<cModelMeshPart>
{
private:
	friend class cModel;
	friend class cModelMesh;
	friend class cModelFactory;
public:
	cModelMeshPart();
	~cModelMeshPart();

	void Render();

	// iClonable을(를) 통해 상속됨
	virtual unique_ptr<cModelMeshPart> Clone() const override;
private:
	
private:
	void Binding();
	//void Render();
private:
	//todo : 있어야하나?
	wstring _materialName;
	weak_ptr<cMaterial> _material;

	ID3D11Buffer *_vb;
	ID3D11Buffer *_ib;

	vector<ModelVertexType> _vertices;
	vector<UINT> _indices;
};