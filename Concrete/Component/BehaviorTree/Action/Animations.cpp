#include "stdafx.h"
#include "Animations.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./Helper/cString.h"
#include "./Blackboard/cBlackboard.h"
#include "./Transform/sTransform.h"
#include "./Viewer/cCameraFactory.h"
#include "./Helper/cMath.h"

cSingleAnimation::cSingleAnimation(const weak_ptr<cActor> & actor,
								   const weak_ptr<cBehaviorTree>& tree,
								   const ImVec2& position,
								   weak_ptr<cTask> parent)
	: cTask(tree, "Animate One", position, parent)
	, _actor(actor)
	, _selectedClip("")
	, _overwrite(false)
	, _blendTime(0.2f)
	, _isLoop(true)
{
}

cSingleAnimation::~cSingleAnimation()
{
}

std::unique_ptr<cTask> cSingleAnimation::Clone() const
{
	auto task = make_unique<cSingleAnimation>(_actor, _tree, _pos);

	task->_clip = _clip;
	task->_overwrite = _overwrite;
	task->_blendTime = _blendTime;
	task->_isLoop = _isLoop;
	task->_selectedClip = _selectedClip;

	return move(task);
}

cTask::eState cSingleAnimation::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	auto& animatorPtr = actorPtr->GetAnimator();
	auto& blackboardPtr = actorPtr->GetBlackboard();

	bool str = _selectedClip == "Thug_Walk_Forward.anim";
	if (_state == eState::NONE)
	{
		animatorPtr->SetCurrentClip(_clip, _overwrite, _blendTime, _isLoop);
		blackboardPtr->SetBool("Animate", true);
		_tree.lock()->SetCurrentTask(shared_from_this());
		_state = eState::RUNNING;
	}
	
	if (_state == eState::RUNNING)
	{
		//todo : 이벤트 프레임 도달시 이벤트 실행
		if (animatorPtr->IsEndAnimation())
		{
			blackboardPtr->SetBool("Animate", false);
			_state = eState::SUCCESS;
		}
		//todo : 애니메이션 완료시 석세스, 미완료시 Running 
	}

	return _state;
}

void cSingleAnimation::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto& clips = actorPtr->GetAnimator()->GetClips();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	ImGui::Text("Animation");
	if (ImGui::BeginCombo("", _selectedClip.c_str()))
	{
		for (auto&& clip : clips)
		{
			string clipName = cString::String(clip->GetName());
			bool isSelected = _selectedClip == clipName;
			if (ImGui::Selectable(clipName.c_str(), isSelected))
			{
				_selectedClip = clipName;
				_clip = clip;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	ImGui::PushID(1);
	ImGui::Text("Blending Time :");
	ImGui::InputFloat("", &_blendTime);
	ImGui::PopID();

	ImGui::Checkbox("Is Loop", &_isLoop);
	ImGui::Checkbox("Overwrite", &_overwrite);
	//todo : 석세스 넣을지 여부
}

void cSingleAnimation::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Clip Name", _selectedClip);

	if (_selectedClip != "")
	{
		auto actorPtr = _actor.lock();
		auto& animPtr = actorPtr->GetAnimator();

		wstring clipName = cString::Wstring(_selectedClip);
		for (auto&& clip : animPtr->GetClips())
		{
			if (clip->GetName() == clipName)
			{
				_clip = clip;
				break;
			}
		}
	}

	Json::GetValue(root, "Overwrite", _overwrite);
	Json::GetValue(root, "Blend Time", _blendTime);
	Json::GetValue(root, "Is Loop", _isLoop);
}

void cSingleAnimation::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	string clipName = "";
	auto clipPtr = _clip.lock();
	if (clipPtr)
		clipName = cString::String(clipPtr->GetName());
	Json::SetValue(task, "Clip Name", clipName);

	Json::SetValue(task, "Overwrite", _overwrite);
	Json::SetValue(task, "Blend Time", _blendTime);
	Json::SetValue(task, "Is Loop", _isLoop);

	root.append(task);
}

/*******************************************************************/

unordered_map<cMultiAnimation::eDirection, cMultiAnimation::ClipStruct> cMultiAnimation::_clips;

cMultiAnimation::cMultiAnimation(const weak_ptr<cActor> & actor,
								   const weak_ptr<cBehaviorTree>& tree,
								   const ImVec2& position,
								   weak_ptr<cTask> parent)
	: cTask(tree, "Animate Multi", position, parent)
	, _actor(actor)
	, _overwrite(false)
	, _blendTime(0.2f)
{
	static bool init = true;
	if (init)
	{
		weak_ptr<cAnimClip> clip;
		_clips[eDirection::LEFT] = ClipStruct{ "", clip };
		_clips[eDirection::RIGHT] = ClipStruct{ "", clip };
		_clips[eDirection::FORWARD] = ClipStruct{ "", clip };
		_clips[eDirection::BACKWARD] = ClipStruct{ "", clip };
		init = false;
	}
}

cMultiAnimation::~cMultiAnimation()
{
}

