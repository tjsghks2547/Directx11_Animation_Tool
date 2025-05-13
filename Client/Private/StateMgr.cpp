#include "pch.h"
#include "StateMgr.h"
#include "PlayerIdle.h"
#include "PlayerRun.h"

#pragma region ��Ŭ�� ���� 
#include "Attack_LButton_1.h"	
#include "Attack_LButton_2.h"	
#include "Attack_LButton_3.h"	
#pragma endregion 

#pragma region ��Ŭ�� ���� 
#include "Attack_LongClaw_1.h"		
#include "Attack_LongClaw_2.h"		
#pragma endregion	


CStateMgr::CStateMgr()
{

}

HRESULT CStateMgr::Initialize()
{
	m_vecState.reserve(30);


	/* 0�� Idle ���� */
	CPlayerIdle* pStateIdle = CPlayerIdle::Create();
	m_vecState.push_back(pStateIdle);


	/* 1�� Run ������ �ٴ� ���� �ִϸ��̼�  W Ű */
	CPlayerRun* pStateRun = CPlayerRun::Create();
	m_vecState.push_back(pStateRun);

	/* 2�� Attack ��Ŭ�� ���� ù ��° ��� LButton Ű */
	CAttack_LButton_1* pAttackLButton_1 = CAttack_LButton_1::Create();
	m_vecState.push_back(pAttackLButton_1);

	/* 3�� Attack ��Ŭ�� ���� �� ��° ��� LButton Ű */
	CAttack_LButton_2* pAttackLButton_2 = CAttack_LButton_2::Create();
	m_vecState.push_back(pAttackLButton_2);

	/* 4�� Attack ��Ŭ�� ���� �� ��° ��� LButton Ű */
	CAttack_LButton_3* pAttackLButton_3 = CAttack_LButton_3::Create();
	m_vecState.push_back(pAttackLButton_3);

	/* 5�� Attack ��Ŭ�� ���� ù ��° ��� RButton Enter Ű */
	CAttack_LongClaw_1* pAttack_LongClaw_1 = CAttack_LongClaw_1::Create();	
	m_vecState.push_back(pAttack_LongClaw_1);	

	/* 6�� Attack ��Ŭ�� ���� �� ��° ��� RButton Enter Ű */
	CAttack_LongClaw_2* pAttack_LongClaw_2 = CAttack_LongClaw_2::Create();	
	m_vecState.push_back(pAttack_LongClaw_2);	

	return S_OK;
}

CStateMgr* CStateMgr::Create()
{
	CStateMgr* pInstance = new CStateMgr();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CStateMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStateMgr::Free()
{
	__super::Free();

	for (auto& iter : m_vecState)
	{
		Safe_Release(iter);
	}

	m_vecState.clear();

}
