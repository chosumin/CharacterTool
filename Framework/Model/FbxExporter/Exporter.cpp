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
		WriteMaterialData(root, saveFolder, move(material));
	
	//Json ����
	Json::WriteData(saveFolder + saveName + L".material", &root);

	_materials.clear();
}

void Fbx::Exporter::ExportMesh(wstring saveFolder, wstring saveName)
{
	ReadBoneData(_scene->GetRootNode(), -1, -1);
	ReadSkinData();

	WriteMeshData(saveFolder, saveName);

	_bones.clear();
	_meshes.clear();
}

void Fbx::Exporter::ExportAnimation(wstring saveFolder, wstring saveName)
{
	//�� �������� �ð� ������
	auto mode = _scene->GetGlobalSettings().GetTimeMode();
	float frameRate = static_cast<float>(FbxTime::GetFrameRate(mode));

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

void Fbx::Exporter::WriteMaterialData(Json::Value & root, wstring saveFolder, unique_ptr<FbxMaterial> material)
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
		return;

	auto file = cString::Wstring(textureFile);
	auto fileName = cPath::GetFileName(file);

	//���� ����
	if (cPath::ExistFile(textureFile) == true)
		CopyFile(file.c_str(), (saveFolder + fileName).c_str(), FALSE);

	textureFile = cString::String(fileName);
}

void Fbx::Exporter::ReadBoneData(FbxNode * node, int index, int parent)
{
	auto attribute = node->GetNodeAttribute();

	if (attribute != nullptr)
	{
		auto nodeType = attribute->GetAttributeType();

		if (IsBoneType(nodeType))
		{
			auto bone = CreateBoneData(index, 
							   parent, 
							   node->GetName(), 
							   Utility::ToMatrix(node->EvaluateLocalTransform()), 
							   Utility::ToMatrix(node->EvaluateGlobalTransform()));
			_bones.emplace_back(move(bone));

			//�޽� ����� �޽� �����͵� ���� �о��
			if (nodeType == FbxNodeAttribute::eMesh)
			{
				_converter->Triangulate(attribute, true, true);
				ReadMeshData(node, index);
			}
		}
	}

	//�ڽ� ��� ���
	for (int i = 0; i < node->GetChildCount(); i++)
		ReadBoneData(node->GetChild(i), _bones.size(), index);
}

bool Fbx::Exporter::IsBoneType(FbxNodeAttribute::EType nodeType)
{
	bool bCheck = false;
	bCheck |= (nodeType == FbxNodeAttribute::eSkeleton);
	bCheck |= (nodeType == FbxNodeAttribute::eMesh);
	bCheck |= (nodeType == FbxNodeAttribute::eNull);
	bCheck |= (nodeType == FbxNodeAttribute::eMarker);
	return bCheck;
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

void Fbx::Exporter::ReadMeshData(FbxNode* node, int parentBone)
{
	auto mesh = node->GetMesh();
	
	vector<shared_ptr<FbxVertex>> vertices;

	//�ﰢ�� ������ŭ ��ȸ
	int polygonCount = mesh->GetPolygonCount();
	for (int p = 0; p < polygonCount; p++)
	{
		int vertexInPolygon = mesh->GetPolygonSize(p);
		assert(vertexInPolygon == 3 && "������ ���� ����!");

		//�ﰢ�� ���� ��ȸ
		for (int vi = 0; vi < vertexInPolygon; vi++)
			vertices.emplace_back(move(CreatePolygonVertex(mesh, p, vi)));
	}

	_meshes.emplace_back(CreateMeshData(node, parentBone, vertices));
}

unique_ptr<Fbx::FbxVertex> Fbx::Exporter::CreatePolygonVertex(FbxMesh *mesh, int p, int vi)
{
	auto vertex = make_unique<FbxVertex>();

	int cpIndex = mesh->GetPolygonVertex(p, vi);

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

	return move(vertex);
}

unique_ptr<Fbx::FbxMeshData> Fbx::Exporter::CreateMeshData(FbxNode * node, int parent, const vector<shared_ptr<FbxVertex>>& vertices)
{
	auto meshData = make_unique<FbxMeshData>();
	meshData->Name = node->GetName();
	meshData->ParentBone = parent;
	meshData->Vertices = vertices;
	meshData->Mesh = node->GetMesh();

	return move(meshData);
}

void Fbx::Exporter::ReadSkinData()
{
	for (auto&& meshData : _meshes)
	{
		auto mesh = meshData->Mesh;

		int deformerCount = mesh->GetDeformerCount();

		//����ġ�� ���� ������ �ڷ��̹Ƿ� ��Ʈ�� ����Ʈ ������ ����.
		vector<FbxBoneWeights> boneWeights(mesh->GetControlPointsCount(), FbxBoneWeights());

		//����ġ ����
		for (int i = 0; i < deformerCount; i++)
			PushBoneWeight(mesh, boneWeights, i);

		//�޽� �������� �θ� �� ����
		for (int i = 0; i < deformerCount; i++)
			PushMeshParent(meshData, i);

		for (UINT i = 0; i < boneWeights.size(); i++)
			boneWeights[i].Normalize();

		PushWeightInVertices(meshData->Vertices, boneWeights);
		
		//�޽� �����̳� ��ȸ�ϸ� �޽� ��Ʈ ����
		for (int i = 0; i < _scene->GetMaterialCount(); i++)
		{
			auto meshPart = CreateMeshPart(i, meshData->Vertices);
			meshData->MeshParts.emplace_back(move(meshPart));
		}
	}//for(meshData : _meshes)
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

		//�� ��ȣ ����
		string linkName = cluster->GetLink()->GetName();
		UINT jointIndex = GetBoneIndexByName(linkName);

		//����Ʈ ����, ���� ��� ������
		FbxAMatrix transform, absoluteTransform;
		cluster->GetTransformMatrix(transform);
		cluster->GetTransformLinkMatrix(absoluteTransform);

		//FIXME : SRT�� �� �־��־���ҵ�
		_bones[jointIndex]->Transform = Utility::ToMatrix(transform);
		_bones[jointIndex]->AbsoluteTransform = Utility::ToMatrix(absoluteTransform);

		//���� ������ ��� ���Ͽ� ��Ʈ�� ����Ʈ ��ȣ ��ȸ
		for (int index = 0; index < cluster->GetControlPointIndicesCount(); index++)
		{
			int temp = cluster->GetControlPointIndices()[index];

			double *weights = cluster->GetControlPointWeights();

			boneWeights[temp].AddBoneWeight(jointIndex, (float)weights[index]);
		}
	}//for(clusterIndex : skin->GetClusterCount())
}

