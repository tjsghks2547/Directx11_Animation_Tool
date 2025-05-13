#include "Camera.h"
#include "GameInstance.h"

CCamera::CCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject(pDevice,pContext)
{
}

CCamera::CCamera(const CCamera& Prototype)
    :CGameObject(Prototype) 
{
}

HRESULT CCamera::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
    CAMERA_DESC* pDesc = (CAMERA_DESC*)pArg;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL; 

    /* 카메라의 트랜스폼에 카메라의 초기 상태를 셋팅한다.*/
    /* 카메라의 월드행렬의 역행렬 == 뷰 스페이스변환 행렬*/

    m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMLoadFloat4(&pDesc->vEye));
    m_pTransformCom->LookAt(XMLoadFloat4(&pDesc->vAt));

    m_fFovy   = pDesc->fFovy;
    m_fAspect = pDesc->fAspect;
    m_fNear   = pDesc->fNear;
    m_fFar    = pDesc->fFar;

    return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
    m_pGameInstance->Set_Transform(CPipeLine::D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInverse());
    m_pGameInstance->Set_Transform(CPipeLine::D3DTS_PROJ, XMMatrixPerspectiveFovLH(m_fFovy, m_fAspect, m_fNear, m_fFar));
}

void CCamera::Update(_float fTimeDelta )
{

 

}

void CCamera::Late_Update(_float fTimeDelta)
{
}

CGameObject* CCamera::Clone(void* pArg)
{
    return S_OK;
}

void CCamera::Free()
{
    __super::Free();
}
