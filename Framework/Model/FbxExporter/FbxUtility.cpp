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

D3DXQUATERNION Fbx::Utility::ToQuaternion(FbxQuaternion & q)
{
	return D3DXQUATERNION{ (float)q.mData[0],(float)q.mData[1] ,(float)q.mData[2] ,(float)q.mData[3] };
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
		//prop이 요소를 가지고 있는지 확인
		if (prop.GetSrcObjectCount() > 0)
		{
			auto texture = prop.GetSrcObject<FbxFileTexture>();

			if (texture != nullptr)
				return string(texture->GetFileName());
		}
	}
	return "";
}

string Fbx::Utility::GetMaterialName(FbxMesh * mesh, int polygonIndex, int cpIndex)
{
	auto node = mesh->GetNode();
	if (node == nullptr)
		return "";

	auto material = mesh->GetLayer(0)->GetMaterials();
	if (material == nullptr)
		return "";

	auto mappingMode = material->GetMappingMode();
	auto refMode = material->GetReferenceMode();

	int mappingIndex = -1;
	switch (mappingMode)
	{
		//머터리얼 번호가 모두 일치
		case FbxLayerElement::eAllSame: mappingIndex = 0; break;
		//폴리곤에 의해서 인덱스가 정해진다
		case FbxLayerElement::eByPolygon: mappingIndex = polygonIndex; break;
		//컨트롤 포인트와 일치
		case FbxLayerElement::eByControlPoint: mappingIndex = cpIndex; break;
			//p * 3 + vi으로 vi가 들어가야하지만 폴리곤 하나당 머터리얼은 모두 같으므로 굳이 정점번호까지 더할 필요 없다.
		case FbxLayerElement::eByPolygonVertex: mappingIndex = polygonIndex * 3; break;
		default: assert(false); break;
	}

	FbxSurfaceMaterial *findMaterial = nullptr;
	//직접 매핑
	if (refMode == FbxLayerElement::eDirect)
	{
		if (mappingIndex < node->GetMaterialCount())
			findMaterial = node->GetMaterial(mappingIndex);
	}
	//인덱스를 통해 매핑한다.
	else if (refMode == FbxLayerElement::eIndexToDirect)
	{
		//머터리얼 번호를 가진 배열
		FbxLayerElementArrayTemplate<int>& indexArray = material->GetIndexArray();

		//머터리얼 번호보다 작아야함
		if (mappingIndex < indexArray.GetCount())
		{
			int tempIndex = indexArray.GetAt(mappingIndex);

			if (tempIndex < node->GetMaterialCount())
				findMaterial = node->GetMaterial(tempIndex);
		}
	}

	if (findMaterial == nullptr) return "";

	return findMaterial->GetName();
}

D3DXVECTOR2 Fbx::Utility::GetUv(FbxMesh * mesh, int cpIndex, int uvIndex)
{
	D3DXVECTOR2 result{ 0.0f,0.0f };

	auto uv = mesh->GetLayer(0)->GetUVs();
	if (uv == nullptr)
		return result;

	auto mappingMode = uv->GetMappingMode();
	//번호를 직접적으로 가졌는지, 참조했는지
	auto refMode = uv->GetReferenceMode();
	switch (mappingMode)
	{
		//uv값이 컨트롤 포인트와 매칭된 상태
		case FbxLayerElement::eByControlPoint:
		{
			//FbxLayerElement:::eDirect으로 구글 검색
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
		//해당 정점의 번호가 uv번호
		case FbxLayerElement::eByPolygonVertex:
		{
			//레퍼런스 모드 관계없이 동일
			result.x = (float)uv->GetDirectArray().GetAt(uvIndex).mData[0];
			result.y = (float)uv->GetDirectArray().GetAt(uvIndex).mData[1];
		}
		break;
	}

	//주의!
	result.y = 1.0f - result.y;

	return result;
}
