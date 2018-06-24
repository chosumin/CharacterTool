#include "stdafx.h"
#include "ModelKeyFrames.h"

sModelKeyFrame::sModelKeyFrame()
{
}

sModelKeyFrame::~sModelKeyFrame()
{
}

std::unique_ptr<sModelKeyFrame> sModelKeyFrame::Clone() const
{
	auto frame = make_unique<sModelKeyFrame>();

	frame->BoneName = BoneName;

	for (const auto& temp : FrameData)
	{
		sModelKeyFrameData data;
		data.Rotation = temp.Rotation;
		data.Transform = temp.Transform;
		data.Translation = temp.Translation;
		data.Scale = temp.Scale;

		frame->FrameData.push_back(move(data));
	}
	return move(frame);
}
