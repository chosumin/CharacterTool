#include "stdafx.h"
#include "Exporter.h"
#include "FbxType.h"
#include "FbxBoneWeights.h"
#include "FbxUtility.h"
#include "./Helper/cString.h"
#include "./Helper/cPath.h"
#include "./Helper/Json.h"
#include "./Helper/cBinary.h"

Fbx::Exporter::Exporter(wstring fbxFile)
{
	_manager = FbxManager::Create();

	_scene = FbxScene::Create(_manager, "");

	//ios ����
	_ios = FbxIOSettings::Create(_manager, IOSROOT);
	_ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	_manager->SetIOSettings(_ios);

	//importer ����
	_importer = FbxImporter::Create(_manager, "");
	auto sFile = cString::String(fbxFile);
	bool bCheck = _importer->Initialize(sFile.c_str(), -1, _ios);
	assert(bCheck == true && "������ �ʱ�ȭ ����!");
	bCheck = _importer->Import(_scene);
	assert(bCheck == true && "�� �ҷ����� ���� ���� �߻�!");

	//converter ����
	_converter = new FbxGeometryConverter(_manager);
}

Fbx::Exporter::~Exporter()
{
	SAFE_DELETE(_converter);

	_ios->Destroy();
	_importer->Destroy();
	_scene->Destroy();
	_manager->Destroy();
}

void Fbx::Exporter::ExportMaterial(wstring saveFolder, wstring saveName)
{
	int count = _scene->GetMaterialCount();

	//Fbx ���͸��� �����̳� ��ȸ
	for (int i = 0; i < count; i++)
		PushMaterials(i);

	//����� ���͸��� ���� ���� ������ ���� ��� ����
	if (cPath::ExistDirectory(saveFolder) == false)
		CreateDirectory(saveFolder.c_str(), nullptr);

	//Json ��Ʈ ��� ����
	Json::Value root;

	//����� ���͸��� �����̳� ��ȸ
	for (auto&& material : _materials)
		WriteMaterialDataOnJson(root, saveFolder, move(material));
	
	//Json ����
	Json::WriteData(saveFolder + saveName + L".material", &root);

	_materials.clear();
}

void Fbx::Exporter::ExportMesh(wstring saveFolder, wstring saveName)
{
	ReadJointData(_scene->GetRootNode(), -1, -1);

	ReadMeshData(_scene->GetRootNode(), -1, -1);

	CreateBoneFromMeshData();
	CreateBoneFromJointData();

	WriteMeshData(saveFolder, saveName);

	_joints.clear();
	_bones.clear();
	_meshes.clear();
}

void Fbx::Exporter::ExportAnimation(wstring saveFolder, wstring saveName)
{
	//�� �������� �ð� ������
	auto mode = _scene->GetGlobalSettings().GetTimeMode();
	float frameRate = static_cast<float>(FbxTime::GetFrameRate(mode));

	/*FbxArray<FbxString *> animationArray;
	auto document = dynamic_cast<FbxDocument*>(scene);
	document->FillAnimStackNameArray(animationArray);*/

	ReadAnimation(frameRate);

	WriteAnimation(saveFolder, saveName);

	_animations.clear();
}

void Fbx::Exporter::PushMaterials(int number)
{
	auto fbxMaterial = _scene->GetMaterial(number);

	//����� ���͸��� ��ü ����
	auto material = make_unique<FbxMaterial>();
	material->Name = fbxMaterial->GetName();

	//����Ʈ ���� ����
	if (fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		auto lambert = (FbxSurfaceLambert*)fbxMaterial;

		material->Ambient = Fbx::Utility::ToColor(lambert->Ambient, lambert->AmbientFactor);
		material->Diffuse = Fbx::Utility::ToColor(lambert->Diffuse, lambert->DiffuseFactor);
		material->Emissive = Fbx::Utility::ToColor(lambert->Emissive, lambert->EmissiveFactor);
	}

	//�� ���� ����
	if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		auto phong = (FbxSurfacePhong*)fbxMaterial;

		material->Specular = Fbx::Utility::ToColor(phong->Specular, phong->SpecularFactor);
		material->Shininess = static_cast<float>(phong->Shininess);
	}

	//���͸��� �ؽ��� ���� ���� ����
	FbxProperty prop;
	prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sDiffuse);
	material->DiffuseFile = Fbx::Utility::GetTextureFile(prop);

	prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sSpecular);
	material->SpecularFile = Fbx::Utility::GetTextureFile(prop);

	prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sEmissive);
	material->EmissiveFile = Fbx::Utility::GetTextureFile(prop);

	prop = fbxMaterial->FindProperty(FbxSurfaceMaterial::sNormalMap);
	material->NormalFile = Fbx::Utility::GetTextureFile(prop);

	_materials.push_back(move(material));
}

