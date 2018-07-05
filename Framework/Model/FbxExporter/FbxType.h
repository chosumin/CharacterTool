#pragma once
#include "stdafx.h"

namespace Fbx
{
	struct FbxMaterial
	{
		string Name;

		D3DXCOLOR Ambient; //환경광
		D3DXCOLOR Diffuse; //확산광
		D3DXCOLOR Specular; //반사광
		D3DXCOLOR Emissive; //방사도

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

		//todo : 확인
		D3DXVECTOR3 Scale = { 1,1,1 };
		D3DXVECTOR3 Rotation = { 0,0,0 };
		D3DXVECTOR3 Translation = { 0,0,0 };
		D3DXQUATERNION Quaternion;
	};

	struct FbxKeyFrameData
	{
		//todo : transform이 필요한 것일까
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