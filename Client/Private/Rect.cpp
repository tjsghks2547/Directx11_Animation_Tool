#include "pch.h"
#include "Rect.h"
#include "GameInstance.h"

CRect::CRect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CRect::CRect(const CRect& Prototype)
    :CGameObject(Prototype)
{
}

HRESULT CRect::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL; 

    return S_OK;
}

HRESULT CRect::Initialize(void* pArg)
{
    GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
    pDesc->fRotationPerSec = XMConvertToRadians(90.f);
    pDesc->fSpeedPerSec = 5.f; 


    /* 카메라가 없으니 임시로 설정*/
    
    //XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    //XMStoreFloat4x4(&m_ProjMatrix, XMMatrixIdentity());
   
    //float Aspect = float(g_iWinSizeX) / float(g_iWinSizeY);

    //XMStoreFloat4x4(&m_ViewMatrix, XMMatrixLookAtLH(XMVectorSet(0.f, 1.f, -1.f, 1.f), XMVectorSet(0.f, 0.f, 0.f, 1.f), XMVectorSet(0.f, 1.f, 0.f, 0.f)));
    //XMStoreFloat4x4(&m_ProjMatrix, XMMatrixPerspectiveFovLH(90.f, Aspect, 0.1f, 100.f));
   

    /* 여기서 transform 항등행렬 되고 */
    if (FAILED(__super::Initialize(pDesc)))
        return E_FAIL;

    if (FAILED(Ready_Component()))
        return E_FAIL;   
     
    

    return S_OK;
}

void CRect::Priority_Update(_float fTimeDelta)
{
}

void CRect::Update(_float fTimeDelta)
{
}

void CRect::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CRect::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShader->Begin(1);

    m_pVIBufferCom->Bind_InputAssembler();

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CRect::Ready_Component()
{
    /* Com_Texture */
    //if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture"),
    //    TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
    //    return E_FAIL;


    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_Rect"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Shader_Brush"),
        TEXT("Com_Rect_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
        return E_FAIL;

    return S_OK;
}

HRESULT CRect::Bind_ShaderResources()
{

    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader, "g_WorldMatrix")))
        return E_FAIL;

    if(FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    
    if(FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    //if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
    //    return E_FAIL;

    //if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
    //    return E_FAIL;

    /*if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL;*/

    return S_OK;    
}

CRect* CRect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRect* pInstance = new CRect(pDevice, pContext);

    if(FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Create : CRect");
        Safe_Release(pInstance);
    }

    return pInstance;   
}

CGameObject* CRect::Clone(void* pArg)
{
    CRect* pInstance = new CRect(*this);

    if(FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CRect");
        Safe_Release(pInstance);
    }

    return pInstance;   
}

void CRect::Free()
{
    __super::Free();

    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pShader);

}
