#include "stdafx.h"
#include "cShader.h"

/**************************
		쉐이더 팩토리
***************************/
shared_ptr<cShader> cShader::Create(wstring shaderFile, string vsName, string psName)
{
	//쉐이더 캐시
	static unordered_map<wstring, weak_ptr<cShader>> cache;

	//캐시에서 쉐이더 검색
	auto shaderPtr = cache[shaderFile].lock();
	if (shaderPtr == nullptr)
	{
		//트릭 구조체
		struct make_shared_enabler : public cShader {
			make_shared_enabler(wstring shaderFile, string vsName, string psName) :cShader(shaderFile, vsName, psName) {}
		};
		shaderPtr = make_shared<make_shared_enabler>(shaderFile, vsName, psName);
		cache[shaderFile] = shaderPtr;
	}

	return shaderPtr;
}

void cShader::Render()
{
	D3D::GetDC()->IASetInputLayout(mInputLayout);
	D3D::GetDC()->VSSetShader(mVertexShader, nullptr, 0);
	D3D::GetDC()->PSSetShader(mPixelShader, nullptr, 0);
}

cShader::cShader(wstring shaderFile, string vsName, string psName)
	:mShaderName(shaderFile)
	, mVsName(vsName)
	, mPsName(psName)
{
	CreateVertexShader();
	CreatePixelShader();
}

cShader::~cShader()
{
	SAFE_RELEASE(mReflection);
	SAFE_RELEASE(mInputLayout);
	SAFE_RELEASE(mVertexShader);
	SAFE_RELEASE(mPixelShader);
}

/*********************
	정점 쉐이더 생성
**********************/
void cShader::CreateVertexShader()
{
	ID3DBlob *blob = nullptr;
	CheckShaderError(&blob, mVsName, "vs");

	HRESULT hr = D3D::GetDevice()->CreateVertexShader
	(
		blob->GetBufferPointer()
		, blob->GetBufferSize()
		, nullptr
		, &mVertexShader
	);
	assert(SUCCEEDED(hr) && "정점 쉐이더 생성 에러");

	//인풋 레이아웃 생성
	CreateInputLayout(blob);

	SAFE_RELEASE(blob);
}

/*********************
	픽셀 쉐이더 생성
**********************/
void cShader::CreatePixelShader()
{
	ID3DBlob *blob = nullptr;
	CheckShaderError(&blob, mPsName, "ps");

	HRESULT hr = D3D::GetDevice()->CreatePixelShader
	(
		blob->GetBufferPointer()
		, blob->GetBufferSize()
		, nullptr
		, &mPixelShader
	);
	assert(SUCCEEDED(hr) && "픽셀 쉐이더 생성 에러");

	SAFE_RELEASE(blob);
}

/*******************************
파일 생성시 오류 체크 및 blob 반환
********************************/
void cShader::CheckShaderError(ID3DBlob** outBlob, string name, string type)
{
	ID3DBlob *error = nullptr;
	/*HRESULT hr = D3DX11CompileFromFile
	(
		mShaderName.c_str(), nullptr, nullptr, name.c_str(), (type + "_5_0").c_str()
		, D3D10_SHADER_ENABLE_STRICTNESS, 0, nullptr
		, outBlob, &error, nullptr
	);*/

	HRESULT hr = D3DCompileFromFile
	(
		mShaderName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, name.c_str(), (type + "_5_0").c_str()
		, 0, 0, outBlob, &error
	);


	if (FAILED(hr))
	{
		if (error != NULL)
		{
			string str = (const char *)error->GetBufferPointer();
			MessageBoxA(NULL, str.c_str(), "Shader Error", MB_OK);
			assert(false); 
		}
	}
}

void cShader::CreateInputLayout(ID3DBlob *vertexBlob)
{
	HRESULT hr;
	hr = D3DReflect
	(
		vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, IID_ID3D11ShaderReflection
		, (void**)&mReflection
	);
	assert(SUCCEEDED(hr));

	D3D11_SHADER_DESC shaderDesc;
	mReflection->GetDesc(&shaderDesc);

	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (UINT i = 0; i< shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		mReflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string temp = paramDesc.SemanticName;
		if (temp == "POSITION")
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		inputLayoutDesc.push_back(elementDesc);
	}

	hr = D3D::GetDevice()->CreateInputLayout
	(
		&inputLayoutDesc[0]
		, inputLayoutDesc.size()
		, vertexBlob->GetBufferPointer()
		, vertexBlob->GetBufferSize()
		, &mInputLayout
	);
	assert(SUCCEEDED(hr));
}
