#pragma once

namespace Fbx
{
	class Utility
	{
	public:
		static D3DXVECTOR2 ToVector2(FbxVector2& vec);
		static D3DXVECTOR3 ToVector3(FbxVector4& vec);

		static D3DXQUATERNION ToQuaternion(FbxQuaternion& q);

		static D3DXCOLOR ToColor(FbxVector4& vec);
		static D3DXCOLOR ToColor(FbxPropertyT<FbxDouble3>& vec, FbxPropertyT<FbxDouble>& factor);
		static D3DXMATRIX ToMatrix(FbxAMatrix& matrix);

		static string GetTextureFile(FbxProperty& prop);
		static string GetMaterialName(FbxMesh* mesh, int polygonIndex, int cpIndex);
		static D3DXVECTOR2 GetUv(FbxMesh* mesh, int cpIndex, int uvIndex);
	};
}