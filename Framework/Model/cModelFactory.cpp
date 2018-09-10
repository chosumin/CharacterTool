#include "stdafx.h"
#include "cModelFactory.h"
#include "cModel.h"
#include "./Transform/sTransform.h"
#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"
#include "./ModelPart/cModelMeshPart.h"
#include "./Graphic/cMaterial.h"
#include "./Helper/Json.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"
#include "./Helper/cBinary.h"
#include "./Helper/cMath.h"

unique_ptr<cModel> cModelFactory::_model = nullptr;

unique_ptr<cModel> cModelFactory::Create(wstring filePath, wstring fileName)
{
	//머터리얼, 메쉬는 모델간 공유가 되서는 안되므로 캐쉬를 생성하지 않음
	struct make_unique_enabler : public cModel {};

	_model = make_unique<make_unique_enabler>();

	_model->_modelPath = filePath + fileName;
	
	ReadMaterials(filePath + fileName + L".material");
	ReadMesh(filePath + fileName + L".mesh");

	return move(_model);
}

unique_ptr<cModel> cModelFactory::Clone(weak_ptr<cModel> model)
{
	auto copiedModel = model.lock()->Clone();

	BindMeshData();

	return move(copiedModel);
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
		material->SetTextureColor(ColorType::AMBIENT, color, 0);

		Json::GetValue(value, "Diffuse", color);
		material->SetTextureColor(ColorType::DIFFUSE, color, 0);

		Json::GetValue(value, "Emissive", color);
		material->SetTextureColor(ColorType::EMISSIVE, color, 0);

		Json::GetValue(value, "Specular", color);
		material->SetTextureColor(ColorType::SPECULAR, color, 0);

		float shininess;
		Json::GetValue(value, "Shininess", shininess);
		material->SetTextureColor(ColorType::SHININESS, {}, shininess);

		string textureFile;
		string directory = cPath::GetDirectoryName(cString::String(file));

		Json::GetValue(value, "DiffuseFile", textureFile);
		ReadTextureMap(material.get(), TextureType::DIFFUSE, directory, textureFile);

		Json::GetValue(value, "SpecularFile", textureFile);
		ReadTextureMap(material.get(), TextureType::SPECULAR, directory, textureFile);

		Json::GetValue(value, "EmissiveFile", textureFile);
		ReadTextureMap(material.get(), TextureType::EMISSIVE, directory, textureFile);

		Json::GetValue(value, "NormalFile", textureFile);
		ReadTextureMap(material.get(), TextureType::NORMAL, directory, textureFile);

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

		//_model->_animatedTM.assign(count, cMath::MATRIX_IDENTITY);
		_model->_skinnedTM.assign(count, cMath::MATRIX_IDENTITY);

		count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			auto mesh = make_shared<cModelMesh>();
			ReadMeshData(r, mesh);
			_model->_meshes.emplace_back(move(mesh));
		}
	}
	r->Close();

	BindMeshData();
}

void cModelFactory::ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone)
{
	auto bonePtr = bone.lock();
	auto readerPtr = r.lock();
	bonePtr->_index = readerPtr->Int();
	bonePtr->_name = cString::Wstring(readerPtr->String());
	bonePtr->_parentIndex = readerPtr->Int();
	
	readerPtr->Matrix();
	bonePtr->_absoluteTransform->Matrix = readerPtr->Matrix();
	D3DXMatrixInverse(&bonePtr->_invAbsoluteMatrix, nullptr, &bonePtr->_absoluteTransform->Matrix);
	bonePtr->_absoluteTransform->Decompose();

	*bonePtr->_animatedTransform = *bonePtr->_absoluteTransform;
}

void cModelFactory::ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh)
{
	auto meshPtr = mesh.lock();
	auto readerPtr = r.lock();
	meshPtr->_name = cString::Wstring(readerPtr->String());

	int parentBoneIndex = readerPtr->Int();
	LinkMeshToBone(mesh, parentBoneIndex);

	UINT partCount = readerPtr->UInt();

	vector<wstring> matNames;
	for (UINT k = 0; k < partCount; k++)
	{
		auto meshPart = make_shared<cModelMeshPart>();
		ReadMeshPartData(r, meshPart);

		bool duplicate = false;
		for (auto&& matName : matNames)
		{
			if (meshPart->_materialName == matName)
			{
				duplicate = true;
				break;
			}
		}

		if (duplicate == false)
		{
			matNames.push_back(meshPart->_materialName);
			meshPtr->_meshParts.push_back(move(meshPart));
		}
	}
}

void cModelFactory::LinkMeshToBone(weak_ptr<cModelMesh> mesh, int parentBoneIndex)
{
	auto meshPtr = mesh.lock();
	for (auto&& bone : _model->_bones)
	{
		if (parentBoneIndex == bone->_index)
		{
			meshPtr->_parentBoneIndex = parentBoneIndex;
			meshPtr->_parentBone = bone;
			break;
		}
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

void cModelFactory::BindMeshData()
{
	for (auto&& mesh : _model->_meshes)
	{
		for (auto&& part : mesh->_meshParts)
			part->_material = _model->GetMaterial(part->_materialName);
	}

	_model->_rootBone = _model->_bones[0];
	for (auto&& bone : _model->_bones)
	{
		if (bone->_parentIndex > -1)
		{
			bone->_parent = _model->_bones[bone->_parentIndex];
			bone->_parent.lock()->_children.push_back(bone);
		}
	}

	for (auto&& mesh : _model->_meshes)
	{
		mesh->_parentBone = _model->_bones[mesh->_parentBoneIndex];
		mesh->Binding();
	}
}

void cModelFactory::ReadTextureMap(cMaterial * material, TextureType type, string directory, string fileName)
{
	if (fileName.length() > 0)
		material->SetTextureMap(type, directory + fileName);
}
