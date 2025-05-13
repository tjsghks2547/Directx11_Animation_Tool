#include "Shader.h"

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent(pDevice,pContext)
{


}

CShader::CShader(const CShader& Prototype)
    :CComponent(Prototype)
    , m_pEffect(Prototype.m_pEffect)
    , m_iNumPasses(Prototype.m_iNumPasses)
    , m_InputLayouts(Prototype.m_InputLayouts)
{

    Safe_AddRef(m_pEffect);
    
    for (auto& pID3D11InputLayout : m_InputLayouts)
    {
        Safe_AddRef(pID3D11InputLayout);
    }
  
}



HRESULT CShader::Initialize_Prototype(const wstring& ShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
    _uint  iHlsFlag = { 0 };

#ifdef _DEBUG
    iHlsFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION; 
#else
    iHlsFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1; 
#endif


    if(FAILED(D3DX11CompileEffectFromFile(ShaderFilePath.c_str(),
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        iHlsFlag,
        0,
        m_pDevice,
        &m_pEffect,
        nullptr)))
    {
        return E_FAIL;
    }

    ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(0);
    if (nullptr == pTechnique)
        return E_FAIL; 

    D3DX11_TECHNIQUE_DESC TechniqueDesc{};

    pTechnique->GetDesc(&TechniqueDesc); // pTechnique의 정보를 TechniqueDesc에 채워 넣음 
      
    m_iNumPasses = TechniqueDesc.Passes;    

    for(size_t i = 0; i< m_iNumPasses; i++)
    {
        ID3DX11EffectPass* pPass = pTechnique->GetPassByIndex(i);

        D3DX11_PASS_DESC PassDesc{};

        pPass->GetDesc(&PassDesc); // pPass의 정보를 PassDesc에 채워 넣음

        ID3D11InputLayout* pInputLayout = { nullptr };

        if (FAILED(m_pDevice->CreateInputLayout(pElements,
            iNumElements,
            PassDesc.pIAInputSignature, 
            PassDesc.IAInputSignatureSize, 
            &pInputLayout)))
            return E_FAIL;
        /* 셰이더의 내용과 정점과의 내용비교를 하는것 셰이더의 내용은 PassDesc에서 다룸*/
        /* input layout을 만드는 이유가 셰이더와 호환이 되는지 확인하기 위해서이다.*/

        m_InputLayouts.push_back(pInputLayout); 
    }

    return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{

    return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex)
{
     if (nullptr == m_InputLayouts[iPassIndex])
        return E_FAIL; 
    
    m_pContext->IASetInputLayout(m_InputLayouts[iPassIndex]);

    
    ID3DX11EffectPass* pPass = m_pEffect->GetTechniqueByIndex(0)->GetPassByIndex(iPassIndex);
    if (nullptr == pPass)
        return E_FAIL;

    /* 이 쉐이더에 이 패스로 그립니다*/
    /* 쉐이더에 전달해야할 모든 데이터를 다 던져놓아야한다.*/
    pPass->Apply(0,m_pContext);

    return S_OK;    
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pValule, _uint iLength)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;

    return pVariable->SetRawValue(pValule, 0, iLength);
}


HRESULT CShader::Bind_Matrix(const _char* pConstantName, const _float4x4* pMatrix)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (pVariable == nullptr)
        return E_FAIL;

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (nullptr == pMatrixVariable)
        return E_FAIL;


    return pMatrixVariable->SetMatrix((_float*)pMatrix);    
}


HRESULT CShader::Bind_Matrices(const _char* pConstantName, const _float4x4* pMatrices, _uint iNumMatrices)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (pVariable == nullptr)
        return E_FAIL; 

    ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
    if (pMatrixVariable == nullptr)
        return E_FAIL;

    return pMatrixVariable->SetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader::Bind_SRV(const _char* pConstantName, ID3D11ShaderResourceView* pSRV)
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
    if (nullptr == pVariable)
        return E_FAIL;
 
    
    ID3DX11EffectShaderResourceVariable* pShaderResourceVariable = pVariable->AsShaderResource();
    if (pShaderResourceVariable == nullptr)
        return E_FAIL;

    return pShaderResourceVariable->SetResource(pSRV);
}

HRESULT CShader::Bind_Bool(const _char* pConstantName, bool value)  
{
    ID3DX11EffectVariable* pVariable = m_pEffect->GetConstantBufferByName(pConstantName);
    if (pVariable == nullptr)
        return E_FAIL; 

    ID3DX11EffectScalarVariable* pBoolVariable = pVariable->AsScalar();
    if (pBoolVariable == nullptr)
        return E_FAIL;

    return pBoolVariable->SetBool(value);
}


CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& pShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{

    CShader* pInstance = new CShader(pDevice, pContext);

    if(FAILED(pInstance->Initialize_Prototype(pShaderFilePath,pElements,iNumElements)))
    {
        MSG_BOX("Create Failed : Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CShader::Clone(void* pArg)
{
    CShader* pInstance = new CShader(*this);

    if(FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Cloned Failed : Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CShader::Free()
{
    __super::Free();
   
    
    for(auto& pID3D11InputLayout : m_InputLayouts)
    {
        Safe_Release(pID3D11InputLayout);
    }

    m_InputLayouts.clear();
    
    Safe_Release(m_pEffect);    
}
