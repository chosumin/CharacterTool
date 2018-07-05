#pragma once
#include "stdafx.h"

namespace Fbx
{
	struct FbxMaterial
	{
		string Name;

		D3DXCOLOR Ambient; //ȯ�汤
		D3DXCOLOR Diffuse; //Ȯ�걤
		D3DXCOLOR Specular; //�ݻ籤
		D3DXCOLOR Emissive; //��絵

		float Shininess;

		string DiffuseFile;
		string SpecularFile;
		string EmissiveFile;
		string NormalFile;
	};

	struct FbxVertex
	{
		int ControlPoint;
		string MaterialName;

		ModelVertexType Vertex;
	};

	struct FbxMeshPartData
	{
		string MaterialName;

		vector<ModelVertexType> Vertices;
		vector<UINT> Indices;
	};

	struct FbxMeshData
	{
		string Name;
		int ParentBone;
		
		FbxMesh *Mesh;

		vector<shared_ptr<FbxVertex>> Vertices;
		vector<shared_ptr<FbxMeshPartData>> MeshParts;
	};

	struct FbxBoneData
	{
		int Index;
		string Name;

		int Parent;
		D3DXMATRIX Transform;
		D3DXMATRIX AbsoluteTransform;

		//todo : Ȯ��
		D3DXVECTOR3 Scale = { 1,1,1 };
		D3DXVECTOR3 Rotation = { 0,0,0 };
		D3DXVECTOR3 Translation = { 0,0,0 };
		D3DXQUATERNION Quaternion;
	};

	struct FbxKeyFrameData
	{
		//todo : transform�� �ʿ��� ���ϱ�
		D3DXMATRIX Transform;
		D3DXQUATERNION Rotation;
		D3DXVECTOR3 Translation;
		D3DXVECTOR3 Scale;
	};

	struct FbxKeyFrame
	{
		string BoneName;
		vector<FbxKeyFrameData> Transform;
	};

	struct FbxAnimation
	{
		string Name;

		int TotalFrame;
		float FrameRate;

		vector<unique_ptr<FbxKeyFrame>> KeyFrames;
	};
}