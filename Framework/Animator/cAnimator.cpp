#include "stdafx.h"
#include "cAnimator.h"
#include "cAnimClip.h"
#include "./Model/cModel.h"
#include "./Model/ModelPart/cModelBone.h"
#include "./Transform/sTransform.h"
#include "./Helper/cMath.h"

cAnimator::cAnimator(weak_ptr<cModel> model)
	: _model(model)
	, _mode(Mode::STOP)
	, _blendTime(0.0f)
	, _interpolationTime(0.0f)
	, _elapsedTime(0.0f)
	, _frameTime(0.0f)
	, _animSpeed(1.0f)
{
	_current.Init();
	_prev.Init();
}

cAnimator::~cAnimator()
{
}

void cAnimator::Update()
{
	if (_current.Clip.expired() || _mode == Mode::STOP)
		return;

	UpdateTime();
	UpdateBones();
}

void cAnimator::SetCurrentClip(const weak_ptr<cAnimClip> & clip, bool overwrite, float blendTime, bool isLoop)
{
	_mode = Mode::PLAY;
	
	//현재 클립이 없으면 현재에 세팅
	if (_current.Clip.expired())
	{
		_current.Clip = clip;
		_current.Init();
		_current.IsLoop = isLoop;
		_blendTime = blendTime;
	}
	else //현재 클립이 있음
	{
		_interpolationTime = 0.0f;

		UINT curCount = _current.CurrentKeyFrame;

		//클립 이동
		ShiftClip();

		_current.Clip = clip;
		_current.Init();
		_current.IsLoop = isLoop;

		//덮어쓰기일 때
		if (!overwrite && _prev.Clip.lock() == clip.lock())
		{
			_current.CurrentKeyFrame = curCount;
			_elapsedTime = _blendTime;
		}
	}
}

bool cAnimator::Duplicated(const weak_ptr<cAnimClip> & clip)
{
	auto clipPtr = clip.lock();

	for (auto&& clip : _clips)
	{
		if (clip == clipPtr)
			return true;
	}

	return false;
}

void cAnimator::AddClip(const shared_ptr<cAnimClip> & clip)
{
	_clips.emplace_back(clip);
}

cAnimator::ClipIter cAnimator::DeleteClip(weak_ptr<cAnimClip> clip)
{
	for (auto iter = _clips.begin(); iter != _clips.end(); iter++)
	{
		if (*iter == clip.lock())
			return _clips.erase(iter);
	}

	assert(false && "삭제하려는 클립이 존재하지 않습니다!");
	return _clips.begin();
}

bool cAnimator::IsEndAnimation()
{
	auto curClipPtr = _current.Clip.lock();
	if (curClipPtr)
	{
		return _current.CurrentKeyFrame >= curClipPtr->GetTotalFrame() - 1 -_blendTime * curClipPtr->GetFrameRate();
	}

	return false;
}

const vector<shared_ptr<cAnimClip>>& cAnimator::GetClips() const
{
	return _clips;
}

UINT cAnimator::GetCurrentFrameCount()
{
	UINT lastFrame = GetLastFrameCount();

	return min(_current.CurrentKeyFrame, lastFrame);
}

void cAnimator::SetCurrentFrameCount(UINT count)
{
	_current.CurrentKeyFrame = count;
}

UINT cAnimator::GetLastFrameCount()
{
	auto clipPtr = _current.Clip.lock();

	if (clipPtr == false)
		return 0;
	
	return clipPtr->GetTotalFrame() - 1;
}

cAnimator::Mode cAnimator::GetMode()
{
	if (_current.Clip.expired())
		return Mode::STOP;

	return _mode;
}

void cAnimator::SetMode(Mode mode)
{
	if (mode == Mode::STOP)
	{
		_current.Init();
		_prev.Init();
		_prev.Clip.reset();

		_model.lock()->ResetBones();
	}

	_mode = mode;
}

void cAnimator::SetModel(weak_ptr<cModel> model)
{
	_model = model;
}

const weak_ptr<cAnimClip>& cAnimator::GetCurrentClip() const
{
	return _current.Clip;
}

float cAnimator::GetAnimSpeed() const
{
	return _animSpeed;
}

void cAnimator::SetAnimSpeed(float speed)
{
	_animSpeed = speed;
}

void cAnimator::MultiplyAnimSpeed(float multiplied)
{
	_animSpeed = _animSpeed * multiplied;
}

void cAnimator::UpdateTime()
{
	if (_mode == Mode::PAUSE)
		return;

	//현재 클립 포인터
	auto curClipPtr = _current.Clip.lock();
	if (!curClipPtr)
		return;

	_frameTime += cFrame::Delta();
	_invFrameRate = 1 / (curClipPtr->GetFrameRate() * _animSpeed); //한 프레임당 시간

	if (_frameTime > _invFrameRate)
	{
		//현재 키프레임 증가
		IncreaseKeyFrame(_current.CurrentKeyFrame,
						 curClipPtr->GetTotalFrame(),
						 _current.IsLoop);
		_frameTime = 0.0f;
	}

	//이전 클립 포인터
	auto prevClipPtr = _prev.Clip.lock();
	if (!prevClipPtr)
		return;

	CalculateBlendTime();

	//다음 프레임 증가
	if (_frameTime <= 0.0f)
	{
		IncreaseKeyFrame(_prev.CurrentKeyFrame,
						 prevClipPtr->GetTotalFrame(),
						 _prev.IsLoop);
	}
}