std::unique_ptr<cTask> cMultiAnimation::Clone() const
{
	auto task = make_unique<cMultiAnimation>(_actor, _tree, _pos);

	for (auto&& clip : _clips)
	{
		task->_clips[clip.first] = clip.second;
	}

	task->_overwrite = _overwrite;
	task->_blendTime = _blendTime;

	return move(task);
}

cTask::eState cMultiAnimation::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	auto& animatorPtr = actorPtr->GetAnimator();
	auto& blackboardPtr = actorPtr->GetBlackboard();

	if (_state == eState::NONE)
	{				
		//방향 결정
		const auto& targetDir = blackboardPtr->GetVector3("TargetDirection");
		
		const auto& transformPtr = actorPtr->GetTransform();
		D3DXVECTOR3 actorDir;
		transformPtr->GetDirection(actorDir);

		//두 벡터 내적하여 각도 계산
		float angle = cMath::AngleBetweenTwoVectors(targetDir,
													actorDir);

		//외적하여 좌우 판단
		D3DXVECTOR3 temp;
		D3DXVec3Cross(&temp, &targetDir, &actorDir);
		if (temp.y < 0)
			angle = -angle;

		cDebug::RenewalLog("angle : %f", angle);

		//방향 결정
		eDirection eDir = eDirection::FORWARD;
		eDir = angle > 30 ? eDirection::LEFT : eDir;
		eDir = angle < -30 ? eDirection::RIGHT : eDir;
		eDir = abs(angle) > 110.0f ? eDirection::BACKWARD : eDir;

		//방향 결정된 클립 세팅
		const auto& clip = _clips[eDir].Clip;
		animatorPtr->SetCurrentClip(clip, _overwrite, _blendTime);
		_tree.lock()->SetCurrentTask(shared_from_this());
		_state = eState::RUNNING;

		//블랙보드에 전달
		blackboardPtr->SetBool("Animate", true);
	}
	
	if (_state == eState::RUNNING)
	{
		//todo : 이벤트 프레임 도달시 이벤트 실행
		if (animatorPtr->IsEndAnimation())
		{
			_state = eState::SUCCESS;
			blackboardPtr->SetBool("Animate", false);
		}
		//todo : 애니메이션 완료시 석세스, 미완료시 Running 
	}

	return _state;
}

void cMultiAnimation::RenderInfo()
{
	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	auto actorPtr = _actor.lock();
	auto& animClips = actorPtr->GetAnimator()->GetClips();

	int id = 0;
	for (auto&& clip : _clips)
	{
		ImGui::Text(GetDirectionText(clip.first).c_str());

		ImGui::PushID(id++);
		if (ImGui::BeginCombo("", clip.second.ClipName.c_str()))
		{
			for (auto&& animClip : animClips)
			{
				string clipName = cString::String(animClip->GetName());

				bool isSelected = clip.second.ClipName == clipName;
				if (ImGui::Selectable(clipName.c_str(), isSelected))
				{
					clip.second.ClipName = clipName;
					clip.second.Clip = animClip;
				}
				ImGui::SetItemDefaultFocus();
			}
			ImGui::EndCombo();
		}
		ImGui::PopID();
	}

	ImGui::PushID(id++);
	ImGui::Text("Blending Time :");
	ImGui::InputFloat("", &_blendTime);
	ImGui::PopID();

	ImGui::Checkbox("Overwrite", &_overwrite);
	//todo : 석세스 넣을지 여부
}

void cMultiAnimation::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	for (auto&& clip : _clips)
	{
		auto& name = clip.second.ClipName;
		Json::GetValue(root, GetDirectionText(clip.first), name);

		if (name != "")
		{
			auto actorPtr = _actor.lock();
			auto& animPtr = actorPtr->GetAnimator();

			wstring clipName = cString::Wstring(name);
			for (auto&& animClip : animPtr->GetClips())
			{
				if (animClip->GetName() == clipName)
				{
					clip.second.Clip = animClip;
					break;
				}
			}
		}
	}

	Json::GetValue(root, "Overwrite", _overwrite);
	Json::GetValue(root, "Blend Time", _blendTime);
}

void cMultiAnimation::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	for (auto&& clip : _clips)
	{
		string clipName = "";
		auto clipPtr = clip.second.Clip.lock();
		if (clipPtr)
			clipName = cString::String(clipPtr->GetName());

		Json::SetValue(task,
					   GetDirectionText(clip.first).c_str(),
					   clipName);
	}

	Json::SetValue(task, "Overwrite", _overwrite);
	Json::SetValue(task, "Blend Time", _blendTime);

	root.append(task);
}

string cMultiAnimation::GetDirectionText(eDirection direction)
{
	switch (direction)
	{
		case cMultiAnimation::LEFT:
			return "Left";
		case cMultiAnimation::RIGHT:
			return "Right";
		case cMultiAnimation::FORWARD:
			return "Forward";
		case cMultiAnimation::BACKWARD:
			return "Backward";
	}

	return "";
}
