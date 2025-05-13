#include "Navigation.h"
#include "Cell.h"
#include "Cube.h"   
#include "Shader.h"
#include "GameInstance.h"

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent(pDevice,pContext)
{
    
}

HRESULT CNavigation::Initialize()
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());    

     m_pShader = CShader::Create(m_pDevice,m_pContext, TEXT("../../EngineSDK/Hlsl/Shader_Cell.hlsl"), VTXPOS::Elements, VTXPOS::iNumElements);
    if (nullptr == m_pShader)
        return E_FAIL; 

    return S_OK;    
}


HRESULT CNavigation::Add_Cell(_float3* pPoints)
{
    /* 여기서 관리해야겠네. */
    
    for (int i = 0; i < 3; i++)
    {
        if (Find_Cube(&pPoints[i]))
            continue;
            
        else
        {
            CCube* pCube = CCube::Create(m_pDevice, m_pContext, &pPoints[i]);
            m_vecCubes.push_back(pCube);
        }
    }
    
    CCell* pCell = CCell::Create(m_pDevice, m_pContext, pPoints);       

              
    m_Cells.push_back(pCell);       

    return S_OK;
}

_bool CNavigation::Find_Cube(_float3* pPoints)  
{
    for (auto& pCubes : m_vecCubes)      
    {
        if (pCubes->Get_WorldMatrix()->_41 == pPoints->x &&   
            pCubes->Get_WorldMatrix()->_42 == pPoints->y &&   
            pCubes->Get_WorldMatrix()->_43 == pPoints->z)     
        {
            return true;       
        }
        
    }

    return false; 
}

HRESULT CNavigation::Render()
{
    if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    
    m_pShader->Begin(m_iPass);   
    
    for (auto& pCell : m_Cells)
        pCell->Render();    

    for (auto& pCube : m_vecCubes) 
        pCube->Render();       


    return S_OK;    
}


CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CNavigation* pInstance = new CNavigation(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))    
    {
        MSG_BOX("Failed to Created : Navigation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{

    /* 순수 가상 함수 때문에 설계*/
    return S_OK;
}


void CNavigation::Free()
{
    __super::Free();    

    for (auto& pCell : m_Cells) 
        Safe_Release(pCell);    

    for (auto& pCube : m_vecCubes)
        Safe_Release(pCube);


    m_Cells.clear();    
    m_vecCubes.clear();


    Safe_Release(m_pShader);

}
