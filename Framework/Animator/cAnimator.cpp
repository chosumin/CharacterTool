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
{
	_current.Init();
	_next.Init();
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
	
	//���� Ŭ���� ������ ���翡 ����
	if (_current.Clip.expired())
	{
		_current.Clip = clip;
		_current.Init();
		_current.IsLoop = isLoop;
		_blendTime = blendTime;
	}
	else
	{
		_interpolationTime = 0.0f;

		UINT curCount = _current.CurrentKeyFrame;

		//���� Ŭ�� ������ ������ �о����
		if (!_next.Clip.expired())
			ShiftClip();

		_next.Clip = clip;
		_next.Init();
		_next.IsLoop = isLoop;

		//������� ��
		if (!overwrite && _current.Clip.lock() == clip.lock())
		{
			_next.CurrentKeyFrame = curCount;
			_elapsedTime = _blendTime;
		}

		//���� �ִϸ��̼� ����� �ٷ� ��ü
		if (IsEndAnimation())
			ShiftClip();
	}
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

	assert(false && "�����Ϸ��� Ŭ���� �������� �ʽ��ϴ�!");
	return _clips.begin();
}

bool cAnimator::IsEndAnimation()
{
	auto curClipPtr = _current.Clip.lock();
	if (curClipPtr)
	{
		return _current.CurrentKeyFrame >= curClipPtr->GetTotalFrame() - 1;// -_blendTime * curClipPtr->GetFrameRate();
	}

	return false;
}

const vector<shared_ptr<cAnimClip>>& cAnimator::GetClips() const
{
	return _clips;
}

int cAnimator::GetCurrentFrameCount()
{
	int lastFrame = GetLastFrameCount();

	return min(_current.CurrentKeyFrame, lastFrame);
}

void cAnimator::SetCurrentFrameCount(int count)
{
	_current.CurrentKeyFrame = count;
}

int cAnimator::GetLastFrameCount()
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
		_next.Init();
		_next.Clip.reset();

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

void cAnimator::UpdateTime()
{
	if (_mode == Mode::PAUSE)
		return;

	//���� Ŭ�� ������
	auto curClipPtr = _current.Clip.lock();
	if (!curClipPtr)
		return;

	_frameTime += cFrame::Delta();
	_invFrameRate = 1 / curClipPtr->GetFrameRate(); //�� �����Ӵ� �ð�

	if (_frameTime > _invFrameRate)
	{
		//���� Ű������ ����
		IncreaseKeyFrame(_current.CurrentKeyFrame,
						 curClipPtr->GetTotalFrame(),
						 _current.IsLoop);
		_frameTime = 0.0f;
	}

	//���� Ŭ�� ������
	auto nextClipPtr = _next.Clip.lock();
	if (!nextClipPtr)
		return;

	CalculateBlendTime();

	//���� ������ ����
	if (_frameTime <= 0.0f)
	{
		IncreaseKeyFrame(_next.CurrentKeyFrame,
						 nextClipPtr->GetTotalFrame(),
						 _next.IsLoop);
	}
}

void cAnimator::IncreaseKeyFrame(OUT int & curFrame, int totalFrame,
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
		//���� �ð� �ٵ����� ������ ����� ��ü
		ShiftClip();
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
	D3DXMATRIX localAnim;

	//���� Ŭ�� ������ ����, �ƴϸ� ���� Ŭ�� �״�� ����
	auto& nextClipPtr = _next.Clip.lock();
	if (nextClipPtr)
		Blend(size, modelPtr, curClipPtr, nextClipPtr);
	else
		GetCurrentClipMatrix(size, modelPtr, curClipPtr);
}

void cAnimator::Blend(UINT size, shared_ptr<cModel>& modelPtr,
					  shared_ptr<cAnimClip>& curClipPtr,
					  shared_ptr<cAnimClip>& nextClipPtr)
{
	D3DXMATRIX localAnim;

	//������ ��� ����
	D3DXMATRIX S, R;
	D3DXVECTOR3 s[2], t[2];
	D3DXQUATERNION q[2];
	for (UINT i = 0; i < size; i++)
	{
		auto& bone = modelPtr->GetBone(i).lock();

		if (curClipPtr->IsCorrectKeyFrame(bone->GetName()) == false)
			return;

		curClipPtr->GetKeyFrameSRT(bone->GetName(),
								   _current.CurrentKeyFrame,
								   t[0], q[0], s[0]);

		nextClipPtr->GetKeyFrameSRT(bone->GetName(),
									_next.CurrentKeyFrame,
									t[1], q[1], s[1]);

		D3DXVec3Lerp(&s[0], &s[0], &s[1], _interpolationTime);
		D3DXVec3Lerp(&t[0], &t[0], &t[1], _interpolationTime);
		D3DXQuaternionSlerp(&q[0], &q[0], &q[1], _interpolationTime);

		D3DXMatrixScaling(&S, s[0].x, s[0].y, s[0].z);
		D3DXMatrixRotationQuaternion(&R, &q[0]);

		localAnim = S * R;
		localAnim._41 = t[0].x, localAnim._42 = t[0].y, localAnim._43 = t[0].z;

		//��Ʈ�� ��� ����(in-place)
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

	//���� Ŭ�� ������ ��� ����
	for (UINT i = 0; i < size; i++)
	{
		//frame : �� ���� 220������, �߰��ϸ� 90������
		auto& bone = modelPtr->GetBone(i).lock();

		//frame : 40 ������ ����
		//��ġ�ϴ� ������ Ȯ��
		if (curClipPtr->IsCorrectKeyFrame(bone->GetName()) == false)
			return;

		//frame : 30 ������ ����
		curClipPtr->GetFrameTransform(localAnim, bone->GetName(), _current.CurrentKeyFrame);

		//��Ʈ�� ��� ����(in-place)
		if (i == 0)
		{
			localAnim._41 = 0;
			localAnim._43 = 0;
		}

		//=> 10 ������ ����
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
	_current.Clip = _next.Clip;
	_current.CurrentKeyFrame = _next.CurrentKeyFrame;
	_current.IsLoop = _next.IsLoop;
	_next.Clip.reset();
}