void Fbx::Exporter::WriteMaterialDataOnJson(Json::Value & root, wstring saveFolder, unique_ptr<FbxMaterial> material)
{
	Json::Value val;
	Json::SetValue(val, "Name", material->Name);

	//todo : ���̴� ���� �ٲ��� ������?
	string shaderName = "999_Mesh.hlsl";
	Json::SetValue(val, "ShaderName", shaderName);

	Json::SetValue(val, "Ambient", material->Ambient);
	Json::SetValue(val, "Diffuse", material->Diffuse);
	Json::SetValue(val, "Emissive", material->Emissive);
	Json::SetValue(val, "Specular", material->Specular);

	Json::SetValue(val, "Shininess", material->Shininess);

	//���� ����, json�� ��� ����
	CopyTextureFile(material->DiffuseFile, saveFolder);
	Json::SetValue(val, "DiffuseFile", material->DiffuseFile);

	CopyTextureFile(material->SpecularFile, saveFolder);
	Json::SetValue(val, "SpecularFile", material->SpecularFile);

	CopyTextureFile(material->EmissiveFile, saveFolder);
	Json::SetValue(val, "EmissiveFile", material->EmissiveFile);

	CopyTextureFile(material->NormalFile, saveFolder);
	Json::SetValue(val, "NormalFile", material->NormalFile);

	root[material->Name.c_str()] = val;
}

void Fbx::Exporter::CopyTextureFile(string & textureFile, wstring saveFolder)
{
	//���� ������ ����
	if (textureFile.length() < 1)
	{
		//cDebugLog::Log(&typeid(this), []() { cout << "�������� �ʴ� ���ϸ�" << endl; });
		return;
	}

	auto file = cString::Wstring(textureFile);
	auto fileName = cPath::GetFileName(file);

	//���� ����
	if (cPath::ExistFile(textureFile) == true)
		CopyFile(file.c_str(), (saveFolder + fileName).c_str(), FALSE);

	textureFile = cString::String(fileName);
}

void Fbx::Exporter::ReadJointData(FbxNode * node, int index, int parent)
{
	auto attribute = node->GetNodeAttribute();

	if (attribute != nullptr)
	{
		auto nodeType = attribute->GetAttributeType();

		if (nodeType == FbxNodeAttribute::eSkeleton)
		{
			auto joint = make_unique<FbxJoint>();
			joint->Parent = parent;
			joint->Name = node->GetName();

			_joints.emplace_back(move(joint));
		}
	}

	//�ڽ� ��� ���
	for (int i = 0; i < node->GetChildCount(); i++)
		ReadJointData(node->GetChild(i), _joints.size(), index);
}

void Fbx::Exporter::ReadMeshData(FbxNode * node, int index, int parent)
{
	auto attribute = node->GetNodeAttribute();
	if (attribute != nullptr)
	{
		auto nodeType = attribute->GetAttributeType();
		if (nodeType == FbxNodeAttribute::eMesh)
		{
			_converter->Triangulate(attribute, true, true);
			auto mesh = node->GetMesh();
			int polygonCount = mesh->GetPolygonCount();

			////////////////////////������/////////////////////////

			int deformerCount = mesh->GetDeformerCount();

			//����ġ�� ���� ������ �ڷ��̹Ƿ� ��Ʈ�� ����Ʈ ������ ����.
			vector<FbxBoneWeights> boneWeights(mesh->GetControlPointsCount(), FbxBoneWeights());

			for (int i = 0; i < deformerCount; i++)
				PushBoneWeight(mesh, boneWeights, i);

			for (UINT i = 0; i < boneWeights.size(); i++)
				boneWeights[i].Normalize();

			//todo : Ȯ�� ���, �� ������ ������ �����ϱ�?
			vector<FbxBoneWeights> meshBoneWeights(boneWeights.size(), FbxBoneWeights());
			for (UINT i = 0; i < boneWeights.size(); i++)
				meshBoneWeights[i].AddBoneWeights(boneWeights[i]);

			///////////////////////����///////////////////////////
			vector<shared_ptr<FbxVertex>> vertices;

			//�ﰢ�� ������ŭ ��ȸ
			for (int p = 0; p < polygonCount; p++)
			{
				int vertexInPolygon = mesh->GetPolygonSize(p);
				assert(vertexInPolygon == 3 && "������ ���� ����!");

				//�ﰢ�� ���� ��ȸ
				for (int vi = 0; vi < vertexInPolygon; vi++)
				{
					vertices.emplace_back(move(CreatePolygonVertex(mesh, p, vi, meshBoneWeights)));
				}
			}

			_meshes.emplace_back(CreateMeshData(node, parent, vertices));
		}
	}

	//�ڽ� ��� ���
	for (int i = 0; i < node->GetChildCount(); i++)
		ReadMeshData(node->GetChild(i), _meshes.size(), index);
}

