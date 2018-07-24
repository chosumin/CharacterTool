#pragma once
#include "../cTool.h"

struct sTransform;
class cModel;
class cModelMesh;
class cModelBone;
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
		// cTool��(��) ���� ��ӵ�
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy(int i) override;
		virtual void ShowInspector() override;
	private:
		/********************
			���̾��Ű ����
		*********************/

		//�� ��ư�� ���� ���� ���� üũ
		void SelectModel();

		//�޽� ���ý� ���� ������ ����.
		void ShowMeshHierarchy();

		//�� ���ý� ���� ������ ����
		//@param : ������ ��
		void ShowBoneHierarchy(weak_ptr<cModelBone> bone);

		//�� ��ư�� ���� ���� ���� üũ
		//@param : ������ ���� �̸�
		//@param : ������ ���� Ʈ������
		void SelectBone(string boneName,
						weak_ptr<cModelBone> bone);

		//���������� ���õ� ���� �����Ѵ�.
		//@param : ���õ� ��
		void NotifySelectedBone(weak_ptr<cModelBone> bone);

		//���������� ���õ� �޽��� �����Ѵ�.
		//@param : ���õ� �޽�
		void NotifySelectedMesh(weak_ptr<cModelMesh> mesh);
	private:
		/******************
			�ν����� ����
		*******************/

		//�� ���ý� �� Ʈ�������� ����
		void ShowTransformInspector();

		//�� �ν����͸� ����
		void ShowModelInspector();

		//�� ���� ���̾�α׸� �ҷ����� ���� ����
		void OpenFile();

		//���������� �� ������ �����Ѵ�
		void NotifyChangeModel();

		//������ ������ Fbx�� �����ϰ� ��ȯ�Ѵ�
		//@param : ������ ���
		//@param : �����ϰ� ��ȯ�� ���ο� ���� ���(OUT)
		void ExportFbx(wstring path, OUT wstring *newPath);

		//��ȯ�� ������ ����
		//@param : ���� ���
		//@param : ���ϸ�
		void OpenModel(wstring path, wstring name);

		//�� �ν����Ϳ� �޽� ������ ����
		void ShowMeshInfo();
	private:
		/**********
			����
		***********/

		//�� ������ ���� ���ٸ� ������� ����Ѵ�
		bool AlertModel();
	private:
		bool _selectSubTool;
		weak_ptr<cModel> _model;
		string _modelName;

		vector<weak_ptr<iObserver>> _observers;
	};
}