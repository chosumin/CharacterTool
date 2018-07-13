#pragma once
#include "../cTool.h"

class cModel;
class cModelMesh;
namespace UI
{
	class cModelTool : public cTool
	{
	public:
		cModelTool();
		~cModelTool();
	public:
		// cTool을(를) 통해 상속됨
		virtual void Update() override;
		virtual void Render() override;
		virtual void ShowHierarchy() override;
		virtual void ShowInspector() override;
	private:
		void ShowMeshHierarchy();
		void ShowModelInspector();
		bool AlertModel();

		void OpenFile();
		void OpenModel(wstring path, wstring name);
		void ExportFbx(wstring path, OUT wstring *newPath);
	private:
		weak_ptr<cModel> _model;
		string _modelName;
	};
}