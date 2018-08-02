#include "stdafx.h"
#include "cModelTool.h"
#include "iObserver.h"
#include "./GameObject/cActor.h"
#include "./Model/cModel.h"
#include "./Model/ModelPart/cModelBone.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/FbxExporter/Exporter.h"
#include "./Model/cModelFactory.h"
#include "./Helper/cPath.h"
#include "./Helper/cString.h"
#include "./UI/Gizmo/cGizmo.h"

UI::cModelTool::cModelTool()
	:_modelName("None")
	,_selectSubTool(false)
{
}

UI::cModelTool::~cModelTool()
{
}

void UI::cModelTool::RegisterObserver(weak_ptr<iObserver> observer)
{
	_observers.emplace_back(observer);
}

void UI::cModelTool::Update()
{
}

void UI::cModelTool::Render()
{
}

void UI::cModelTool::ShowHierarchy(int i)
{
	if (ImGui::TreeNode(to_string(i).c_str()))
	{
		SelectModel();
		if ((AlertActor() || AlertModel()) == false)
		{
			ShowMeshHierarchy();
			ShowBoneHierarchy(_model.lock()->GetRoot());
		}
		ImGui::TreePop();
	}
	else
		SelectModel();
}

void UI::cModelTool::SelectModel()
{
	ImGui::SameLine();
	if (ImGui::Selectable("Model"))
	{
		if (AlertActor())
			return;

		SendMe();
		_selectSubTool = false;
		cDebug::Log("Model Selected!");
	}
}

void UI::cModelTool::ShowMeshHierarchy()
{
	auto modelPtr = _model.lock();
	for (auto&& mesh : modelPtr->GetMeshes())
	{
		string meshName = cString::String(mesh->GetName()).c_str();
		if (ImGui::Selectable(meshName.c_str()))
		{
			cDebug::Log((meshName + " Selected!").c_str());
			_selectSubTool = true;

			auto meshTransform = mesh->GetParentTransform();

			cGizmo::Get()->AddTransform(meshTransform);

			//todo : 메쉬 전달시 주석 해제
			//NotifySelectedMesh(mesh);
		}
	}
}

void UI::cModelTool::ShowBoneHierarchy(weak_ptr<cModelBone> bone)
{
	auto bonePtr = bone.lock();
	auto boneIndex = bonePtr->GetIndex();
	auto boneName = cString::String(bonePtr->GetName());
	auto boneTransform = bonePtr->GetAnimatedTransform();

	if (ImGui::TreeNodeEx(to_string(boneIndex).c_str()))
	{
		SelectBone(boneName, bonePtr);

		for (auto&& bone : bonePtr->GetChildren())
			ShowBoneHierarchy(bone);

		ImGui::TreePop();
	}
	else
		SelectBone(boneName, bonePtr);
}

void UI::cModelTool::SelectBone(string boneName,
								weak_ptr<cModelBone> bone)
{
	ImGui::SameLine();
	if (ImGui::Selectable(boneName.c_str()))
	{
		cDebug::Log((boneName + " Selected!").c_str());
		_selectSubTool = true;

		auto boneTransform = bone.lock()->GetAnimatedTransform();

		cGizmo::Get()->AddTransform(boneTransform);

		//todo : 본 전달시 주석 해제
		NotifySelectedBone(bone);
	}
}

void UI::cModelTool::NotifySelectedBone(weak_ptr<cModelBone> bone)
{
	for (auto&& observer : _observers)
	{
		observer.lock()->SelectBone(bone);
	}
}

void UI::cModelTool::NotifySelectedMesh(weak_ptr<cModelMesh> mesh)
{
	for (auto&& observer : _observers)
	{
		observer.lock()->SelectMesh(mesh);
	}
}

void UI::cModelTool::ShowInspector()
{
	ShowModelInspector();

	if (_selectSubTool)
		ShowTransformInspector();
}

void UI::cModelTool::ShowTransformInspector()
{
	if (ImGui::CollapsingHeader("Transform"))
		cGizmo::Get()->PostRender();
}

void UI::cModelTool::ShowModelInspector()
{
	if (ImGui::CollapsingHeader("Model"))
	{
		ImGui::TextDisabled(_modelName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			//todo : 이미 모델이 존재하면 팝업 띄워서 물어보기
			OpenFile();
			NotifyChangeModel();
		}

		ShowMeshInfo();
		//todo : 머티리얼 정보 표시하기
	}
}

void UI::cModelTool::OpenFile()
{
	bool openFbx = false;
	cPath::OpenFileDialog(L"Open Model", cPath::ModelFilter, Asset, [this](wstring path)
	{
		auto extension = cPath::GetExtension(path);

		//확장자가 fbx면 export 후에 불러오기
		if (extension == L"fbx" || extension == L"FBX")
			ExportFbx(path, &path);

		OpenModel(cPath::GetDirectoryName(path),
				  cPath::GetFileNameWithoutExtension(path));
	});
}

void UI::cModelTool::NotifyChangeModel()
{
	for (auto&& observer : _observers)
	{
		observer.lock()->ChangeModel(_model);
	}
}

void UI::cModelTool::ExportFbx(wstring path, OUT wstring *newPath)
{
	Fbx::Exporter exporter{ path };

	cPath::SaveFileDialog(cPath::GetFileNameWithoutExtension(path), cPath::MaterialFilter, Model, [&exporter, &newPath](wstring savePath)
	{
		exporter.ExportMaterial(cPath::GetDirectoryName(savePath),
								cPath::GetFileName(savePath));

		exporter.ExportMesh(cPath::GetDirectoryName(savePath),
							cPath::GetFileName(savePath));

		//추출한 새 경로를 반환
		*newPath = savePath + L".material";
	});

	cDebug::Log("Fbx Export Success!");
}

void UI::cModelTool::OpenModel(wstring path, wstring name)
{
	//todo : 모델 팩토리 수정
	shared_ptr<cModel> model = cModelFactory::Create(path, name);

	_model = model;
	_actor.lock()->SetModel(model);

	_modelName = cString::String(name);
	cDebug::Log((_modelName + " Open!").c_str());
}

void UI::cModelTool::ShowMeshInfo()
{
	if (_model.expired())
		return;

	ImGui::Separator();

	auto meshes = _model.lock()->GetMeshes();
	for (auto&& mesh : meshes)
	{
		//todo : 메쉬 정보에 추가할 것들 여기에 삽입
		ImGui::Text(cString::String(mesh->GetName()).c_str());
	}
}

bool UI::cModelTool::AlertModel()
{
	if (_model.expired())
	{
		cDebug::Log("Add Model First!");
		return true;
	}

	return false;
}

void UI::cModelTool::SaveJson(Json::Value& root)
{
}

void UI::cModelTool::LoadJson(Json::Value& root)
{
}
