#include "stdafx.h"
#include "cModelAnimPlayer.h"
#include "cModel.h"
#include "./ModelPart/cModelAnimClip.h"
#include "./ModelPart/ModelKeyFrames.h"
#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"
#include "./Graphic/cMaterial.h"
#include "./Graphic/ConstBuffer/cModelBoneBuffer.h"

cModelAnimPlayer::cModelAnimPlayer(weak_ptr<cModel> model)
	:model(model)
	, mode(Mode::Play)
	, currentKeyFrame(0)
	, frameTime(0.0f)
	, keyFrameFactor(0.0f)
	, nextKeyFrame(0)
	, bUseSlerp(true)
{
	auto modelPtr = model.lock();
	shader = cShader::Create(Shader + L"999_Animation.hlsl");

	auto materials = modelPtr->GetMaterials();
	for (auto&& material : materials)
		material->SetShader(shader);

	currentClip = modelPtr->GetClip(0);

	auto root = modelPtr->GetBone(1);
	GatherBones(bones, root);

	skinTransform.assign(bones.size(), D3DXMATRIX());
	boneAnimation.assign(bones.size(), D3DXMATRIX());
}

cModelAnimPlayer::~cModelAnimPlayer()
{
}

void cModelAnimPlayer::Update()
{
	if (currentClip.lock() == nullptr || mode != Mode::Play)
		return;

	UpdateTime();
	UpdateBone();

	model.lock()->GetBuffer().lock()->SetBones(&skinTransform[0], skinTransform.size());
}

void cModelAnimPlayer::Render()
{
	model.lock()->GetBuffer().lock()->SetVSBuffer(2);

	for (auto&& mesh : model.lock()->GetMeshes())
		mesh->Render();
}

void cModelAnimPlayer::UpdateTime()
{
	auto clipPtr = currentClip.lock();

	//스피드 조절 가능
	frameTime += cFrame::Delta();

	float invFrameRate = 1.0f / clipPtr->GetFrameRate();
	while (frameTime > invFrameRate)
	{
		int keyFrameCount = clipPtr->GetTotalFrame();

		//현재 프레임수
		currentKeyFrame = (currentKeyFrame + 1) % keyFrameCount;

		//더해진 값에 하나를 더 더함
		nextKeyFrame = (currentKeyFrame + 1) % keyFrameCount;

		frameTime -= invFrameRate;
	}

	//지금 프레임부터 다음 프레임까지의 비율
	keyFrameFactor = frameTime / invFrameRate;
}

void cModelAnimPlayer::UpdateBone()
{
	int index = 0;

	if (skinTransform.size() < 1)
		skinTransform.assign(bones.size(), D3DXMATRIX());

	if (boneAnimation.size() < 1)
		boneAnimation.assign(bones.size(), D3DXMATRIX());

	for (auto&& bone : bones)
	{
		auto bonePtr = bone.lock();
		//애니메이션 될 행렬
		D3DXMATRIX matAnimation;

		//부모와 계산될 행렬
		D3DXMATRIX matParentAnimation;

		//자신의 글로벌 행렬
		D3DXMATRIX matInvBindPose = bonePtr->AbsoluteTransform();
		D3DXMatrixInverse(&matInvBindPose, nullptr, &matInvBindPose);

		auto frame = currentClip.lock()->GetKeyframe(bonePtr->GetName());
		if (bUseSlerp == true)
		{
			//구형 보간
			D3DXMATRIX S, R, T;

			sModelKeyFrameData current = frame->FrameData[currentKeyFrame];
			sModelKeyFrameData next = frame->FrameData[nextKeyFrame];

			D3DXVECTOR3 s1 = current.Scale;
			D3DXVECTOR3 s2 = next.Scale;

			D3DXVECTOR3 s;
			D3DXVec3Lerp(&s, &s1, &s2, keyFrameFactor);
			D3DXMatrixScaling(&S, s.x, s.y, s.z);


			D3DXQUATERNION q1 = current.Rotation;
			D3DXQUATERNION q2 = next.Rotation;

			D3DXQUATERNION q;
			D3DXQuaternionSlerp(&q, &q1, &q2, keyFrameFactor);
			D3DXMatrixRotationQuaternion(&R, &q);


			D3DXVECTOR3 t1 = current.Translation;
			D3DXVECTOR3 t2 = next.Translation;

			D3DXVECTOR3 t;
			D3DXVec3Lerp(&t, &t1, &t2, keyFrameFactor);
			D3DXMatrixTranslation(&T, t.x, t.y, t.z);

			matAnimation = S * R * T;
		}
		else
		{
			matAnimation = frame->FrameData[currentKeyFrame].Transform;
		}

		//처음에 하나 추가했던 노드는 제외하므로 -1
		int parentIndex = bonePtr->GetParentIndex() - 1;
		if (parentIndex < 0)
			D3DXMatrixIdentity(&matParentAnimation);
		else
			matParentAnimation = boneAnimation[parentIndex];

		boneAnimation[index] = matAnimation * matParentAnimation;
		skinTransform[index] = matInvBindPose * boneAnimation[index];

		index++;
	}
}

void cModelAnimPlayer::GatherBones(vector<weak_ptr<cModelBone>>& bones, weak_ptr<cModelBone> bone)
{
	bones.push_back(bone);

	auto bonePtr = bone.lock();
	for (UINT i = 0; i < bonePtr->ChildCount(); i++)
		GatherBones(bones, bonePtr->Child(i));
}