void Fbx::Exporter::PushBoneWeight(FbxMesh * mesh, OUT vector<struct FbxBoneWeights>& boneWeights, UINT i)
{
	auto deformer = mesh->GetDeformer(i, FbxDeformer::eSkin);

	//��Ų Ÿ������ �ٿ�ĳ����
	auto skin = reinterpret_cast<FbxSkin*>(deformer);
	if (skin == nullptr)
		return;

	//������ ������ Ŭ������ ��ȸ
	for (int clusterIndex = 0; clusterIndex < skin->GetClusterCount(); clusterIndex++)
	{
		auto cluster = skin->GetCluster(clusterIndex);
		assert(cluster->GetLinkMode() == FbxCluster::eNormalize);

		//joint ��ȣ ����
		string linkName = cluster->GetLink()->GetName();
		UINT jointIndex = GetJointIndexByName(linkName);

		//����Ʈ ����, ���� ��� ������
		FbxAMatrix transform, absoluteTransform;
		cluster->GetTransformMatrix(transform);
		cluster->GetTransformLinkMatrix(absoluteTransform);

		_joints[jointIndex]->Transform = Utility::ToMatrix(transform);
		_joints[jointIndex]->AbsoluteTransform = Utility::ToMatrix(absoluteTransform);

		//���� ������ ��� ���Ͽ� ��Ʈ�� ����Ʈ ��ȣ ��ȸ
		for (int index = 0; index < cluster->GetControlPointIndicesCount(); index++)
		{
			int temp = cluster->GetControlPointIndices()[index];

			double *weights = cluster->GetControlPointWeights();

			boneWeights[temp].AddBoneWeight(jointIndex, (float)weights[index]);
		}
	}//for(clusterIndex : skin->GetClusterCount())
}

UINT Fbx::Exporter::GetJointIndexByName(string name)
{
	for (UINT i = 0; i < _joints.size(); i++)
	{
		if (_joints[i]->Name == name)
			return i;
	}

	return 0;
}

unique_ptr<Fbx::FbxVertex> Fbx::Exporter::CreatePolygonVertex(FbxMesh * mesh, int p, int vi, vector<struct FbxBoneWeights>& meshBoneWeights)
{
	auto vertex = make_unique<FbxVertex>();

	int cpIndex = mesh->GetPolygonVertex(p, vi);

	//todo : ���� ���� ���ص��ɰŰ�����
	vertex->ControlPoint = cpIndex;

	//��ġ ����
	FbxVector4 position = mesh->GetControlPointAt(cpIndex);
	vertex->Vertex.position = Fbx::Utility::ToVector3(position);

	//���� ����
	FbxVector4 normal;
	mesh->GetPolygonVertexNormal(p, vi, normal);
	normal.Normalize();
	vertex->Vertex.normal = Fbx::Utility::ToVector3(normal);

	//���͸��� �̸� ����
	vertex->MaterialName = Fbx::Utility::GetMaterialName(mesh, p, cpIndex);

	//UV ����
	int uvIndex = mesh->GetTextureUVIndex(p, vi);
	vertex->Vertex.uv = Fbx::Utility::GetUv(mesh, cpIndex, uvIndex);

	//����ġ ����
	FbxBlendWeight weight;
	meshBoneWeights[cpIndex].GetBlendWeights(weight);
	vertex->Vertex.blendIndices = weight.Indices;
	vertex->Vertex.blendWeights = weight.Weights;

	return move(vertex);
}

