#pragma once
#include <memory>
#include <vector>

class cModelBone;
class cModelMesh;
class cModel;
namespace UI
{
	class iObserver
		: public inheritable_enable_shared_from_this<iObserver>
	{
	public:
		virtual ~iObserver() {}

		virtual void SelectBone(weak_ptr<cModelBone> bone) = 0;
		virtual void SelectMesh(weak_ptr<cModelMesh> mesh) = 0;
		virtual void ChangeModel(weak_ptr<cModel> newModel) = 0;
	protected:
		iObserver() {}
	};
}