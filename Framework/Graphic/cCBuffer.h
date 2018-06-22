#pragma once
#include "stdafx.h"

class cCBuffer
{
public:
	void SetVSBuffer(UINT slot)
	{
		MapData(mData, mDataSize);

		D3D::GetDC()->VSSetConstantBuffers(slot, 1, &mBuffer);
	}

	void SetPSBuffer(UINT slot)
	{
		MapData(mData, mDataSize);

		D3D::GetDC()->PSSetConstantBuffers(slot, 1, &mBuffer);
	}
protected:
	cCBuffer(void *data, UINT dataSize)
		: mData(data), mDataSize(dataSize)
	{
		D3D11_BUFFER_DESC desc;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = dataSize;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		HRESULT hr = D3D::GetDevice()->CreateBuffer(&desc, NULL, &mBuffer);
		assert(SUCCEEDED(hr));
	}

	virtual ~cCBuffer()
	{
		SAFE_RELEASE(mBuffer);
	}
private:
	void MapData(void *data, UINT dataSize)
	{
		D3D11_MAPPED_SUBRESOURCE subResource;
		HRESULT hr = D3D::GetDC()->Map
		(
			mBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource
		);
		memcpy(subResource.pData, data, dataSize);
		D3D::GetDC()->Unmap(mBuffer, 0);
	}
private:
	ID3D11Buffer *mBuffer;
	void *mData;
	UINT mDataSize;
};