#include "stdafx.h"
#include "cModelAnimPlayer.h"
#include "cModel.h"

#include "./ModelPart/cModelAnimClip.h"
#include "./ModelPart/ModelKeyFrames.h"
#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"

#include "./Graphic/cMaterial.h"
#include "./Graphic/ConstBuffer/cModelBoneBuffer.h"

#include "./Transform/sTransform.h"

cModelAnimPlayer::cModelAnimPlayer(weak_ptr<cModel> model)
	: model(model)
	, mode(Mode::Play)
	, currentKeyFrame(0)
	, frameTime(0.0f)
	, keyFrameFactor(0.0f)
	, nextKeyFrame(0)
	, bUseSlerp(true)
{
	shader = cShader::Create(Shader + L"999_Animation.hlsl");

	auto modelPtr = model.lock();
	auto materials = modelPtr->GetMaterials();
	for (auto&& material : materials)
		material->SetShader(shader);

	currentClipNumber = 0;
	currentClip = modelPtr->GetClip(currentClipNumber);

	skinTransform.assign(modelPtr->GetBoneCount(), D3DXMATRIX());
	boneAnimation.assign(modelPtr->GetBoneCount(), D3DXMATRIX());
}

cModelAnimPlayer::~cModelAnimPlayer()
{
}

UINT cModelAnimPlayer::GetClip() const
{
	return currentClipNumber;
}

void cModelAnimPlayer::SetClip(UINT clipNumber)
{
	currentClip = model.lock()->GetClip(clipNumber);
	currentClipNumber = clipNumber;
	currentKeyFrame = 0;
	nextKeyFrame = 1;
}

void cModelAnimPlayer::Update(weak_ptr<sTransform> actorWorld)
{
	if (currentClip.lock() == nullptr || mode != Mode::Play)
		return;

	UpdateTime();
	UpdateBone(actorWorld);

	model.lock()->GetBuffer().lock()->SetBones(&skinTransform[0], skinTransform.size());
}

void cModelAnimPlayer::Render()
{
	if (model.expired())
		return;

	auto modelPtr = model.lock();
	modelPtr->GetBuffer().lock()->SetVSBuffer(2);

	for (auto&& mesh : modelPtr->GetMeshes())
		mesh->Render();
}

void cModelAnimPlayer::PostRender()
{
	if (model.expired())
		return;

	ImGui::Text("Animation");
	ImGui::SliderInt("Frame", &currentKeyFrame, 0, 30);
}

void cModelAnimPlayer::UpdateTime()
{
	auto clipPtr = currentClip.lock();

	float invFrameRate = 1.0f / clipPtr->GetFrameRate();
	//스피드 조절 가능

	frameTime += cFrame::Delta();
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

void cModelAnimPlayer::UpdateBone(weak_ptr<sTransform> actorWorld)
{
	auto modelPtr = model.lock();
	for (UINT i = 0; i < modelPtr->GetBoneCount(); i++)
	{
		auto bonePtr = modelPtr->GetBone(i).lock();
		//애니메이션 될 행렬
		D3DXMATRIX matAnimation;

		//부모와 계산될 행렬
		D3DXMATRIX matParentAnimation;

		//자신의 글로벌 행렬
		D3DXMATRIX matInvBindPose = bonePtr->AbsoluteTransform();
		D3DXMatrixInverse(&matInvBindPose, nullptr, &matInvBindPose);

		auto frame = currentClip.lock()->GetKeyframe(bonePtr->GetName());
		if (frame == nullptr)
			return;

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

		int parentIndex = bonePtr->GetParentIndex();

		//조작할 때 이 부분을 바꿔라
		if (parentIndex < 0)
			matParentAnimation = actorWorld.lock()->Matrix;
		else
			matParentAnimation = boneAnimation[parentIndex];

		boneAnimation[i] = matAnimation * matParentAnimation;
		
		//test : 콜라이더 부모 테스트
		bonePtr->SetTransform(boneAnimation[i]);
		skinTransform[i] = matInvBindPose * boneAnimation[i];
	}
}