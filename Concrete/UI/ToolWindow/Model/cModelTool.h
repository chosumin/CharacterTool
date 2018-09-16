#pragma once
#include "../cTool.h"

enum class TextureType;
struct sTransform;
class cModel;
class cModelMesh;
class cModelBone;
class cMaterial;
class cTexture;
namespace UI
{
	class iObserver;
	class cModelTool : public cTool
	{
	public:
		cModelTool();
		~cModelTool();
	public:
		void RegisterObserver(weak_ptr<iObserver> observer);
	public:
		// cTool을(를) 통해 상속됨
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
		virtual void SaveJson(Json::Value& root) override;
		virtual void LoadJson() override;
	private:
		/********************
			하이어라키 관련
		*********************/

		//모델 버튼을 띄우고 선택 여부 체크
		void SelectModel();

		//메쉬 선택시 계층 구조를 띄운다.
		void ShowMeshHierarchy();

		//본 선택시 계층 구조를 띄운다
		//@param : 선택한 본
		void ShowBoneHierarchy(weak_ptr<cModelBone> bone);

		//본 버튼을 띄우고 선택 여부 체크
		//@param : 선택할 본의 이름
		//@param : 선택할 본의 트랜스폼
		void SelectBone(string boneName,
						weak_ptr<cModelBone> bone);

		//옵저버에게 선택된 본을 전달한다.
		//@param : 선택된 본
		void NotifySelectedBone(weak_ptr<cModelBone> bone);

		//옵저버에게 선택된 메쉬를 전달한다.
		//@param : 선택된 메쉬
		void NotifySelectedMesh(weak_ptr<cModelMesh> mesh);
	private:
		/***********************
			모델 인스펙터 관련
		************************/

		//본 선택시 본 트랜스폼을 띄운다
		void ShowTransformInspector();

		//모델 인스펙터를 띄운다
		void ShowModelInspector();

		//모델 파일 다이얼로그를 불러오고 모델을 연다
		void OpenFile();

		//옵저버에게 모델 변경을 통지한다
		void NotifyChangeModel();

		//선택한 파일이 Fbx면 추출하고 변환한다
		//@param : 선택한 경로
		//@param : 추출하고 변환한 새로운 파일 경로(OUT)
		void ExportFbx(wstring path, OUT wstring *newPath);

		//변환된 파일을 연다
		//@param : 파일 경로
		//@param : 파일명
		void OpenModel(wstring path, wstring name);
	private:
		/**************************
			머티리얼 인스펙터 관련
		***************************/
		
		//현재 머티리얼 정보 저장
		void SaveMaterial();

		//머티리얼에 존재하는 텍스쳐명 저장
		void SaveTextureFile(Json::Value& root, const string & jsonName, const shared_ptr<cMaterial>& material, TextureType type);

		//머티리얼 정보 보여줌
		void ShowMaterials();

		void OpenShaderFile(shared_ptr<cMaterial> material);

		//Ambient, Diffuse, Specular, Shineness 보여주기
		void ShowTextureColors(int& index, shared_ptr<cMaterial> material);

		//텍스쳐 파일 보여주기
		void ShowTexture(shared_ptr<cTexture> texturePtr);
	private:
		/**********
			공통
		***********/
		
		void ShowText(string name, string text);
		//모델 파일이 현재 없다면 경고문을 출력한다
		bool AlertModel();
	private:
		bool _selectSubTool;
		weak_ptr<cModel> _model;
		string _modelName;

		vector<weak_ptr<iObserver>> _observers;
	};
}