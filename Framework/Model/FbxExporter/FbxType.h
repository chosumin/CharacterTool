#pragma once
#include "stdafx.h"

namespace Fbx
{
	struct FbxMaterial
	{
		string Name;

		D3DXCOLOR Ambient; //È¯°æ±¤
		D3DXCOLOR Diffuse; //È®»ê±¤
		D3DXCOLOR Specular; //¹Ý»ç±¤
		D3DXCOLOR Emissive; //¹æ»çµµ

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
	};

	struct FbxKeyFrameData
	{
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