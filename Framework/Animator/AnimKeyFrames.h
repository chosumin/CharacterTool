#pragma once

struct sAnimKeyFrameData
{
	D3DXMATRIX Transform;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Translation;
	D3DXVECTOR3 Scale;
};

struct sAnimKeyFrame
{
	wstring BoneName;
	vector<sAnimKeyFrameData> FrameData;
};