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

	//ios 세팅
	_ios = FbxIOSettings::Create(_manager, IOSROOT);
	_ios->SetBoolProp(IMP_FBX_TEXTURE, true);
	_manager->SetIOSettings(_ios);

	//importer 세팅
	_importer = FbxImporter::Create(_manager, "");
	auto sFile = cString::String(fbxFile);
	bool bCheck = _importer->Initialize(sFile.c_str(), -1, _ios);
	assert(bCheck == true && "임포터 초기화 오류!");
	bCheck = _importer->Import(_scene);
	assert(bCheck == true && "씬 불러오는 도중 오류 발생!");

	//converter 생성
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

	//Fbx 머터리얼 컨테이너 순회
	for (int i = 0; i < count; i++)
		PushMaterials(i);

	//사용자 머터리얼 정보 파일 저장할 폴더 경로 생성
	if (cPath::ExistDirectory(saveFolder) == false)
		CreateDirectory(saveFolder.c_str(), nullptr);

	//Json 루트 노드 생성
	Json::Value root;

	//사용자 머터리얼 컨테이너 순회
	for (auto&& material : _materials)
		WriteMaterialDataOnJson(root, saveFolder, move(material));
	
	//Json 저장
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
	//한 프레임의 시간 가져옴
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

	//사용자 머터리얼 객체 생성
	auto material = make_unique<FbxMaterial>();
	material->Name = fbxMaterial->GetName();

	//램버트 정보 삽입
	if (fbxMaterial->GetClassId().Is(FbxSurfaceLambert::ClassId))
	{
		auto lambert = (FbxSurfaceLambert*)fbxMaterial;

		material->Ambient = Fbx::Utility::ToColor(lambert->Ambient, lambert->AmbientFactor);
		material->Diffuse = Fbx::Utility::ToColor(lambert->Diffuse, lambert->DiffuseFactor);
		material->Emissive = Fbx::Utility::ToColor(lambert->Emissive, lambert->EmissiveFactor);
	}

	//퐁 정보 삽입
	if (fbxMaterial->GetClassId().Is(FbxSurfacePhong::ClassId))
	{
		auto phong = (FbxSurfacePhong*)fbxMaterial;

		material->Specular = Fbx::Utility::ToColor(phong->Specular, phong->SpecularFactor);
		material->Shininess = static_cast<float>(phong->Shininess);
	}

	//머터리얼 텍스쳐 파일 정보 삽입
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

	//todo : 쉐이더 파일 바뀌어야 될지도?
	string shaderName = "999_Mesh.hlsl";
	Json::SetValue(val, "ShaderName", shaderName);

	Json::SetValue(val, "Ambient", material->Ambient);
	Json::SetValue(val, "Diffuse", material->Diffuse);
	Json::SetValue(val, "Emissive", material->Emissive);
	Json::SetValue(val, "Specular", material->Specular);

	Json::SetValue(val, "Shininess", material->Shininess);

	//파일 복사, json에 경로 저장
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
	//파일 없으면 종료
	if (textureFile.length() < 1)
	{
		//cDebugLog::Log(&typeid(this), []() { cout << "존재하지 않는 파일명" << endl; });
		return;
	}

	auto file = cString::Wstring(textureFile);
	auto fileName = cPath::GetFileName(file);

	//파일 복사
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

	//자식 노드 재귀
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

			////////////////////////디포머/////////////////////////

			int deformerCount = mesh->GetDeformerCount();

			//가중치는 정점 내부의 자료이므로 컨트롤 포인트 개수와 같다.
			vector<FbxBoneWeights> boneWeights(mesh->GetControlPointsCount(), FbxBoneWeights());

			for (int i = 0; i < deformerCount; i++)
				PushBoneWeight(mesh, boneWeights, i);

			for (UINT i = 0; i < boneWeights.size(); i++)
				boneWeights[i].Normalize();

			//todo : 확인 요망, 이 루프의 목적은 무엇일까?
			vector<FbxBoneWeights> meshBoneWeights(boneWeights.size(), FbxBoneWeights());
			for (UINT i = 0; i < boneWeights.size(); i++)
				meshBoneWeights[i].AddBoneWeights(boneWeights[i]);

			///////////////////////정점///////////////////////////
			vector<shared_ptr<FbxVertex>> vertices;

			//삼각형 개수만큼 순회
			for (int p = 0; p < polygonCount; p++)
			{
				int vertexInPolygon = mesh->GetPolygonSize(p);
				assert(vertexInPolygon == 3 && "폴리곤 개수 오류!");

				//삼각형 정점 순회
				for (int vi = 0; vi < vertexInPolygon; vi++)
				{
					vertices.emplace_back(move(CreatePolygonVertex(mesh, p, vi, meshBoneWeights)));
				}
			}

			_meshes.emplace_back(CreateMeshData(node, parent, vertices));
		}
	}

	//자식 노드 재귀
	for (int i = 0; i < node->GetChildCount(); i++)
		ReadMeshData(node->GetChild(i), _meshes.size(), index);
}

