#pragma once
#include "../cTool.h"
#include "../Model/iObserver.h"

class cActorColliders;
enum class eColliderType;
enum class eColliderShape;
namespace UI
{
	class cModelTool;
	class cColliderTool :
		public cTool,
		public iObserver
	{
	public:
		cColliderTool(weak_ptr<cModelTool> modelTool);
		~cColliderTool();
	public:
		// cTool을(를) 통해 상속됨
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson(Json::Value& root) override;
	public:
		// iObserver을(를) 통해 상속됨
		virtual void SelectBone(weak_ptr<cModelBone> bone) override;
		virtual void SelectMesh(weak_ptr<cModelMesh> mesh) override;
		virtual void ChangeModel(weak_ptr<cModel> newModel) override;
	private:
		/********************
			하이어라키 관련
		*********************/

		//Hierarchy의 콜라이더 버튼
		void SelectCollider();
	private:
		/******************
			인스펙터 관련
		*******************/

		//콜라이더 생성 창
		void ShowAddWindowInspector();
		
		//콜라이더 생성 버튼
		void AddCollider();

		//콜라이더 리스트 인스펙터
		void ShowColliderInspector(eColliderType type, int& index);

		//콜라이더 리스트
		void ShowColliders(eColliderType type, weak_ptr<cModelBone> bone, int& index);
	private:
		/**********
			공통
		***********/

		//액터나 모델이 없다면 경고문을 출력한다.
		bool Alert();
		bool BoneAlert();
	private:
		weak_ptr<cModelTool> _modelTool;
		weak_ptr<cModelBone> _selectedBone;
		weak_ptr<cModel> _model;
		weak_ptr<cActorColliders> _colliders;

		int _shapeNumber;
		bool _selectAttack;
	};
}