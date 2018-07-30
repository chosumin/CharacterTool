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
		_animator = actorPtr->GetAnimator();
}

void UI::cAnimTool::Render()
{
	//DO NOTHING
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
	cPath::OpenFileDialog(L"Open Animation Clip", cPath::AnimFbxFilter, Asset, [this](wstring path)
	{
		auto extension = cPath::GetExtension(path);

		//확장자가 fbx면 export 후에 불러오기
		if (extension == L"fbx" || extension == L"FBX")
			ExportFbx(path, &path);

		wstring& directory = cPath::GetDirectoryName(path);
		wstring& name = cPath::GetFileName(path);
		auto clip = cAnimClip::Create(directory, name);

		if (clip->IsCorrectBones(_model.lock()->GetBones()))
			_animator.lock()->AddClip(clip);
		else
			//todo : 팝업 띄우기
			cDebug::Log("Unfitting Bones!");
	});
}

string UI::cAnimTool::GetClipName()
{
	auto clipPtr = _selectedClip.lock();
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

	ImGui::SliderInt("Frame", &currentCount, 0, lastCount);
	
	if (ImGui::Button("PAUSE"))
	{
		if (animPtr)
			animPtr->SetMode(cAnimator::Mode::PAUSE);
	}
	ImGui::SameLine();
	if (ImGui::Button("PLAY"))
	{
		if (animPtr)
			animPtr->SetMode(cAnimator::Mode::PLAY);
	}
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
	_animator.lock()->SetModel(_model);
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

void UI::cAnimTool::ShowAnimationList()
{
	auto animPtr = _animator.lock();

	for (const auto& clip : animPtr->GetClips())
	{
		//클립 선택
		if (ImGui::Selectable(cString::String(clip->GetName()).c_str()))
		{
			_selectedClip = clip;
			animPtr->SetCurrentClip(clip);
			cDebug::Log((cString::String(clip->GetName()) + " Selected!").c_str());
		}
		
		ImGui::SameLine();

		if (ImGui::Button("Delete"))
		{
			//hack : clip 순회 중인데 중간에 clip 삭제함
			animPtr->DeleteClip(clip);
		}
	}
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