void Fbx::Exporter::PushBoneWeight(FbxMesh * mesh, OUT vector<struct FbxBoneWeights>& boneWeights, UINT i)
{
	auto deformer = mesh->GetDeformer(i, FbxDeformer::eSkin);

	//스킨 타입으로 다운캐스팅
	auto skin = reinterpret_cast<FbxSkin*>(deformer);
	if (skin == nullptr)
		return;

	//디포머 내부의 클러스터 순회
	for (int clusterIndex = 0; clusterIndex < skin->GetClusterCount(); clusterIndex++)
	{
		auto cluster = skin->GetCluster(clusterIndex);
		assert(cluster->GetLinkMode() == FbxCluster::eNormalize);

		//joint 번호 받음
		string linkName = cluster->GetLink()->GetName();
		UINT jointIndex = GetJointIndexByName(linkName);

		//조인트 로컬, 월드 행렬 가져옴
		FbxAMatrix transform, absoluteTransform;
		cluster->GetTransformMatrix(transform);
		cluster->GetTransformLinkMatrix(absoluteTransform);

		_joints[jointIndex]->Transform = Utility::ToMatrix(transform);
		_joints[jointIndex]->AbsoluteTransform = Utility::ToMatrix(absoluteTransform);

		//블렌드 정보를 얻기 위하여 컨트롤 포인트 번호 순회
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

	//todo : 굳이 저장 안해도될거같은데
	vertex->ControlPoint = cpIndex;

	//위치 삽입
	FbxVector4 position = mesh->GetControlPointAt(cpIndex);
	vertex->Vertex.position = Fbx::Utility::ToVector3(position);

	//법선 삽입
	FbxVector4 normal;
	mesh->GetPolygonVertexNormal(p, vi, normal);
	normal.Normalize();
	vertex->Vertex.normal = Fbx::Utility::ToVector3(normal);

	//머터리얼 이름 삽입
	vertex->MaterialName = Fbx::Utility::GetMaterialName(mesh, p, cpIndex);

	//UV 삽입
	int uvIndex = mesh->GetTextureUVIndex(p, vi);
	vertex->Vertex.uv = Fbx::Utility::GetUv(mesh, cpIndex, uvIndex);

	//가중치 삽입
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

	//메쉬 데이터에 메쉬 파트 삽입
	for (int i = 0; i < _scene->GetMaterialCount(); i++)
	{
		auto meshPart = move(CreateMeshPart(i, vertices));
		if (meshPart)
			meshData->MeshParts.emplace_back(move(meshPart));
	}

	//로컬 행렬
	FbxAMatrix matrix = node->EvaluateLocalTransform();
	meshData->Transform = Utility::ToMatrix(matrix);

	//월드 행렬
	matrix = node->EvaluateGlobalTransform();
	meshData->AbsoluteTransform = Utility::ToMatrix(matrix);

	return move(meshData);
}

unique_ptr<Fbx::FbxMeshPartData> Fbx::Exporter::CreateMeshPart(int i, const vector<shared_ptr<FbxVertex>>& vertices)
{
	auto material = _scene->GetMaterial(i);
	string materialName = material->GetName();

	//머터리얼별로 컨테이너에 묶음
	vector<weak_ptr<FbxVertex>> gather;
	for (auto&& temp : vertices)
	{
		if (temp->MaterialName == materialName)
			gather.push_back(temp);
	}

	if (gather.size() < 1)
		return nullptr;

	//사용자 정점, 인덱스 생성
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
		bone->Index = i; //부모 메쉬 데이터가 참조하는 값
		bone->Parent = _meshes[i]->ParentBone;
		bone->Name = _meshes[i]->Name;
		bone->Transform = _meshes[i]->Transform;

		//렌더링이 될 주체는 본이다
		_meshes[i]->ParentBone = bone->Index;

		_bones.emplace_back(move(bone));
	}
}

void Fbx::Exporter::CreateBoneFromJointData()
{
	if (_joints.size() < 1)
		return;

	//본 데이터 푸쉬
	D3DXMATRIX matrix;
	D3DXMatrixIdentity(&matrix);
	auto bone = CreateBoneData(0, -1, "RootNode", matrix, matrix);
	_bones.emplace_back(move(bone));
	//본 트리
	for (UINT i = 0; i < _joints.size(); i++)
	{
		//todo : 오류날시에 부모 번호를 원래대로 변경
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

	//메쉬 노드
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

		//애니메이션 가져옴
		auto takeInfo = _importer->GetTakeInfo(i);
		animation->Name = takeInfo->mName.Buffer();

		//시간 계산
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

		//1초 기준으로 프레임을 끊는다.
		animationTime.SetSecondDouble(time);

		//해당 시간의 로컬 트랜스폼
		auto matrix = node->EvaluateLocalTransform(animationTime);
		auto transform = Utility::ToMatrix(matrix);

		auto data = CreateKeyFrameData(node, transform, animationTime);
		keyFrame->Transform.push_back(data);

		//1프레임 - 1/24를 하나의 프레임으로 생각한다.
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

	//로컬 스케일 가져옴
	auto scale = node->EvaluateLocalScaling(animationTime);
	data.Scale = Utility::ToVector3(scale);

	return data;
}

void Fbx::Exporter::WriteAnimation(wstring saveFolder, wstring saveName)
{
	//존재하지 않는 경로면 새로 생성
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