void cAnimator::IncreaseKeyFrame(OUT UINT& curFrame, UINT totalFrame,
								 bool isLoop)
{
	if(isLoop)
		curFrame = (curFrame + 1) % totalFrame;
	else
	{
		if (curFrame + 1 < totalFrame)
			++curFrame;
	}
}

void cAnimator::CalculateBlendTime()
{
	_elapsedTime += cFrame::Delta();
	_interpolationTime = _elapsedTime / _blendTime;
	if (_interpolationTime > 1.0f)
	{
		//블렌드 시간 다됐으면 이전 클립 삭제
		DeletePrevClip();
		InitTime();
		return;
	}
}

void cAnimator::UpdateBones()
{
	auto modelPtr = _model.lock();
	if (modelPtr == false)
		return;

	auto& curClipPtr = _current.Clip.lock();
	if (curClipPtr == false)
		return;

	UINT size = modelPtr->GetBoneCount();

	//이전 클립 있으면 블렌딩, 아니면 현재 클립 그대로 전달
	auto& prevClipPtr = _prev.Clip.lock();
	if (prevClipPtr)
		Blend(size, modelPtr, prevClipPtr, curClipPtr);
	else
		GetCurrentClipMatrix(size, modelPtr, curClipPtr);
}

void cAnimator::Blend(UINT size, shared_ptr<cModel>& modelPtr,
					  shared_ptr<cAnimClip>& front,
					  shared_ptr<cAnimClip>& back)
{
	D3DXMATRIX localAnim;

	//블렌드한 행렬 전달
	D3DXMATRIX S, R;
	D3DXVECTOR3 s[2], t[2];
	D3DXQUATERNION q[2];
	for (UINT i = 0; i < size; i++)
	{
		auto& bone = modelPtr->GetBone(i);

		if (front->IsCorrectKeyFrame(bone->GetName()) == false)
			return;

		front->GetKeyFrameSRT(bone->GetName(),
							  _prev.CurrentKeyFrame,
							  t[0], q[0], s[0]);

		back->GetKeyFrameSRT(bone->GetName(),
							 _current.CurrentKeyFrame,
							 t[1], q[1], s[1]);

		D3DXVec3Lerp(&s[0], &s[0], &s[1], _interpolationTime);
		D3DXVec3Lerp(&t[0], &t[0], &t[1], _interpolationTime);
		D3DXQuaternionSlerp(&q[0], &q[0], &q[1], _interpolationTime);

		D3DXMatrixScaling(&S, s[0].x, s[0].y, s[0].z);
		D3DXMatrixRotationQuaternion(&R, &q[0]);

		D3DXMatrixMultiply(&localAnim, &S, &R);
		localAnim._41 = t[0].x, localAnim._42 = t[0].y, localAnim._43 = t[0].z;

		//루트는 경로 고정(in-place)
		if (i == 0)
		{
			localAnim._41 = 0;
			localAnim._43 = 0;
		}

		modelPtr->AnimateBone(i, localAnim);
	}
}

void cAnimator::GetCurrentClipMatrix(UINT size,
									 shared_ptr<cModel>& modelPtr,
									 shared_ptr<cAnimClip>& curClipPtr)
{
	D3DXMATRIX localAnim;

	//현재 클립 프레임 행렬 전달
	for (UINT i = 0; i < size; i++)
	{
		auto& bone = modelPtr->GetBone(i);

		//일치하는 본인지 확인
		if (curClipPtr->IsCorrectKeyFrame(bone->GetName()) == false)
			return;

		curClipPtr->GetFrameTransform(localAnim, bone->GetName(), _current.CurrentKeyFrame);

		//루트는 경로 고정(in-place)
		if (i == 0)
		{
			localAnim._41 = 0;
			localAnim._43 = 0;
		}

		//=> 10 프레임 차이
		modelPtr->AnimateBone(i, localAnim);
	}
}

void cAnimator::InitTime()
{
	//_blendTime = 0.0f;
	_elapsedTime = 0.0f;
	_interpolationTime = 0.0f;
}

void cAnimator::ShiftClip()
{
	_prev.Clip = _current.Clip;
	_prev.CurrentKeyFrame = _current.CurrentKeyFrame;
	_prev.IsLoop = _current.IsLoop;
	_current.Clip.reset();
}

void cAnimator::DeletePrevClip()
{
	_prev.Clip.reset();
	_prev.CurrentKeyFrame = 0;
}
