#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader final : public CComponent
{
private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext); 
	CShader(const CShader& Prototype);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& ShaderFilePath,const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg) override; 

public:
	HRESULT Begin(_uint iPassIndex);
	HRESULT Bind_RawValue(const _char* pConstantName, const void* pValule, _uint iLength);
	HRESULT Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix);
	HRESULT Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices);
	HRESULT Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV);/*SRV�� Shader Resource View�� �ǹ� ( �ؽ�ó�� ���̷�Ʈ���� ����ϱ� ���� ������ ����ȯ����)*/
	HRESULT Bind_Bool(const _char* pConstantName, bool value);
	

	


private:
	ID3DX11Effect* m_pEffect = { nullptr };
	_uint          m_iNumPasses = { 0 };

	vector<ID3D11InputLayout*>  m_InputLayouts;

public:
	static CShader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& pShaderFilePath,const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements);
	virtual CComponent* Clone(void* pArg = nullptr); /* CComponet�� �ִ� ������ GameObject�� �� �����̳��� Second CCompnents �Ǿ��ֱ⶧��*/
	virtual void Free() override; 
};

END