unique_ptr<Fbx::FbxMeshData> Fbx::Exporter::CreateMeshData(FbxNode * node, int parent, const vector<shared_ptr<FbxVertex>>& vertices)
{
	auto meshData = make_unique<FbxMeshData>();
	meshData->Name = node->GetName();
	meshData->ParentBone = parent;

	//�޽� �����Ϳ� �޽� ��Ʈ ����
	for (int i = 0; i < _scene->GetMaterialCount(); i++)
	{
		auto meshPart = move(CreateMeshPart(i, vertices));
		if (meshPart)
			meshData->MeshParts.emplace_back(move(meshPart));
	}

	//���� ���
	FbxAMatrix matrix = node->EvaluateLocalTransform();
	meshData->Transform = Utility::ToMatrix(matrix);

	//���� ���
	matrix = node->EvaluateGlobalTransform();
	meshData->AbsoluteTransform = Utility::ToMatrix(matrix);

	return move(meshData);
}

unique_ptr<Fbx::FbxMeshPartData> Fbx::Exporter::CreateMeshPart(int i, const vector<shared_ptr<FbxVertex>>& vertices)
{
	auto material = _scene->GetMaterial(i);
	string materialName = material->GetName();

	//���͸��󺰷� �����̳ʿ� ����
	vector<weak_ptr<FbxVertex>> gather;
	for (auto&& temp : vertices)
	{
		if (temp->MaterialName == materialName)
			gather.push_back(temp);
	}

	if (gather.size() < 1)
		return nullptr;

	//����� ����, �ε��� ����
	auto meshPart = make_unique<FbxMeshPartData>();
	meshPart->MaterialName = materialName;
	for (auto&& temp : gather)
	{
		meshPart->Vertices.emplace_back(temp.lock()->Vertex);
		meshPart->Indices.emplace_back(meshPart->Indices.size());
	}

	return move(meshPart);
}

void Fbx::Exporter::CreateBoneFromMeshData()
{
	if (_joints.size() > 0)
		return;

	for (size_t i = 0; i < _meshes.size(); i++)
	{
		auto bone = make_unique<FbxBoneData>();
		bone->Index = i; //�θ� �޽� �����Ͱ� �����ϴ� ��
		bone->Parent = _meshes[i]->ParentBone;
		bone->Name = _meshes[i]->Name;
		bone->Transform = _meshes[i]->Transform;

		//�������� �� ��ü�� ���̴�
		_meshes[i]->ParentBone = bone->Index;

		_bones.emplace_back(move(bone));
	}
}

void Fbx::Exporter::CreateBoneFromJointData()
{
	if (_joints.size() < 1)
		return;

	//�� ������ Ǫ��
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	auto bone = CreateBoneData(0, -1, "RootNode", matrix, matrix);
	_bones.emplace_back(move(bone));
	//�� Ʈ��
	for (UINT i = 0; i < _joints.size(); i++)
	{
		//todo : �������ÿ� �θ� ��ȣ�� ������� ����
		auto bone = CreateBoneData
		(
			_bones.size(),
			_joints[i]->Parent + 1,
			_joints[i]->Name,
			_joints[i]->Transform,
			_joints[i]->AbsoluteTransform
		);
		_bones.emplace_back(move(bone));
	}

	//�޽� ���
	for (UINT i = 0; i < _meshes.size(); i++)
	{
		auto bone = CreateBoneData
		(
			_bones.size(),
			0,
			_meshes[i]->Name,
			_meshes[i]->Transform,
			_meshes[i]->AbsoluteTransform
		);
		_bones.emplace_back(move(bone));
	}
}

unique_ptr<Fbx::FbxBoneData> Fbx::Exporter::CreateBoneData(int index, int parent, string name, const D3DXMATRIX & transform, const D3DXMATRIX & absoluteTransform)
{
	auto root = make_unique<FbxBoneData>();
	root->Index = index;
	root->Parent = parent;
	root->Name = name;
	root->Transform = transform;
	root->AbsoluteTransform = absoluteTransform;

	return move(root);
}

void Fbx::Exporter::WriteMeshData(wstring saveFolder, wstring saveName)
{
	if (cPath::ExistDirectory(saveFolder) == false)
		CreateDirectory(saveFolder.c_str(), nullptr);

	auto w = cBinaryWriter::Open(saveFolder + saveName + L".mesh");
	{
		WriteBoneDataOnBinaryFile(w.get());
		WriteMeshDataOnBinaryFile(w.get());
	}
	w->Close();
}

void Fbx::Exporter::WriteBoneDataOnBinaryFile(cBinaryWriter * w)
{
	w->UInt(_bones.size());
	for (auto&& bone : _bones)
	{
		w->Int(bone->Index);
		w->String(bone->Name);

		w->Int(bone->Parent);
		w->Matrix(bone->Transform);
		w->Matrix(bone->AbsoluteTransform);
	}
}

