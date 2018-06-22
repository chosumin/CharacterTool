#include "stdafx.h"
#include "cModelFactory.h"
#include "cModel.h"

#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"
#include "./ModelPart/cModelMeshPart.h"

#include "./Graphic/cMaterial.h"
#include "./Helper/Json.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"
#include "./Helper/cBinary.h"

shared_ptr<cModel> cModelFactory::Create(wstring file)
{
	//트릭 구조체
	struct make_shared_enabler : public cModel {};

	_model = make_unique<make_shared_enabler>();

	ReadMaterials(file + L".material");
	ReadMesh(file + L".mesh");
	Binding();

	return move(_model);
}

shared_ptr<cModel> cModelFactory::Clone(weak_ptr<cModel> model)
{
	_model = model.lock()->Clone();

	Binding();

	return move(_model);
}

cModelFactory::cModelFactory()
{
}

cModelFactory::~cModelFactory()
{
}

void cModelFactory::ReadMaterials(wstring file)
{
	Json::Value root;
	Json::ReadData(file, &root);

	for (auto& value : root)
	{
		auto material = make_shared<cMaterial>();

		string name;
		Json::GetValue(value, "Name", name);
		material->SetName(cString::Wstring(name));

		Json::GetValue(value, "ShaderName", name);
		if (name.length() > 0)
			material->SetShader(Shader + cString::Wstring(name));

		D3DXCOLOR color;
		Json::GetValue(value, "Ambient", color);
		material->SetTextureColor(ColorType::Ambient, color, 0);

		Json::GetValue(value, "Diffuse", color);
		material->SetTextureColor(ColorType::Diffuse, color, 0);

		Json::GetValue(value, "Emissive", color);
		material->SetTextureColor(ColorType::Emissive, color, 0);

		Json::GetValue(value, "Specular", color);
		material->SetTextureColor(ColorType::Specular, color, 0);

		float shininess;
		Json::GetValue(value, "Shininess", shininess);
		material->SetTextureColor(ColorType::Shineniss, {}, shininess);

		string textureFile;
		string directory = cPath::GetDirectoryName(cString::String(file));

		Json::GetValue(value, "DiffuseFile", textureFile);
		ReadTextureMap(material.get(), TextureType::Diffuse, directory, textureFile);

		Json::GetValue(value, "SpecularFile", textureFile);
		ReadTextureMap(material.get(), TextureType::Specular, directory, textureFile);

		Json::GetValue(value, "EmissiveFile", textureFile);
		ReadTextureMap(material.get(), TextureType::Emissive, directory, textureFile);

		Json::GetValue(value, "NormalFile", textureFile);
		ReadTextureMap(material.get(), TextureType::Normal, directory, textureFile);

		_model->_materials.emplace_back(move(material));
	}
}

void cModelFactory::ReadMesh(wstring file)
{
	shared_ptr<cBinaryReader> r = cBinaryReader::Open(file);
	{
		UINT count = 0;
		count = r->UInt();

		for (UINT i = 0; i < count; i++)
		{
			auto bone = make_shared<cModelBone>();
			ReadModelBoneData(r, bone);
			_model->_bones.emplace_back(move(bone));
		}

		count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			auto mesh = make_shared<cModelMesh>();
			ReadMeshData(r, mesh);
			_model->_meshes.emplace_back(move(mesh));
		}
	}
	r->Close();
}

void cModelFactory::Binding()
{
	for (UINT i = 0; i < _model->_bones.size(); i++)
	{
		auto bone = _model->_bones[i];

		//루트 노드
		if (bone->_parentIndex < 0)
			_model->_root = _model->_bones[i];
		else
			bone->_parent = _model->_bones[bone->_parentIndex];
	}

	for (size_t i = 0; i < _model->_meshes.size(); i++)
		_model->_meshes[i]->Binding();

	for (auto&& mesh : _model->_meshes)
	{
		for (auto&& part : mesh->_meshParts)
			part->_material = _model->GetMaterial(part->_materialName);
	}
}

void cModelFactory::ReadTextureMap(cMaterial * material, TextureType type, string directory, string fileName)
{
	if (fileName.length() > 0)
		material->SetTextureMap(type, directory + fileName);
}

void cModelFactory::ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone)
{
	bone.lock()->_index = r.lock()->Int();
	bone.lock()->_name = cString::Wstring(r.lock()->String());
	bone.lock()->_parentIndex = r.lock()->Int();
	bone.lock()->_transform = r.lock()->Matrix();
}

void cModelFactory::ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh)
{
	mesh.lock()->_name = cString::Wstring(r.lock()->String());

	int parentBoneIndex = r.lock()->Int();
	LinkMeshToBone(mesh, parentBoneIndex);

	UINT partCount = r.lock()->UInt();
	for (UINT k = 0; k < partCount; k++)
	{
		auto meshPart = make_shared<cModelMeshPart>();
		ReadMeshPartData(r, meshPart);
		mesh.lock()->_meshParts.push_back(move(meshPart));
	}
}

void cModelFactory::ReadMeshPartData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMeshPart> meshPart)
{
	meshPart.lock()->_materialName = cString::Wstring(r.lock()->String());

	//VertexData
	{
		UINT count = r.lock()->UInt();
		meshPart.lock()->_vertices.assign(count, ModelVertexType());

		void* ptr = (void*)&(meshPart.lock()->_vertices[0]);
		r.lock()->Byte(&ptr, sizeof(ModelVertexType) * count);
	}

	//IndexData
	{
		UINT count = r.lock()->UInt();
		meshPart.lock()->_indices.assign(count, UINT());

		void* ptr = (void*)&(meshPart.lock()->_indices[0]);
		r.lock()->Byte(&ptr, sizeof(UINT) * count);
	}
}

void cModelFactory::LinkMeshToBone(weak_ptr<cModelMesh> mesh, int parentBoneIndex)
{
	for (auto&& bone : _model->_bones)
	{
		if (parentBoneIndex == bone->_index)
		{
			mesh.lock()->_parentBoneIndex = parentBoneIndex;
			//mesh.lock()->_parentBone = bone;
			break;
		}
	}
}
