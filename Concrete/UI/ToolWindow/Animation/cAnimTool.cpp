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
	: _modelTool(modelTool)
	, _eventName("")
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
	}
}

void UI::cAnimTool::Render()
{
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
	UINT index = 0;
	if (ImGui::CollapsingHeader("Animator"))
		ShowAnimatorInspector(index);
	
	//Animation List
	if (ImGui::CollapsingHeader("Animation List"))
		ShowAnimationList();
}

void UI::cAnimTool::ShowAnimatorInspector(UINT& index)
{
	//�ִϸ��̼� �ӵ� ����
	SetAnimationSpeed(index);

	ImGui::Text("Clip : ");
	ImGui::SameLine();
	ImGui::Text(GetClipName().c_str());
	
	ShowProgressInspector();

	ImGui::Separator();

	ShowEventFrameList(index);

	ImGui::Separator();
}

void UI::cAnimTool::SetAnimationSpeed(UINT& index)
{
	ImGui::PushID(index++);
	ImGui::Text("Animation Speed :");
	ImGui::SameLine();

	float speed = 0.0f;

	auto animPtr = _animator.lock();
	if (animPtr)
	{
		speed = animPtr->GetAnimSpeed();
		ImGui::InputFloat("", &speed);
		animPtr->SetAnimSpeed(speed);
	}
	else
		ImGui::InputFloat("", &speed);

	ImGui::PopID();
}

void UI::cAnimTool::OpenAnimationClip()
{
	cPath::OpenMultiFileDialog(L"Open Animation Clip", cPath::AnimFbxFilter, Asset, [this](wstring path)
	{
		auto extension = cPath::GetExtension(path);

		//Ȯ���ڰ� fbx�� export �Ŀ� �ҷ�����
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

		//������ �� ��θ� ��ȯ
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
		//todo : �˾� ����
		cDebug::Log("Unfitting Bones!");
}

string UI::cAnimTool::GetClipName()
{
	if (!_animator.expired())
	{
		auto clipPtr = _animator.lock()->GetCurrentClip().lock();

		if (clipPtr)
			return cString::String(clipPtr->GetName());
	}
	return "None";
}

void UI::cAnimTool::ShowProgressInspector()
{
	//����, �Ͻ�����, ���α׷��� ��
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

void UI::cAnimTool::ShowEventFrameList(UINT& index)
{
	ImGui::Text("Event Frame List :");

	auto clipPtr = _selectedClip.lock();
	if (!clipPtr)
		return;

	//�̺�Ʈ ����Ʈ �߰�
	ImGui::PushID(index++);
	ImGui::InputText("", _eventName, IM_ARRAYSIZE(_eventName));
	ImGui::SameLine();
	ImGui::PopID();

	if (ImGui::Button("Add"))
	{
		auto clipPtr = _selectedClip.lock();
		auto animPtr = _animator.lock();
		wstring eventName = cString::Wstring(_eventName);
		clipPtr->AddEventFrame(eventName, animPtr->GetCurrentFrameCount());
	}

	//�̺�Ʈ ����Ʈ �����ֱ�
	auto& eventFrames = clipPtr->GetEventFrames();
	for (auto iter = eventFrames.begin(); iter != eventFrames.end();)
	{
		ImGui::PushID(index);
		string name = cString::String(iter->first);
		string title = to_string(iter->second) + " " + name;

		//Ŭ���� ����
		if (ImGui::Selectable(title.c_str(), true))
			iter = eventFrames.erase(iter);
		else
		{
			iter++;
			index++;
		}
		ImGui::PopID();
	}
}

void UI::cAnimTool::ShowAnimationList()
{
	if (ImGui::Button("Clip Add"))
	{
		if (Alert())
			return;

		//Ŭ�� �߰�
		OpenAnimationClip();
	}

	ImGui::Separator();

	auto animPtr = _animator.lock();
	if (!animPtr)
		return;

	int index = 1;
	auto iter = animPtr->GetClips().begin();
	for (; iter != animPtr->GetClips().end(); index++)
	{
		ImGui::PushID(index);

		//Ŭ�� ����
		if (ImGui::Selectable(cString::String((*iter)->GetName()).c_str(), false, 0, ImVec2(185, 20)))
		{
			_selectedClip = *iter;
			animPtr->SetCurrentClip(*iter);
		}

		ImGui::SameLine();

		if (ImGui::Button("Del"))
		{
			//hack : clip ��ȸ ���ε� �߰��� clip ������
			iter = animPtr->DeleteClip(*iter);
		}
		else
			iter++;

		ImGui::Separator();
		ImGui::PopID();
	}
}

void UI::cAnimTool::SaveJson(Json::Value& root)
{
	Json::Value animations;

	auto animPtr = _animator.lock();

	//�ִϸ����Ϳ��� animSpeed ����
	float speed = animPtr->GetAnimSpeed();
	Json::SetValue(animations, "Animation Speed", speed);

	Json::Value clips;
	for (auto&& clip : animPtr->GetClips())
	{
		Json::Value animation;

		string path = cString::String(clip->GetFilePath());
		string name = cString::String(clip->GetName());

		string relative = cPath::GetRelativePath(path + name, cPath::TOP_FOLDERNAME_CHAR);

		//��� ����
		Json::SetValue(animation, "Path", relative);
		
		Json::Value eventFrames;
		for (auto&& frame : clip->GetEventFrames())
		{
			Json::Value eventFrame;
			string name = cString::String(frame.first);
			Json::SetValue(eventFrame, "Event Name", name);
			Json::SetValue(eventFrame, "Frame", frame.second);

			eventFrames.append(eventFrame);
		}
		animation["Event Frames"] = eventFrames;

		clips.append(animation);
	}
	animations["Clips"] = clips;
	
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
	//todo : ��ƼŬ�� ���� ��
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
