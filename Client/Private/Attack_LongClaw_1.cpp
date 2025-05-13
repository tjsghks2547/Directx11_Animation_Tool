#include "pch.h"
#include "Attack_LongClaw_1.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"


CAttack_LongClaw_1::CAttack_LongClaw_1()
{
}

HRESULT CAttack_LongClaw_1::Initialize()
{
	return S_OK;
}

void CAttack_LongClaw_1::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLookFront = XMVector3Normalize(XMVector3Cross(CamRight, vUp));
	_vector vLookBack = XMVector3Normalize(XMVector3Cross(vUp, CamRight));

	// vLook과 캐릭터의 look 의 내적구하기 
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));




}

void CAttack_LongClaw_1::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CAttack_LongClaw_1::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CAttack_LongClaw_1* CAttack_LongClaw_1::Create()
{
	CAttack_LongClaw_1* pInstance = new CAttack_LongClaw_1();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Attack_LongClaw_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttack_LongClaw_1::Free()
{
	__super::Free();
}
