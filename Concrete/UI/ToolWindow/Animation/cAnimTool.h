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
		// cTool을(를) 통해 상속됨
		virtual void Init() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson(Json::Value& root) override;
	private:
		// iObserver을(를) 통해 상속됨
		virtual void SelectBone(weak_ptr<cModelBone> bone) override;
		virtual void SelectMesh(weak_ptr<cModelMesh> mesh) override;
		virtual void ChangeModel(weak_ptr<cModel> newModel) override;
	private:
		void SelectAnimator();

		void ShowAnimatorInspector();
		void OpenAnimationClip();
		string GetClipName();
		void ShowProgressInspector();

		//선택한 파일이 Fbx면 추출하고 변환한다
		//@param : 선택한 경로
		//@param : 추출하고 변환한 새로운 파일 경로(OUT)
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