void Fbx::Exporter::WriteMeshDataOnBinaryFile(cBinaryWriter * w)
{
	w->UInt(_meshes.size());
	for (auto&& mesh : _meshes)
	{
		w->String(mesh->Name);
		w->Int(mesh->ParentBone);

		w->UInt(mesh->MeshParts.size());
		for (auto&& part : mesh->MeshParts)
		{
			w->String(part->MaterialName);

			w->UInt(part->Vertices.size());
			w->Byte(&part->Vertices[0], sizeof(ModelVertexType) * part->Vertices.size());

			w->UInt(part->Indices.size());
			w->Byte(&part->Indices[0], sizeof(UINT) * part->Indices.size());
		}
	}
}

void Fbx::Exporter::ReadAnimation(float frameRate)
{
	for (int i = 0; i < _importer->GetAnimStackCount(); i++)
	{
		auto animation = make_unique<FbxAnimation>();
		animation->FrameRate = frameRate;

		//�ִϸ��̼� ������
		auto takeInfo = _importer->GetTakeInfo(i);
		animation->Name = takeInfo->mName.Buffer();

		//�ð� ���
		auto span = takeInfo->mLocalTimeSpan;
		float start = (float)span.GetStart().GetSecondDouble();
		float end = (float)span.GetStop().GetSecondDouble();

		if (start < end)
		{
			animation->TotalFrame = (int)((end - start) * frameRate) + 1;

			ReadAnimation(*animation, _scene->GetRootNode(), frameRate, start, end);
		}

		_animations.emplace_back(move(animation));
	}
}

void Fbx::Exporter::ReadAnimation(OUT FbxAnimation & animation, FbxNode * node, float frameRate, float start, float end)
{
	auto attribute = node->GetNodeAttribute();

	if (attribute != nullptr)
	{
		auto nodeType = attribute->GetAttributeType();
		if (nodeType == FbxNodeAttribute::eSkeleton)
		{
			auto keyFrame = CreateKeyFrame(node, end, frameRate);
			animation.KeyFrames.emplace_back(move(keyFrame));
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
		ReadAnimation(animation, node->GetChild(i), frameRate, start, end);
}

unique_ptr<Fbx::FbxKeyFrame> Fbx::Exporter::CreateKeyFrame(FbxNode * node, float end, float frameRate)
{
	auto keyFrame = make_unique<FbxKeyFrame>();
	keyFrame->BoneName = node->GetName();

	float time = 0.0f;
	while (time <= end)
	{
		FbxTime animationTime;

		//1�� �������� �������� ���´�.
		animationTime.SetSecondDouble(time);

		//�ش� �ð��� ���� Ʈ������
		auto matrix = node->EvaluateLocalTransform(animationTime);
		auto transform = Utility::ToMatrix(matrix);

		auto data = CreateKeyFrameData(node, transform, animationTime);
		keyFrame->Transform.push_back(data);

		//1������ - 1/24�� �ϳ��� ���������� �����Ѵ�.
		time += 1.0f / frameRate;
	}

	return move(keyFrame);
}

Fbx::FbxKeyFrameData Fbx::Exporter::CreateKeyFrameData(FbxNode * node, const D3DXMATRIX& transform, FbxTime animationTime)
{
	FbxKeyFrameData data;
	data.Transform = transform;
	data.Translation = D3DXVECTOR3{ transform._41, transform._42, transform._43 };
	D3DXQuaternionRotationMatrix(&data.Rotation, &transform);

	//���� ������ ������
	auto scale = node->EvaluateLocalScaling(animationTime);
	data.Scale = Utility::ToVector3(scale);

	return data;
}

void Fbx::Exporter::WriteAnimation(wstring saveFolder, wstring saveName)
{
	//�������� �ʴ� ��θ� ���� ����
	if (cPath::ExistDirectory(saveFolder) == false)
		CreateDirectory(saveFolder.c_str(), nullptr);

	auto w = cBinaryWriter::Open(saveFolder + saveName + L".anim");
	{
		w->UInt(_animations.size());
		for(auto&& anim : _animations)
		{
			w->String(anim->Name);

			w->Int(anim->TotalFrame);
			w->Float(anim->FrameRate);

			w->UInt(anim->KeyFrames.size());
			for (auto&& frame : anim->KeyFrames)
			{
				w->String(frame->BoneName);
				w->UInt(frame->Transform.size());
				w->Byte(&frame->Transform[0], sizeof(FbxKeyFrameData) * frame->Transform.size());
			}
		}
	}
	w->Close();
}