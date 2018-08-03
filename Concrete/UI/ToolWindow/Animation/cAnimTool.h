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
		virtual void LoadJson() override;

		virtual void SetActor(weak_ptr<cActor> actor) override;
	private:
		// iObserver을(를) 통해 상속됨
		virtual void SelectBone(weak_ptr<cModelBone> bone) override;
		virtual void SelectMesh(weak_ptr<cModelMesh> mesh) override;
		virtual void ChangeModel(weak_ptr<cModel> newModel) override;
	private:
		/********************
			하이어라키 관련
		*********************/

		//애니메이터 버튼 클릭
		void SelectAnimator();

	private:
		/******************
			인스펙터 관련
		*******************/

		//애니메이터 정보 띄움
		void ShowAnimatorInspector();

		//클립 추가 버튼 클릭
		void OpenAnimationClip();

		//선택한 파일이 Fbx면 추출하고 변환한다
		//@param : 선택한 경로
		//@param : 추출하고 변환한 새로운 파일 경로(OUT)
		void ExportFbx(wstring path, OUT wstring *newPath);

		//클립 생성
		//@param : 경로
		//@param : 파일명
		void CreateClip(const wstring& directory, const wstring& name);

		string GetClipName();

		//시작, 정지, 일시정지, 프로그레스바 띄움
		void ShowProgressInspector();

		//애니메이션 리스트 띄움
		void ShowAnimationList();

		bool Alert();
	private:
		weak_ptr<cModel> _model;
		weak_ptr<cModelTool> _modelTool;
		weak_ptr<cAnimClip> _selectedClip;
		weak_ptr<cAnimator> _animator;
	};
}