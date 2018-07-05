#pragma once
#include "cBasicMap.h"

class GridBuffer;
class BrushBuffer;
class cHeightMap : public cBasicMap
{
public:
	cHeightMap(wstring file = L"");
	~cHeightMap();

	void Update();
	virtual void Render() override;
	void PostRender();
public:
	D3DXVECTOR2 GetCellNumber(const D3DXVECTOR3& start, const D3DXVECTOR3& direction);
private:
	cWorldBuffer* m_pWorld;
	class cMaterial* m_pMaterial;
};