#include "pch.h"
#include "CubeObject.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CCubeObject::CCubeObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CCubeObject::CCubeObject(const CCubeObject& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CCubeObject::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCubeObject::Initialize(void* pArg)
{
    GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);   


    if (pDesc == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_CubeObject");

    else
        strcpy_s(m_szName, pDesc->szName);  


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    
    m_pTransformCom->Set_Scale(0.0001f, 0.0001f, 0.0001f);


    
    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->_fPosition));
    
    if(pDesc->_ParentMatrix != nullptr)
        XMStoreFloat4x4(&m_CameraMatrix, XMLoadFloat4x4(pDesc->_ParentMatrix));    

   
   
    m_fCameraRelativePos   = pDesc->_fRelativePos;
    if(pDesc->_fLerpTime != 0.f)
        m_fCameraLerpTime      = pDesc->_fLerpTime;
    if (pDesc->_fLerpTimeSpeed != 0.f)
        m_fCameraLerpTimeSpeed = pDesc->_fLerpTimeSpeed;

    m_iLerpMethod = pDesc->_iLerpMethod;
  

    return S_OK;
}

void  CCubeObject::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CCubeObject::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void  CCubeObject::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CCubeObject::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint  iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        /*  if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
              return E_FAIL;

          if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
              return E_FAIL;*/

        m_pShaderCom->Begin(2);

        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT  CCubeObject::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CubeObject"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CCubeObject::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}



void CCubeObject::OnCollisionEnter(CGameObject* _pOther)
{

}

void CCubeObject::OnCollision(CGameObject* _pOther)
{
}

void CCubeObject::OnCollisionExit(CGameObject* _pOther)
{
}

CCubeObject* CCubeObject::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCubeObject* pInstance = new  CCubeObject(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CCubeObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCubeObject::Clone(void* pArg)
{
    CCubeObject* pInstance = new  CCubeObject(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CCubeObject");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CCubeObject::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
