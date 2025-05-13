#include "pch.h"
#include "BackGround.h"
#include "GameInstance.h"


CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUIObject(pDevice,pContext)
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
    :CUIObject(Prototype)   
{
    /*TriangleTests::Intersects()*/
}

HRESULT CBackGround::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
{
    BACKGROUND_DESC* pDesc = static_cast<BACKGROUND_DESC*>(pArg);
    
    pDesc->fSizeX = g_iWinSizeX;
    pDesc->fSizeY = g_iWinSizeY;
    pDesc->fX = g_iWinSizeX >> 1;
    pDesc->fY = g_iWinSizeY >> 1;
    pDesc->fNear = 0.f;
    pDesc->fFar = 1.f;

    pDesc->fSpeedPerSec = XMConvertToRadians(90.f);
    pDesc->fRotationPerSec = 5.f; 

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;// GameObejct의 Initialize(void* pArg) 되고 
    /* Transform 만들어지고 Transform Initialize 함수 실행되고.*/

    if (FAILED(Ready_Component()))
        return E_FAIL; 


    return S_OK;
}

void CBackGround::Priority_Update(_float fTimeDelta)
{
    int a = 10; 
}

void CBackGround::Update(_float fTimeDelta)
{
    int a = 10;
}

void CBackGround::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this); 
   
    /*m_pTraingleCom->Draw()*/
}

HRESULT CBackGround::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShader->Begin(0);

    m_pVIBuffer_Rect_Com->Bind_InputAssembler();

    m_pVIBuffer_Rect_Com->Render();
    /* input Layout 안가져왔음 그리고 지금 인덱스버퍼 기준으로 드로우 되고있어서 안됨 수정해야함 */
    //m_pTriangleCom->Bind_Buffers();
    //m_pContext->Draw(3, 0);
    return S_OK;
}

HRESULT CBackGround::Ready_Component()
{
    /* Com_Texture */
    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    
    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
        TEXT("Com_Rect"), reinterpret_cast<CComponent**>(&m_pVIBuffer_Rect_Com))))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect_Shader"),
        TEXT("Com_Rect_Shader"), reinterpret_cast<CComponent**>(&m_pShader))))
        return E_FAIL;

    return S_OK;
}

HRESULT CBackGround::Bind_ShaderResources()
{
    
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShader,"g_WorldMatrix")))
        return E_FAIL; 

    if (FAILED(m_pShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;

    if (FAILED(m_pShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL; 

    if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShader, "g_Texture", 0)))
        return E_FAIL; 


    return S_OK;
}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBackGround* pInstance = new CBackGround(pDevice, pContext);

    if(FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBackGround::Clone(void* pArg)
{
    CBackGround* pInstance = new CBackGround(*this);

    if(FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBackGround");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBackGround::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pVIBuffer_Rect_Com);
    Safe_Release(m_pShader);
}
