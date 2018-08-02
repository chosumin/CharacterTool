#pragma once
#include "../cTool.h"
#include "../Model/iObserver.h"

class cAnimClip;
class cAnimator;
namespace UI
{
	class cModelTool;
	class cAnimTool :
		public cTool,
		public iObserver
	{
	public:
		cAnimTool(weak_ptr<cModelTool> modelTool);
		~cAnimTool();
	public:
		// cTool��(��) ���� ��ӵ�
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson(Json::Value& root) override;
	private:
		// iObserver��(��) ���� ��ӵ�
		virtual void SelectBone(weak_ptr<cModelBone> bone) override;
		virtual void SelectMesh(weak_ptr<cModelMesh> mesh) override;
		virtual void ChangeModel(weak_ptr<cModel> newModel) override;
	private:
		void SelectAnimator();

		void ShowAnimatorInspector();
		void OpenAnimationClip();
		string GetClipName();
		void ShowProgressInspector();

		//������ ������ Fbx�� �����ϰ� ��ȯ�Ѵ�
		//@param : ������ ���
		//@param : �����ϰ� ��ȯ�� ���ο� ���� ���(OUT)
		void ExportFbx(wstring path, OUT wstring *newPath);

		void ShowAnimationList();

		bool Alert();
	private:
		weak_ptr<cModel> _model;
		weak_ptr<cModelTool> _modelTool;
		weak_ptr<cAnimClip> _selectedClip;
		weak_ptr<cAnimator> _animator;
	};
}