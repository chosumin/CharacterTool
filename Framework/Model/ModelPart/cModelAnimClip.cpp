#include "stdafx.h"
#include "cModelAnimClip.h"
#include "ModelKeyFrames.h"

cModelAnimClip::cModelAnimClip()
	:_defaultFrameRate(0.0f)
{
}

cModelAnimClip::~cModelAnimClip()
{
	_keyFrames.clear();
}

sModelKeyFrame * cModelAnimClip::GetKeyframe(wstring name)
{
	return _keyFrames[name].get();
}

std::unique_ptr<cModelAnimClip> cModelAnimClip::Clone() const
{
	auto clip = make_unique<cModelAnimClip>();
	clip->_name = _name;

	clip->_totalFrame = _totalFrame;
	clip->_frameRate = _frameRate;
	clip->_defaultFrameRate = _defaultFrameRate;

	for (auto&& key : _keyFrames)
	{
		auto temp = key.second->Clone();

		clip->_keyFrames[temp->BoneName] = move(temp);
	}

	return move(clip);
}
