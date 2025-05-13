#include "pch.h"
#include "CameraModel.h"
#include "Model.h"
#include "Shader.h"
#include "GameInstance.h"


CCameraModel::CCameraModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice, pContext)
{
}

CCameraModel::CCameraModel(const CCameraModel& Prototype)
    :CGameObject(Prototype)
{

}

HRESULT CCameraModel::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CCameraModel::Initialize(void* pArg)
{

    if (m_szName == nullptr)
        strcpy_s(m_szName, "Prototype_GameObject_CameraModel"); 


    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_pTransformCom->Set_Scale(0.001f, 0.001f, 0.001f);


    m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f,3.f,0.f,1.f });

    

    return S_OK;
}

void  CCameraModel::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void  CCameraModel::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void  CCameraModel::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT  CCameraModel::Render()
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

        m_pShaderCom->Begin(1);

        m_pModelCom->Render(i);
    }

    return S_OK;
}

HRESULT  CCameraModel::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CameraModel"),    
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CCameraModel::Bind_ShaderResources()
{
    if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;


    return S_OK;
}



void CCameraModel::OnCollisionEnter(CGameObject* _pOther)
{
   
}

void CCameraModel::OnCollision(CGameObject* _pOther)
{
}

void CCameraModel::OnCollisionExit(CGameObject* _pOther)
{
}

CCameraModel* CCameraModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCameraModel* pInstance = new  CCameraModel(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CCameraModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCameraModel::Clone(void* pArg)
{
    CCameraModel* pInstance = new  CCameraModel(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CCameraModel");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void  CCameraModel::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
