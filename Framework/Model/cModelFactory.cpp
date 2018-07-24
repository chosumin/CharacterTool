#include "stdafx.h"
#include "cModelFactory.h"
#include "cModel.h"

#include "./Transform/sTransform.h"

#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"
#include "./ModelPart/cModelMeshPart.h"
#include "./ModelPart/cModelAnimClip.h"
#include "./ModelPart/ModelKeyFrames.h"

#include "./Graphic/cMaterial.h"
#include "./Helper/Json.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"
#include "./Helper/cBinary.h"

unique_ptr<cModel> cModelFactory::Create(wstring filePath, wstring fileName)
{
	//트릭 구조체
	struct make_unique_enabler : public cModel {};

	_model = make_unique<make_unique_enabler>();

	ReadMaterials(filePath + fileName + L".material");
	ReadMesh(filePath + fileName + L".mesh");

	return move(_model);
}

unique_ptr<cModel> cModelFactory::Clone(weak_ptr<cModel> model)
{
	_model = model.lock()->Clone();

	BindMeshData();

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

	BindMeshData();
}

void cModelFactory::ReadModelBoneData(weak_ptr<cBinaryReader> r, weak_ptr<cModelBone> bone)
{
	auto bonePtr = bone.lock();
	auto readerPtr = r.lock();
	bonePtr->_index = readerPtr->Int();
	bonePtr->_name = cString::Wstring(readerPtr->String());
	bonePtr->_parentIndex = readerPtr->Int();
	bonePtr->_transform->Matrix = readerPtr->Matrix();
	bonePtr->_absoluteTransform->Matrix = readerPtr->Matrix();

	bonePtr->_transform->Decompose();
	bonePtr->_absoluteTransform->Decompose();
}

void cModelFactory::ReadMeshData(weak_ptr<cBinaryReader> r, weak_ptr<cModelMesh> mesh)
{
	auto meshPtr = mesh.lock();
	auto readerPtr = r.lock();
	meshPtr->_name = cString::Wstring(readerPtr->String());

	int parentBoneIndex = readerPtr->Int();
	LinkMeshToBone(mesh, parentBoneIndex);

	UINT partCount = readerPtr->UInt();
	for (UINT k = 0; k < partCount; k++)
	{
		auto meshPart = make_shared<cModelMeshPart>();
		ReadMeshPartData(r, meshPart);
		meshPtr->_meshParts.push_back(move(meshPart));
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

void cModelFactory::ReadAnimation(wstring file)
{
	shared_ptr<cBinaryReader> r = cBinaryReader::Open(file);
	{
		UINT count = r->UInt();
		for (UINT i = 0; i < count; i++)
		{
			auto clip = make_unique<cModelAnimClip>();
			clip->_name = cString::Wstring(r->String());

			clip->_totalFrame = r->Int();
			clip->_frameRate = r->Float();
			clip->_defaultFrameRate = clip->_frameRate;

			UINT frameCount = r->UInt();
			for (UINT frame = 0; frame < frameCount; frame++)
			{
				auto keyframe = make_unique<sModelKeyFrame>();
				keyframe->BoneName = cString::Wstring(r->String());

				UINT count = r->UInt();
				keyframe->FrameData.assign(count, sModelKeyFrameData());

				void* ptr = (void *)&(keyframe->FrameData[0]);
				r->Byte(&ptr, sizeof(sModelKeyFrameData) * count);

				clip->_keyFrames[keyframe->BoneName] = move(keyframe);
			}//for(frame)

			_model->_clips.emplace_back(move(clip));
		}
	}
	r->Close();
}

void cModelFactory::BindMeshData()
{
	for (auto&& mesh : _model->_meshes)
	{
		for (auto&& part : mesh->_meshParts)
			part->_material = _model->GetMaterial(part->_materialName);
	}

	_model->_root = _model->_bones[0];
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
