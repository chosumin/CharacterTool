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
		// cTool��(��) ���� ��ӵ�
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson(Json::Value& root) override;
	public:
		// iObserver��(��) ���� ��ӵ�
		virtual void SelectBone(weak_ptr<cModelBone> bone) override;
		virtual void SelectMesh(weak_ptr<cModelMesh> mesh) override;
		virtual void ChangeModel(weak_ptr<cModel> newModel) override;
	private:
		/********************
			���̾��Ű ����
		*********************/

		//Hierarchy�� �ݶ��̴� ��ư
		void SelectCollider();
	private:
		/******************
			�ν����� ����
		*******************/

		//�ݶ��̴� ���� â
		void ShowAddWindowInspector();
		
		//�ݶ��̴� ���� ��ư
		void AddCollider();

		//�ݶ��̴� ����Ʈ �ν�����
		void ShowColliderInspector(eColliderType type, int& index);

		//�ݶ��̴� ����Ʈ
		void ShowColliders(eColliderType type, weak_ptr<cModelBone> bone, int& index);
	private:
		/**********
			����
		***********/

		//���ͳ� ���� ���ٸ� ����� ����Ѵ�.
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