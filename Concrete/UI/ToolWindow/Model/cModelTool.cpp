#include "stdafx.h"
#include "cModelTool.h"
#include "iObserver.h"
#include "./GameObject/cActor.h"
#include "./Graphic/cMaterial.h"
#include "./Graphic/cTexture.h"
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
	if (_selectSubTool)
		ShowTransformInspector();

	ShowModelInspector();
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
		ImGui::Text("Model Name :"); ImGui::SameLine();
		ImGui::Text(_modelName.c_str());
		if (ImGui::Button("Load Model"))
		{
			//todo : 이미 모델이 존재하면 팝업 띄워서 물어보기
			OpenFile();
		}
	}

	if (ImGui::CollapsingHeader("Material"))
	{
		if (ImGui::Button("Save Material"))
		{
			SaveMaterial();
		}

		//머티리얼 정보를 띄운다.
		ShowMaterials();
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
	shared_ptr<cModel> model = cModelFactory::Create(path, name);

	_model = model;
	_actor.lock()->SetModel(model);

	_modelName = cString::String(name);

	//옵저버에게 모델 변경을 통지
	NotifyChangeModel();

	cDebug::Log((_modelName + " Open!").c_str());
}

void UI::cModelTool::SaveMaterial()
{
	auto modelPtr = _model.lock();
	if (!modelPtr)
	{
		cDebug::Log("Create Model before Saving Material!");
		return;
	}

	Json::Value root;

	for (auto&& material : modelPtr->GetMaterials())
	{
		Json::Value val;

		string name = cString::String(material->GetName());
		Json::SetValue(val, "Name", name);

		string shaderName = cString::String(material->GetShaderName());
		shaderName = cPath::GetFileName(shaderName);
		Json::SetValue(val, "ShaderName", shaderName);

		Json::SetValue(val, "Ambient", *material->GetTextureColor(ColorType::AMBIENT));
		Json::SetValue(val, "Diffuse", *material->GetTextureColor(ColorType::DIFFUSE));
		Json::SetValue(val, "Emissive", *material->GetTextureColor(ColorType::EMISSIVE));
		Json::SetValue(val, "Specular", *material->GetTextureColor(ColorType::SPECULAR));

		Json::SetValue(val, "Shininess", *material->GetTextureColor(ColorType::SHININESS, 0));

		SaveTextureFile(val, "DiffuseFile", material, TextureType::DIFFUSE);
		SaveTextureFile(val, "SpecularFile", material, TextureType::SPECULAR);
		SaveTextureFile(val, "EmissiveFile", material, TextureType::EMISSIVE);
		SaveTextureFile(val, "NormalFile", material, TextureType::NORMAL);

		root[name] = val;
	}
	
	Json::WriteData(modelPtr->GetFilePath() + L".material", &root);

	cDebug::Log("Save Material!");
}

void UI::cModelTool::SaveTextureFile(Json::Value & root, const string & jsonName, const shared_ptr<cMaterial>& material, TextureType type)
{
	string fileName = "";
	auto texture = material->GetTextureMap(type).lock();
	if (texture)
	{
		fileName = cString::String(texture->GetFile());
		fileName = cPath::GetFileName(fileName);
	}
	Json::SetValue(root, jsonName, fileName);
}

void UI::cModelTool::ShowMaterials()
{
	auto modelPtr = _model.lock();
	if (!modelPtr)
		return;

	int index = 0;
	for (auto&& material : modelPtr->GetMaterials())
	{
		ImGui::PushID(index++);
		{
			string matName = cString::String(material->GetName());
			ShowText("Material Name : ", cPath::GetFileName(matName));
			ImGui::NewLine();

			//쉐이더
			string shaderName = cString::String(material->GetShaderName());
			ShowText("Shader Name :", cPath::GetFileName(shaderName));
			ImGui::SameLine();
			if (ImGui::Button("Open"))
			{
				OpenShaderFile(material);
			}

			ShowTextureColors(index, material);
			ImGui::Spacing();
		}
		ImGui::PopID();

		ImGui::Separator();
	}
}

void UI::cModelTool::OpenShaderFile(shared_ptr<cMaterial> material)
{
	cPath::OpenFileDialog(L"Open HLSL File", cPath::ShaderFilter, Shader, [&material](wstring path)
	{
		material->SetShader(path);
	});
}

void UI::cModelTool::ShowTextureColors(int& index, shared_ptr<cMaterial> material)
{
	D3DXCOLOR* color;

	ImGui::PushID(index++);
	{
		ImGui::Text("Ambient :");
		color = material->GetTextureColor(ColorType::AMBIENT);
		ImGui::SliderFloat4("", (*color), 0, 1);
	}
	ImGui::PopID();

	ImGui::PushID(index++);
	{
		ImGui::Text("Diffuse :");
		color = material->GetTextureColor(ColorType::DIFFUSE);
		ImGui::SliderFloat4("", (*color), 0, 1);
	}
	ImGui::PopID();

	ImGui::PushID(index++);
	{
		ImGui::Text("Specular :");
		color = material->GetTextureColor(ColorType::SPECULAR);
		ImGui::SliderFloat4("", (*color), 0, 1);
	}
	ImGui::PopID();

	ImGui::PushID(index++);
	{
		float* shininess;
		ImGui::Text("Normal :");
		shininess = material->GetTextureColor(ColorType::SHININESS, 0);
		ImGui::SliderFloat("", shininess, 0, 10);
	}
	ImGui::PopID();

	auto texturePtr = material->GetTextureMap(TextureType::DIFFUSE).lock();
	ShowTexture(texturePtr);

	texturePtr = material->GetTextureMap(TextureType::SPECULAR).lock();
	ShowTexture(texturePtr);

	texturePtr = material->GetTextureMap(TextureType::NORMAL).lock();
	ShowTexture(texturePtr);
}

void UI::cModelTool::ShowTexture(shared_ptr<cTexture> texturePtr)
{
	if (texturePtr)
	{
		ImGui::Image(texturePtr->GetView(), ImVec2(64, 64));
		ImGui::SameLine();
	}
}

void UI::cModelTool::ShowText(string name, string text)
{
	ImGui::TextColored(ImVec4(255,255,0,255), name.c_str());
	ImGui::Text(text.c_str());
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
	Json::Value model;

	//모델 경로 저장
	Json::Value modelPath;
	
	auto modelPtr = _model.lock();
	if (!modelPtr)
		return;

	wstring relative = cPath::GetRelativePath(modelPtr->GetFilePath(), cPath::TOP_FOLDERNAME_WCHAR);
	Json::SetValue(model, "FilePath", cString::String(relative));

	//머티리얼 쉐이더 파일 저장
	SaveMaterial();

	root["Model"] = model;
}

void UI::cModelTool::LoadJson()
{
	_model = _actor.lock()->GetModel();
	auto path = _model.lock()->GetFilePath();
	path = cPath::GetFileNameWithoutExtension(path);

	_modelName = cString::String(path);

	//todo : 머티리얼 생성

	NotifyChangeModel();
}
