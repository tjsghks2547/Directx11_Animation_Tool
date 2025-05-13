#include "pch.h" 
#include "Boss_Varg.h"
#include "GameInstance.h"
#include "Body_Varg.h"
#include "Animation.h"

CBoss_Varg::CBoss_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{

}

CBoss_Varg::CBoss_Varg(const CBoss_Varg& Prototype)
	:CContainerObject(Prototype)
{

}

HRESULT CBoss_Varg::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CBoss_Varg::Initialize(void* pArg)
{
	strcpy_s(m_szName, "PLAYER");

	CGameObject::GAMEOBJECT_DESC        Desc{};

	Desc.fSpeedPerSec = 45.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	///* 루트 모션 애니메션 코드 */
	//m_pRootMatrix = m_pModel->Get_RootMotionMatrix("kaku");		


	// 시작 지점의 플레이어 위치 1_23일 
	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f,0.f,0.f,1.f });


	m_pTransformCom->Scaling(_float3{ 0.01f, 0.01f, 0.01f });

	return S_OK;
}

void CBoss_Varg::Priority_Update(_float fTimeDelta)
{

	__super::Priority_Update(fTimeDelta);
}

void CBoss_Varg::Update(_float fTimeDelta)
{
	_vector		vCurPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);
	_vector test = { 0.f,0.f,0.f,1.f };
	/* 루트 모션 애니메션 코드 */
	m_pRootMatrix = m_pModel->Get_RootMotionMatrix("root");

	if (!XMVector4Equal(XMLoadFloat4x4(m_pRootMatrix).r[3], test) && m_pModel->Get_LerpFinished() && g_bRootOnOff)
	{
		m_pTransformCom->Set_MulWorldMatrix(m_pRootMatrix);
	}

	_vector		vPosition = m_pTransformCom->Get_State(CTransform::STATE_POSITION);

	__super::Update(fTimeDelta);

}

void CBoss_Varg::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	/* 잠시 네비게이션 정확히 타는지 확인할려고 넣는 코드 */
#ifdef _DEBUG
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
	m_fTimeDelta = fTimeDelta;

}

HRESULT CBoss_Varg::Render()
{
#ifdef _DEBUG
	//m_pNavigationCom->Render();	
	//m_pColliderCom->Render();	
#endif 

	return S_OK;
}

HRESULT CBoss_Varg::Ready_Components()
{

	return S_OK;
}

HRESULT CBoss_Varg::Ready_PartObjects()
{
	CBody_Varg::BODY_PLAYER_DESC BodyDesc{};		

	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	BodyDesc.fSpeedPerSec = 0.f;
	//BodyDesc.fSpeedPerSec = 1.f;
	BodyDesc.fRotationPerSec = 0.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Varg"), &BodyDesc)))
		return E_FAIL;

	m_pModel = dynamic_cast<CModel*>(Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));

	return S_OK;
}


void CBoss_Varg::OnCollisionEnter(CGameObject* _pOther)
{

}


void CBoss_Varg::OnCollision(CGameObject* _pOther)
{

}

void CBoss_Varg::OnCollisionExit(CGameObject* _pOther)
{

}

CBoss_Varg* CBoss_Varg::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBoss_Varg* pInstance = new CBoss_Varg(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CBoss_Varg");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBoss_Varg::Clone(void* pArg)
{
	CBoss_Varg* pInstance = new CBoss_Varg(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CBoss_Varg");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_Varg::Free()
{
	__super::Free();
}