void Fbx::Exporter::PushMeshParent(unique_ptr<FbxMeshData>& meshData, int i)
{
	auto deformer = meshData->Mesh->GetDeformer(i, FbxDeformer::eSkin);

	//��Ų Ÿ������ �ٿ�ĳ����
	auto skin = reinterpret_cast<FbxSkin*>(deformer);
	if (skin == nullptr)
		return;

	//������ ������ Ŭ������ ��ȸ
	for (int clusterIndex = 0; clusterIndex < skin->GetClusterCount(); clusterIndex++)
	{
		auto cluster = skin->GetCluster(clusterIndex);
		assert(cluster->GetLinkMode() == FbxCluster::eNormalize);

		//�� ��ȣ ����
		string linkName = cluster->GetLink()->GetName();
		UINT jointIndex = GetBoneIndexByName(linkName);

		meshData->ParentBone = jointIndex;
	}//for(clusterIndex : skin->GetClusterCount())
}

UINT Fbx::Exporter::GetBoneIndexByName(string name)
{
	for (UINT i = 0; i < _bones.size(); i++)
	{
		if (_bones[i]->Name == name)
			return i;
	}

	return 0;
}

void Fbx::Exporter::PushWeightInVertices(OUT vector<shared_ptr<FbxVertex>>& vertices, const vector<FbxBoneWeights>& boneWeights)
{
	for (auto&& vertex : vertices)
	{
		int cpIndex = vertex->ControlPoint;

		FbxBlendWeight weights;
		boneWeights[cpIndex].GetBlendWeights(weights);
		vertex->Vertex.blendIndices = weights.Indices;
		vertex->Vertex.blendWeights = weights.Weights;
	}
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

		w->Vector3(bone->Scale);
		w->Vector3(bone->Rotation);
		w->Vector3(bone->Translation);
		//todo : ���ʹϾ� Ȯ��
		w->Vector4(bone->Quaternion);
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
		auto start = (int)span.GetStart().GetFrameCount();
		auto end = (int)span.GetStop().GetFrameCount();

		if (start < end)
			ReadAnimation(*animation, _scene->GetRootNode(), start, end);

		animation->TotalFrame = (int)animation->KeyFrames[0]->Transform.size();

		_animations.emplace_back(move(animation));
	}
}

void Fbx::Exporter::ReadAnimation(OUT FbxAnimation& animation, FbxNode *node, int start, int end)
{
	auto attribute = node->GetNodeAttribute();

	if (attribute != nullptr)
	{
		auto nodeType = attribute->GetAttributeType();
		if (nodeType == FbxNodeAttribute::eSkeleton)
		{
			auto keyFrame = CreateKeyFrame(node, start, end);
			animation.KeyFrames.emplace_back(move(keyFrame));
		}
	}

	for (int i = 0; i < node->GetChildCount(); i++)
		ReadAnimation(animation, node->GetChild(i), start, end);
}

unique_ptr<Fbx::FbxKeyFrame> Fbx::Exporter::CreateKeyFrame(FbxNode * node, int start, int end)
{
	auto keyFrame = make_unique<FbxKeyFrame>();
	keyFrame->BoneName = node->GetName();

	//�����Ӹ���
	for (int i = start; i <= end; i++)
	{
		FbxTime animationTime;

		//i��° ������ ������
		animationTime.SetFrame(i);

		//�ش� �ð��� ���� Ʈ������
		auto matrix = node->EvaluateLocalTransform(animationTime);
		auto transform = Utility::ToMatrix(matrix);

		auto data = CreateKeyFrameData(node, transform, animationTime);
		keyFrame->Transform.push_back(data);
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