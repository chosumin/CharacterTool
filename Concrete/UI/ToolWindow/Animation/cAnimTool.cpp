#include "stdafx.h"
#include "cAnimTool.h"
#include "../Model/cModelTool.h"
#include "./GameObject/cActor.h"
#include "./Model/cModel.h"
#include "./Model/FbxExporter/Exporter.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"

UI::cAnimTool::cAnimTool(weak_ptr<cModelTool> modelTool)
	:_modelTool(modelTool)
{
}

UI::cAnimTool::~cAnimTool()
{
}

void UI::cAnimTool::Init()
{
	_modelTool.lock()->RegisterObserver(iObserver::downcasted_shared_from_this<cAnimTool>());
	cTool::Init();
}

void UI::cAnimTool::Update()
{
	auto actorPtr = _actor.lock();
	if (actorPtr)
	{
		_animator = actorPtr->GetAnimator();
		if (!_startGame)
			actorPtr->TestUpdate();
	}
}

void UI::cAnimTool::Render()
{
	if (!_startGame)
	{
		auto actorPtr = _actor.lock();
		if (actorPtr)
			actorPtr->Render();
	}
}

void UI::cAnimTool::ShowHierarchy(int i)
{
	if (ImGui::TreeNode(to_string(i).c_str()))
	{
		SelectAnimator();
		ImGui::TreePop();
	}
	else
		SelectAnimator();
}

void UI::cAnimTool::SelectAnimator()
{
	ImGui::SameLine();
	if (ImGui::Selectable("Animator"))
	{
		SendMe();
		cDebug::Log("Animator Selected!");
	}
}

void UI::cAnimTool::ShowInspector()
{
	//Animation
	if (ImGui::CollapsingHeader("Animator"))
		ShowAnimatorInspector();
	
	//Animation List
	if (ImGui::CollapsingHeader("Animation List"))
		ShowAnimationList();
}

void UI::cAnimTool::ShowAnimatorInspector()
{
	if (ImGui::Button("Add"))
	{
		if (Alert())
			return;

		//클립 추가
		OpenAnimationClip();
	}

	ImGui::Text("Clip : ");
	ImGui::SameLine();
	ImGui::Text(GetClipName().c_str());
	
	ShowProgressInspector();
}

void UI::cAnimTool::OpenAnimationClip()
{
	cPath::OpenMultiFileDialog(L"Open Animation Clip", cPath::AnimFbxFilter, Asset, [this](wstring path)
	{
		auto extension = cPath::GetExtension(path);

		//확장자가 fbx면 export 후에 불러오기
		if (extension == L"fbx" || extension == L"FBX")
			ExportFbx(path, &path);

		wstring& directory = cPath::GetDirectoryName(path);
		wstring& name = cPath::GetFileName(path);
		CreateClip(directory, name);
	});
}

void UI::cAnimTool::ExportFbx(wstring path, OUT wstring * newPath)
{
	Fbx::Exporter exporter{ path };

	cPath::SaveFileDialog(cPath::GetFileNameWithoutExtension(path), L"Animation\0*.anim", Model, [&exporter, &newPath](wstring savePath)
	{
		exporter.ExportAnimation(cPath::GetDirectoryName(savePath),
								 cPath::GetFileName(savePath));

		//추출한 새 경로를 반환
		*newPath = savePath + L".anim";
	});

	cDebug::Log("Fbx Export Success!");
}

void UI::cAnimTool::CreateClip(const wstring & directory, const wstring & name)
{
	auto clip = cAnimClip::Create(directory, name);

	if (clip->IsCorrectBones(_model.lock()->GetBones()))
	{
		auto animPtr = _animator.lock();
		if (animPtr->Duplicated(clip))
			cDebug::Log("Duplicated Clip!");
		else
			animPtr->AddClip(clip);
	}
	else
		//todo : 팝업 띄우기
		cDebug::Log("Unfitting Bones!");
}

string UI::cAnimTool::GetClipName()
{
	auto clipPtr = _animator.lock()->GetCurrentClip().lock();

	if (clipPtr)
		return cString::String(clipPtr->GetName());

	return "None";
}

void UI::cAnimTool::ShowProgressInspector()
{
	//시작, 일시정지, 프로그레스 바
	auto animPtr = _animator.lock();
	
	int currentCount, lastCount;
	if (animPtr)
	{
		currentCount = animPtr->GetCurrentFrameCount();
		lastCount = animPtr->GetLastFrameCount();
	}
	else
		currentCount = lastCount = 0;

	if (ImGui::SliderInt("Frame", &currentCount, 0, lastCount))
		animPtr->SetCurrentFrameCount(currentCount);

	if (ImGui::Button("PLAY"))
		animPtr->SetMode(cAnimator::Mode::PLAY);
	ImGui::SameLine();
	if (ImGui::Button("PAUSE"))
		animPtr->SetMode(cAnimator::Mode::PAUSE);
	ImGui::SameLine();
	if (ImGui::Button("STOP"))
		animPtr->SetMode(cAnimator::Mode::STOP);
}

void UI::cAnimTool::ShowAnimationList()
{
	auto animPtr = _animator.lock();
	if (!animPtr)
		return;

	int index = 1;
	auto iter = animPtr->GetClips().begin();
	for (; iter != animPtr->GetClips().end(); index++)
	{
		//클립 선택
		if (ImGui::Selectable(cString::String((*iter)->GetName()).c_str(), true, 0, ImVec2(150, 20)))
		{
			_selectedClip = *iter;
			animPtr->SetCurrentClip(*iter);
			cDebug::Log((cString::String((*iter)->GetName()) + " Selected!").c_str());
		}

		ImGui::SameLine();

		if (ImGui::Button(("Delete " + to_string(index)).c_str()))
		{
			//hack : clip 순회 중인데 중간에 clip 삭제함
			iter = animPtr->DeleteClip(*iter);
		}
		else
			iter++;

		ImGui::Separator();
	}
}

void UI::cAnimTool::SaveJson(Json::Value& root)
{
	Json::Value animations;

	auto animPtr = _animator.lock();

	for (auto&& clip : animPtr->GetClips())
	{
		string path = cString::String(clip->GetFilePath());
		string name = cString::String(clip->GetName());
		animations.append(path + name);
	}

	root["Animations"] = animations;
}

void UI::cAnimTool::LoadJson()
{
	//DO NOTHING
}

void UI::cAnimTool::SetActor(weak_ptr<cActor> actor)
{
	_actor = actor;
	_animator = _actor.lock()->GetAnimator();
}

void UI::cAnimTool::SelectBone(weak_ptr<cModelBone> bone)
{
	//todo : 파티클을 붙일 본
}

void UI::cAnimTool::SelectMesh(weak_ptr<cModelMesh> mesh)
{
	//DO NOTHING
}

void UI::cAnimTool::ChangeModel(weak_ptr<cModel> newModel)
{
	_model = newModel;
	_animator.lock()->SetModel(newModel);
}

bool UI::cAnimTool::Alert()
{
	if (AlertActor())
		return true;

	if (_model.expired())
	{
		cDebug::Log("Add Model First!");
		return true;
	}

	return false;
}
