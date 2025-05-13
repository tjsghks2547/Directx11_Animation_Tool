#include "Cube.h"
#include "Shader.h"
#include "VIBuffer_Cube.h" 
#include "GameInstance.h"

CCube::CCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    ,m_pContext(pContext)
    ,m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CCube::Initialize(_float3* pPoints)     
{

    m_pShaderCom = CShader::Create(m_pDevice, m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_EngineVtxCube.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
    if (nullptr == m_pShaderCom)    
        return E_FAIL;
    
    m_pVIBufferCom = CVIBuffer_Cube::Create(m_pDevice, m_pContext); 

    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());
    
    m_WorldMatrix._11 = 1.5f;
    m_WorldMatrix._22 = 1.5f;
    m_WorldMatrix._33 = 1.5f;

    m_WorldMatrix._41 = pPoints->x; 
    m_WorldMatrix._42 = pPoints->y; 
    m_WorldMatrix._43 = pPoints->z; 
    
    

    return S_OK;
}

HRESULT CCube::Render()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix))) 
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))  
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))  
        return E_FAIL;

    
    m_pShaderCom->Begin(m_iPass);        

    m_pVIBufferCom->Bind_InputAssembler();
    m_pVIBufferCom->Render();   


    return S_OK;    
}

CCube* CCube::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _float3* pPoints)
{
    CCube* pInstance = new CCube(pDevice, pContext);

    if(FAILED(pInstance->Initialize(pPoints)))  
    {
        MSG_BOX("Failed to Created : CCube");
        Safe_Release(pInstance);
    }
    
    return pInstance;   
}

void CCube::Free()
{
    __super::Free();


    Safe_Release(m_pShaderCom); 
    Safe_Release(m_pVIBufferCom);       
    Safe_Release(m_pGameInstance);  
    Safe_Release(m_pDevice);            
    Safe_Release(m_pContext);       
}
