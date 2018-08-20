#include "stdafx.h"
#include "Actions.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./Helper/cString.h"

//cAttackAction::cAttackAction(weak_ptr<cActor> actor)
//	:cTask(typeid(this).name()),
//	_actor(actor)
//{
//}
//
//cAttackAction::~cAttackAction()
//{
//}
//
//bool cAttackAction::Run()
//{
//	_actor.lock()->Attack();
//	return true;
//}

/******************************************************************/

cAnimationOnly::cAnimationOnly(weak_ptr<cActor> actor,
							   weak_ptr<cBehaviorTree> tree,
							   const ImVec2& position)
	: cTask(tree, "AnimationOnly", position)
	, _actor(actor)
	, _selectedClip("")
{
	
}

cAnimationOnly::~cAnimationOnly()
{
}

std::unique_ptr<cTask> cAnimationOnly::Clone() const
{
	auto task = make_unique<cAnimationOnly>(_actor, _tree, _pos);

	task->_clip = _clip;
	task->_loop = _loop;
	task->_selectedClip = _selectedClip;

	return move(task);
}

bool cAnimationOnly::Run()
{
	auto actorPtr = _actor.lock();
	if(!actorPtr)
		return false;

	actorPtr->GetAnimator().lock()->SetCurrentClip(_clip);
	
	auto treePtr = _tree.lock();
	if (treePtr)
		treePtr->SetCurrentTask(shared_from_this());

	return true;
}

void cAnimationOnly::Update()
{
	//애니메이션만 실행
}

void cAnimationOnly::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

	ImGui::TextColored(ImVec4(250, 250, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	if(ImGui::BeginCombo("", _selectedClip.c_str()))
	{
		int i = 0;
		for (auto&& clip : clips)
		{
			ImGui::PushID(i++);

			string clipName = cString::String(clip->GetName());
			bool isSelected = _selectedClip == clipName;
			if (ImGui::Selectable(clipName.c_str(), isSelected))
			{
				_selectedClip = clipName;
				_clip = clip;
			}
			ImGui::SetItemDefaultFocus();

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}
}