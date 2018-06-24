#pragma once
#include "./Interface/iCloneable.h"

struct sModelKeyFrameData
{
	D3DXMATRIX Transform;
	D3DXQUATERNION Rotation;
	D3DXVECTOR3 Translation;
	D3DXVECTOR3 Scale;
};

struct sModelKeyFrame : public iCloneable<sModelKeyFrame>
{
	sModelKeyFrame();
	~sModelKeyFrame();

	wstring BoneName;
	vector<sModelKeyFrameData> FrameData;

	// iCloneable��(��) ���� ��ӵ�
	virtual std::unique_ptr<sModelKeyFrame> Clone() const override;
};