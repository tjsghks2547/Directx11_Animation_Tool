#include "pch.h" 
#include "Player.h"
#include "GameInstance.h"
#include "Body_Player.h"
#include "RightWeapon.h"
#include "LeftWeapon.h"
#include "Animation.h"
#include "StateMgr.h"

CPlayer::CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CContainerObject(pDevice, pContext)
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	:CContainerObject(Prototype)
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;


	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
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


	m_pTransformCom->Scaling(_float3{ 0.0025f, 0.0025f, 0.0025f });


	m_pStateMgr = CStateMgr::Create();
	if (m_pStateMgr == nullptr)
	{
		MSG_BOX("Failed to Created : StateMgr");
	}

	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

#pragma region Mouse_Input
	/* 마우스 입력키는 (LB, RB ) 동시 입력을 불가능 하도록 설정 */

	//if (m_pGameInstance->isMouseEnter(DIM_LB))
	//{
	//	if (m_iState == STATE_ATTACK_L1
	//		&& (m_pModel->Get_CurrentAnmationTrackPosition() > 15.f
	//			&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
	//	{
	//		m_pStateMgr->Get_VecState().at(3)->Priority_Update(this, nullptr, fTimeDelta);
	//		m_iState = STATE_ATTACK_L2;
	//	}
	//
	//	else if (m_iState == STATE_ATTACK_L2
	//		&& (m_pModel->Get_CurrentAnmationTrackPosition() > 25.f
	//			&& m_pModel->Get_CurrentAnmationTrackPosition() < 50.f))
	//	{
	//		m_pStateMgr->Get_VecState().at(4)->Priority_Update(this, nullptr, fTimeDelta);
	//		m_iState = STATE_ATTACK_L3;
	//	}
	//
	//	else
	//	{
	//		if (m_iState != STATE_ATTACK_L1 && m_iState != STATE_ATTACK_L2 && m_iState != STATE_ATTACK_L3)
	//		{
	//			m_pStateMgr->Get_VecState().at(2)->Priority_Update(this, nullptr, fTimeDelta);
	//			m_iState = STATE_ATTACK_L1;
	//		}
	//
	//	}
	//
	//	m_iPhaseState = PHASE_FIGHT;
	//}
	//
	//else if (m_pGameInstance->isMouseEnter(DIM_RB))
	//{
	//	if (m_iState == STATE_ATTACK_LONG_CLAW_01
	//		&& (m_pModel->Get_CurrentAnmationTrackPosition() > 30.f	
	//			&& m_pModel->Get_CurrentAnmationTrackPosition() < 110.f))	
	//	{
	//		m_pStateMgr->Get_VecState().at(6)->Priority_Update(this, nullptr, fTimeDelta);	
	//		m_iState = STATE_ATTACK_LONG_CLAW_02;
	//	}
	//
	//	else if (m_iState != STATE_ATTACK_LONG_CLAW_01
	//		&& m_iState != STATE_ATTACK_LONG_CLAW_02)
	//	{
	//		m_pStateMgr->Get_VecState().at(5)->Priority_Update(this, nullptr, fTimeDelta);	
	//		m_iState = STATE_ATTACK_LONG_CLAW_01;
	//	}
	//
	//	m_iPhaseState = PHASE_FIGHT;
	//}



	// 그럼 각 행동이 부모의 phase state에 대한 포인터를 가지고 있어야 겠다. 

#pragma endregion 
#pragma region KeyBoard Input


#pragma region 8방향 Run 
	//if (m_iPhaseState != PHASE_FIGHT)
	//{
	//	if ((GetKeyState('W') & 0x8000) || (GetKeyState('S') & 0x8000) || (GetKeyState('A') & 0x8000) || (GetKeyState('D') & 0x8000))
	//	{
	//		m_pStateMgr->Get_VecState().at(1)->Priority_Update(this, nullptr, fTimeDelta);
	//		m_iState = STATE_RUN;
	//	}
	//
	//	else
	//	{
	//		if (m_iState == STATE_RUN)
	//		{
	//			m_pStateMgr->Get_VecState().at(0)->Priority_Update(this, nullptr, fTimeDelta);
	//			m_iState = STATE_IDLE;
	//		}
	//	}
	//
	//	m_iPhaseState = PHASE_IDLE;
	//}
#pragma endregion 


#pragma endregion 


	__super::Priority_Update(fTimeDelta);
}

void CPlayer::Update(_float fTimeDelta)
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


	if (m_pGameInstance->isKeyEnter(DIK_4))
	{
		m_pTransformCom->Turn_Degree(_fvector{ 0.f,1.f,0.f,0.f }, XMConvertToRadians(90.f));
	}


	__super::Update(fTimeDelta);

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);

	/* 잠시 네비게이션 정확히 타는지 확인할려고 넣는 코드 */
#ifdef _DEBUG
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
#endif
	m_fTimeDelta = fTimeDelta;

}

HRESULT CPlayer::Render()
{
#ifdef _DEBUG
	//m_pNavigationCom->Render();	
	//m_pColliderCom->Render();	
#endif 

	return S_OK;
}

HRESULT CPlayer::Ready_Components()
{

	return S_OK;
}

HRESULT CPlayer::Ready_PartObjects()
{
	CBody_Player::BODY_PLAYER_DESC BodyDesc{};

	BodyDesc.pParentState = &m_iState;	
	BodyDesc.pParentPhaseState = &m_iPhaseState;	
	BodyDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();	
	BodyDesc.fSpeedPerSec = 0.f;	
	//BodyDesc.fSpeedPerSec = 1.f;
	BodyDesc.fRotationPerSec = 0.f;	

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"), &BodyDesc)))
		return E_FAIL;

	m_pModel = dynamic_cast<CModel*>(Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));	


	/* 오른손 무기를 만든다. */
	CRightWeapon::WEAPON_DESC		RightWeaponDesc{};

	CModel* pBodyModelCom = dynamic_cast<CModel*>(__super::Find_PartObject_Component(TEXT("Part_Body"), TEXT("Com_Model")));
	if (nullptr == pBodyModelCom)
		return E_FAIL;


	RightWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_r"); /* 캐릭터 모델마다 다름 */
	RightWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	RightWeaponDesc.fSpeedPerSec = 0.f;
	RightWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Right_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Weapon"), &RightWeaponDesc)))
		return E_FAIL;


	/* 오른손 무기를 만든다. */
	CLeftWeapon::WEAPON_DESC		LeftWeaponDesc{};


	LeftWeaponDesc.pSocketMatrix = pBodyModelCom->Get_BoneMatrix("weapon_l"); /* 캐릭터 모델마다 다름 */
	LeftWeaponDesc.pParentWorldMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
	LeftWeaponDesc.fSpeedPerSec = 0.f;
	LeftWeaponDesc.fRotationPerSec = 10.f;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Left_Weapon"), LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Left_Weapon"), &LeftWeaponDesc)))
		return E_FAIL;

	return S_OK;
}


void CPlayer::OnCollisionEnter(CGameObject* _pOther)
{

}


void CPlayer::OnCollision(CGameObject* _pOther)
{

}

void CPlayer::OnCollisionExit(CGameObject* _pOther)
{

}

CPlayer* CPlayer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer* pInstance = new CPlayer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pStateMgr);	
}
