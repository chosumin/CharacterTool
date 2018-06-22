#pragma once

class cShader
{
public:
	static shared_ptr<cShader> Create(wstring shaderFile
											, string vsName = "VS"
											, string psName = "PS");
	~cShader();
public:
	wstring GetFileName() { return mShaderName; }
	void Render();
protected:
	cShader(wstring shaderFile, string vsName, string psName);
private:
	void CreateVertexShader();
	void CreatePixelShader();
	void CheckShaderError(ID3DBlob** outBlob, string name, string type);
	void CreateInputLayout(ID3DBlob *vertexBlob);
private:
	wstring mShaderName;
	string mVsName;
	string mPsName;

	ID3D11VertexShader *mVertexShader;
	ID3D11PixelShader *mPixelShader;

	ID3D11InputLayout *mInputLayout;

	ID3D11ShaderReflection *mReflection;
};