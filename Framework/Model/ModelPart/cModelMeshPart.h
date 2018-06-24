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

	// iClonable��(��) ���� ��ӵ�
	virtual unique_ptr<cModelMeshPart> Clone() const override;
private:
	
private:
	void Binding();
	//void Render();
private:
	//todo : �־���ϳ�?
	wstring _materialName;
	weak_ptr<cMaterial> _material;

	ID3D11Buffer *_vb;
	ID3D11Buffer *_ib;

	vector<ModelVertexType> _vertices;
	vector<UINT> _indices;
};