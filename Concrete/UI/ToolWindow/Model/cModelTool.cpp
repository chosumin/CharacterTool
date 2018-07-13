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
	//�� ���ý�
	ShowModelInspector();

	//�޽� ���ý�
	//��, �޽� �ν����� ��� ����
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
	//�� �ε� ��ư, �ؽ�Ʈ
	if (ImGui::CollapsingHeader("Model"))
	{
		ImGui::TextDisabled(_modelName.c_str());
		ImGui::SameLine();
		if (ImGui::Button("Load"))
		{
			//todo : �̹� ���� �����ϸ� �˾� ����� �����
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

		//Ȯ���ڰ� fbx�� export �Ŀ� �ҷ�����
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

		//������ �� ��θ� ��ȯ
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
