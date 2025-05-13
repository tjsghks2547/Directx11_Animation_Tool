#include "Cell.h"
#include "VIBuffer_Cell.h"
#include "GameInstance.h"
#include "Shader.h" 

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
    ,m_pGameInstance(CGameInstance::GetInstance())  
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CCell::Initialize(const _float3* pPoints)      
{
    memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);    
    
    m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);     
    if (nullptr == m_pVIBuffer)
        return E_FAIL;

    return S_OK;
}

HRESULT CCell::Render()
{

    m_pVIBuffer->Bind_InputAssembler(); 
    m_pVIBuffer->Render();  
   
    return S_OK;    
}


CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints)
{
    CCell* pInstance = new CCell(pDevice, pContext);

    if(FAILED(pInstance->Initialize(pPoints)))
    {
        MSG_BOX("Failed to Created : Cell");
        Safe_Release(pInstance);
    }
    
    return pInstance;
}

CGameObject* CCell::Clone(void* pArg)
{
    return nullptr; 
}

void CCell::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);  
    Safe_Release(m_pVIBuffer);  

}
