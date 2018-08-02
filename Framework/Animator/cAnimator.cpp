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
	, _currentKeyFrame(0)
	, _nextKeyFrame(0)
	, _frameTime(0.0f)
	, _keyFrameFactor(0.0f)
{
}

cAnimator::~cAnimator()
{
}

void cAnimator::Update()
{
	if (_currentClip.expired() || _mode == Mode::STOP)
		return;

	UpdateTime();
	UpdateBones();
}

void cAnimator::SetCurrentClip(weak_ptr<cAnimClip> clip)
{
	_currentClip = clip;
}

bool cAnimator::Duplicated(weak_ptr<cAnimClip> clip)
{
	auto clipPtr = clip.lock();

	for (auto&& clip : _clips)
	{
		if (clip == clipPtr)
			return true;
	}

	return false;
}

void cAnimator::AddClip(weak_ptr<cAnimClip> clip)
{
	_clips.emplace_back(clip.lock());
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

const vector<shared_ptr<cAnimClip>>& cAnimator::GetClips() const
{
	return _clips;
}

int cAnimator::GetCurrentFrameCount()
{
	int lastFrame = GetLastFrameCount();

	return min(_currentKeyFrame, lastFrame);
}

int cAnimator::GetLastFrameCount()
{
	auto clipPtr = _currentClip.lock();

	if (clipPtr == false)
		return 0;
	
	return clipPtr->GetTotalFrame() - 1;
}

cAnimator::Mode cAnimator::GetMode()
{
	if (_currentClip.expired())
		return Mode::STOP;

	return _mode;
}

void cAnimator::SetMode(Mode mode)
{
	if (mode == Mode::STOP)
	{
		_currentKeyFrame = 0;
		_nextKeyFrame = 0;
		_keyFrameFactor = 0.0f;

		_model.lock()->ResetBones();
	}

	_mode = mode;
}

void cAnimator::SetModel(weak_ptr<cModel> model)
{
	_model = model;
}

void cAnimator::UpdateTime()
{
	if (_mode == Mode::PAUSE)
		return;

	_frameTime += cFrame::Delta();

	auto clipPtr = _currentClip.lock();
	
	if (!clipPtr)
		return;

	//프레임 당 시간
	float invFrameRate = 1.0f / clipPtr->GetFrameRate();
	while (_frameTime > invFrameRate)
	{
		int keyFrameCount = clipPtr->GetTotalFrame();

		_currentKeyFrame = (_currentKeyFrame + 1) % keyFrameCount;

		_nextKeyFrame = (_currentKeyFrame + 1) % keyFrameCount;

		_frameTime -= invFrameRate;
	}

	//지금 프레임부터 다음 프레임까지의 비율
	_keyFrameFactor = _frameTime / invFrameRate;
}

void cAnimator::UpdateBones()
{
	auto modelPtr = _model.lock();
	if (modelPtr == false)
		return;
	
	for (UINT i = 0; i < modelPtr->GetBoneCount(); i++)
	{
		auto bone = modelPtr->GetBone(i).lock();

		auto clipPtr = _currentClip.lock();
		if (clipPtr == false)
			return;

		if (clipPtr->IsCorrectKeyFrame(bone->GetName()) == false)
			return;

		//키프레임 애니메이션 계산
		D3DXMATRIX localAnim;
		clipPtr->Interpolate(&localAnim,
							 bone->GetName(),
							 _keyFrameFactor,
							 _currentKeyFrame,
							 _nextKeyFrame);

		modelPtr->AnimateBone(i, localAnim);
	}
}
