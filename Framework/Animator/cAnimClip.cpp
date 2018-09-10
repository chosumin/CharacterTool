#include "stdafx.h"
#include "cAnimClip.h"
#include "AnimKeyFrames.h"
#include "./Model/cModel.h"
#include "./Model/ModelPart/cModelBone.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"

shared_ptr<cAnimClip> cAnimClip::Create(wstring filePath, wstring fileName)
{
	//애니메이션 캐시
	static unordered_map<wstring, weak_ptr<cAnimClip>> cache;

	struct make_shared_enabler : public cAnimClip {
		make_shared_enabler(wstring filePath, wstring fileName) :cAnimClip(filePath, fileName) {}
	};

	//캐시에서 애니메이션 검색
	auto animPtr = cache[fileName].lock();
	if (animPtr == nullptr)
	{
		animPtr = make_shared<make_shared_enabler>(filePath, fileName);
		animPtr->ReadAnimation(filePath + fileName);
		animPtr->SetName(fileName);

		cache[fileName] = animPtr;
	}

	return animPtr;
}

cAnimClip::~cAnimClip()
{
	_keyFrames.clear();
}

void cAnimClip::GetKeyFrameSRT(const wstring & boneName, UINT index,
							   OUT D3DXVECTOR3 & t,
							   OUT D3DXQUATERNION & q,
							   OUT D3DXVECTOR3 & s)
{
	auto& keyFrame = _keyFrames[boneName]->FrameData[index];
	t = keyFrame.Translation;
	q = keyFrame.Rotation;
	s = keyFrame.Scale;
}

void cAnimClip::GetFrameTransform(OUT D3DXMATRIX& transformMatrix, const wstring& boneName, UINT count)
{
	auto& frames = _keyFrames[boneName]->FrameData;

	if (count >= _totalFrame)
	{
		auto& frame = _keyFrames[boneName]->FrameData[_totalFrame - 1];
		transformMatrix = frame.Transform;
	}
	else
	{
		auto& frame = _keyFrames[boneName]->FrameData[count];
		transformMatrix = frame.Transform;
	}
}

void cAnimClip::Interpolate(OUT D3DXMATRIX * pSRT, const wstring& boneName, float keyFrameFactor, UINT current)
{
	UINT next = (current + 1) % _totalFrame;

	D3DXMATRIX S, R;

	const auto& curFrame = _keyFrames[boneName]->FrameData[current];
	const auto& nextFrame = _keyFrames[boneName]->FrameData[next];

	const D3DXVECTOR3& s1 = curFrame.Scale;
	const D3DXVECTOR3& s2 = nextFrame.Scale;

	D3DXVECTOR3 s;
	D3DXVec3Lerp(&s, &s1, &s2, keyFrameFactor);
	D3DXMatrixScaling(&S, s.x, s.y, s.z);

	const D3DXQUATERNION& q1 = curFrame.Rotation;
	const D3DXQUATERNION& q2 = nextFrame.Rotation;

	D3DXQUATERNION q;
	D3DXQuaternionSlerp(&q, &q1, &q2, keyFrameFactor);
	D3DXMatrixRotationQuaternion(&R, &q);

	const D3DXVECTOR3& t1 = curFrame.Translation;
	const D3DXVECTOR3& t2 = nextFrame.Translation;

	D3DXVECTOR3 t;
	D3DXVec3Lerp(&t, &t1, &t2, keyFrameFactor);

	*pSRT = S * R;
	pSRT->_41 = t.x, pSRT->_42 = t.y, pSRT->_43 = t.z;
}

bool cAnimClip::IsCorrectBones(const vector<shared_ptr<cModelBone>>& bones)
{
	//fixme : 배트맨과 캣우먼 모두 같은 애니메이션으로 동작하는데 어떻게 할지 고민
	/*for (auto&& keyFrame : _keyFrames)
	{
		bool fit = false;
		for (auto&& bone : bones)
		{
			if (keyFrame.first == bone->GetName())
			{
				fit = true;
				continue;
			}
		}

		if (fit == false)
			return false;
	}*/

	return true;
}

bool cAnimClip::IsCorrectKeyFrame(const wstring & boneName)
{
	auto& keyFrame = _keyFrames[boneName];
	if (keyFrame == nullptr)
		return false;

	return true;
}

cAnimClip::cAnimClip(wstring filePath, wstring fileName)
	:_defaultFrameRate(0.0f)
{
}

void cAnimClip::ReadAnimation(wstring file)
{
	shared_ptr<cBinaryReader> r = cBinaryReader::Open(file);
	{
		UINT count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			_name = cString::Wstring(r->String());
			_name = cPath::GetFileName(file);
			_filePath = cPath::GetDirectoryName(file);

			_totalFrame = r->Int();
			_frameRate = r->Float();
			_defaultFrameRate = _frameRate;

			UINT frameCount = r->UInt();
			for (UINT frame = 0; frame < frameCount; frame++)
			{
				auto keyframe = make_unique<sAnimKeyFrame>();
				keyframe->BoneName = cString::Wstring(r->String());

				UINT count = r->UInt();
				keyframe->FrameData.assign(count, sAnimKeyFrameData());

				void* ptr = (void *)&(keyframe->FrameData[0]);
				r->Byte(&ptr, sizeof(sAnimKeyFrameData) * count);

				_keyFrames[keyframe->BoneName] = move(keyframe);
			}//for(frame)
		}
	}
	r->Close();
}
