#include "stdafx.h"
#include "cModelTool.h"
#include "./GameObject/cActor.h"
#include "./Model/cModel.h"
#include "./Model/ModelPart/cModelMesh.h"
#include "./Model/FbxExporter/Exporter.h"
#include "./Model/cModelFactory.h"
#include "./Helper/cPath.h"
#include "./Helper/cString.h"

UI::cModelTool::cModelTool()
	:_modelName("None")
{
}

UI::cModelTool::~cModelTool()
{
}

void UI::cModelTool::Update()
{
}

void UI::cModelTool::Render()
{
}

void UI::cModelTool::ShowHierarchy()
{
	if (ImGui::TreeNode(""))
	{
		ShowMeshHierarchy();
		ImGui::TreePop();
	}
	ImGui::SameLine();

	if (ImGui::Selectable("Model"))
	{
		if (AlertActor())
			return;

		SendMe();
		cDebug::Log("Model Selected!");
	}
}

void UI::cModelTool::ShowInspector()
{
	//모델 선택시
	ShowModelInspector();

	//메쉬 선택시
	//모델, 메쉬 인스펙터 모두 띄우기
}

void UI::cModelTool::ShowMeshHierarchy()
{
	if (AlertActor() || AlertModel())
		return;

	auto meshes = _model.lock()->GetMeshes();
	/*for(auto&& mesh : meshes)
		mesh->*/
}

void UI::cModelTool::ShowModelInspector()
{
	//모델 로드 버튼, 텍스트
	if (ImGui::CollapsingHeader("Model"))
	{
		ImGui::TextDisabled(_modelName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			//todo : 이미 모델이 존재하면 팝업 띄워서 물어보기
			OpenFile();
		}
	}
}

bool UI::cModelTool::AlertModel()
{
	if (_model.expired())
		return true;

	return false;
}

void UI::cModelTool::OpenFile()
{
	bool openFbx = false;
	cPath::OpenFileDialog(L"Open Model", cPath::ModelFilter, Asset, [this](wstring path)
	{
		auto extension = cPath::GetExtension(path);

		//확장자가 fbx면 export 후에 불러오기
		if (extension == L"fbx")
			ExportFbx(path, &path);

		OpenModel(cPath::GetDirectoryName(path),
					cPath::GetFileNameWithoutExtension(path));
	});
}

void UI::cModelTool::ExportFbx(wstring path, OUT wstring *newPath)
{
	Fbx::Exporter exporter{ path };
	
	cPath::SaveFileDialog(L"Save Model", cPath::MaterialFilter, Model, [&exporter, &newPath](wstring savePath)
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
	shared_ptr<cModel> model = cModelFactory::Get()->Create(path,
											  name);

	_actor.lock()->SetModel(model);

	cModelFactory::Delete();

	_modelName = cString::String(name);
	cDebug::Log((_modelName + " Open!").c_str());
}
