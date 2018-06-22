#include "stdafx.h"
#include "FbxUtility.h"

D3DXVECTOR2 Fbx::Utility::ToVector2(FbxVector2 & vec)
{
	return D3DXVECTOR2((float)vec.mData[0], (float)vec.mData[1]);
}

D3DXVECTOR3 Fbx::Utility::ToVector3(FbxVector4 & vec)
{
	return D3DXVECTOR3((float)vec.mData[0], (float)vec.mData[1], (float)vec.mData[2]);
}

D3DXCOLOR Fbx::Utility::ToColor(FbxVector4 & vec)
{
	return D3DXCOLOR((float)vec.mData[0], (float)vec.mData[1], (float)vec.mData[2], 1);
}

D3DXCOLOR Fbx::Utility::ToColor(FbxPropertyT<FbxDouble3>& vec, FbxPropertyT<FbxDouble>& factor)
{
	FbxDouble3 color = vec;

	D3DXCOLOR result;
	result.r = (float)color.mData[0];
	result.g = (float)color.mData[1];
	result.b = (float)color.mData[2];
	result.a = (float)factor;

	return result;
}

D3DXMATRIX Fbx::Utility::ToMatrix(FbxAMatrix & matrix)
{
	FbxVector4 r1 = matrix.GetRow(0);
	FbxVector4 r2 = matrix.GetRow(1);
	FbxVector4 r3 = matrix.GetRow(2);
	FbxVector4 r4 = matrix.GetRow(3);

	return D3DXMATRIX
	(
		(float)r1.mData[0], (float)r1.mData[1], (float)r1.mData[2], (float)r1.mData[3],
		(float)r2.mData[0], (float)r2.mData[1], (float)r2.mData[2], (float)r2.mData[3],
		(float)r3.mData[0], (float)r3.mData[1], (float)r3.mData[2], (float)r3.mData[3],
		(float)r4.mData[0], (float)r4.mData[1], (float)r4.mData[2], (float)r4.mData[3]
	);
}

string Fbx::Utility::GetTextureFile(FbxProperty & prop)
{
	if (prop.IsValid() == true)
	{
		//prop�� ��Ҹ� ������ �ִ��� Ȯ��
		if (prop.GetSrcObjectCount() > 0)
		{
			auto texture = prop.GetSrcObject<FbxFileTexture>();

			if (texture != nullptr)
				return string(texture->GetFileName());
		}
	}
	return "";
}

string Fbx::Utility::GetMaterialName(FbxMesh * mesh, int cpIndex)
{
	int result = -1;

	auto material = mesh->GetLayer(0)->GetMaterials();
	if (material == nullptr)
		return "";

	auto mappingMode = material->GetMappingMode();

	//��� ��ġ�� ��
	if (mappingMode == FbxLayerElement::eAllSame)
		result = material->GetIndexArray().GetAt(0);
	//cp�� ����� ���͸��� ��ȣ ��ġ
	else
		result = material->GetIndexArray().GetAt(cpIndex);

	if (result < 0)
		return "";

	FbxNode* node = mesh->GetNode();
	return node->GetMaterial(result)->GetName();
}

D3DXVECTOR2 Fbx::Utility::GetUv(FbxMesh * mesh, int cpIndex, int uvIndex)
{
	D3DXVECTOR2 result{ 0.0f,0.0f };

	auto uv = mesh->GetLayer(0)->GetUVs();
	if (uv == nullptr)
		return result;

	auto mappingMode = uv->GetMappingMode();
	//��ȣ�� ���������� ��������, �����ߴ���
	auto refMode = uv->GetReferenceMode();
	switch (mappingMode)
	{
		//uv���� ��Ʈ�� ����Ʈ�� ��Ī�� ����
		case FbxLayerElement::eByControlPoint:
		{
			//FbxLayerElement:::eDirect���� ���� �˻�
			if (refMode == FbxLayerElement::eDirect)
			{
				result.x = (float)uv->GetDirectArray().GetAt(cpIndex).mData[0];
				result.y = (float)uv->GetDirectArray().GetAt(cpIndex).mData[1];
			}
			else if (refMode == FbxLayerElement::eIndexToDirect)
			{
				int index = uv->GetIndexArray().GetAt(cpIndex);

				result.x = (float)uv->GetDirectArray().GetAt(index).mData[0];
				result.y = (float)uv->GetDirectArray().GetAt(index).mData[1];
			}
		}
		break;
		//�ش� ������ ��ȣ�� uv��ȣ
		case FbxLayerElement::eByPolygonVertex:
		{
			//���۷��� ��� ������� ����
			result.x = (float)uv->GetDirectArray().GetAt(uvIndex).mData[0];
			result.y = (float)uv->GetDirectArray().GetAt(uvIndex).mData[1];
		}
		break;
	}

	//����!
	result.y = 1.0f - result.y;

	return result;
}
