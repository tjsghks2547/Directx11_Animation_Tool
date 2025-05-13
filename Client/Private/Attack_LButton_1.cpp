#include "pch.h"
#include "Attack_LButton_1.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Player.h"


CAttack_LButton_1::CAttack_LButton_1()
{
}

HRESULT CAttack_LButton_1::Initialize()
{
	return S_OK;
}

void CAttack_LButton_1::Priority_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
	/* Ű�� �ϳ� ������ �� �� ���ػ�ƾ��ҵ�.*/

	//if(GetKeyState('W') & 0x8000)
	 
	if (GetKeyState('W') & 0x8000)
	{
		_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
		_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

		_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
		_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;


		_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // ������ �ϸ� ���� 2�� ����� �ϴµ� ��.. �̰� ���߿� ����ϱ�. 

		// vLook�� ĳ������ look �� �������ϱ� 
		_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}


		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
		
		pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

	}



	if (GetKeyState('S') & 0x8000)
	{
		_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
		_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

		_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
		_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;


		_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight));

		// vLook�� ĳ������ look �� �������ϱ� 
		_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}


		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);
	
		pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

	}



	if (GetKeyState('A') & 0x8000)
	{
		_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
		_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

		_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
		_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;


		_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight)); // ������ �ϸ� ���� 2�� ����� �ϴµ� ��.. �̰� ���߿� ����ϱ�. 

		// vLook�� ĳ������ look �� �������ϱ� 
		_vector PlayerLeft = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT)) * -1.f;

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLeft));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLeft, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}


		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);

		pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

	}


	if (GetKeyState('D') & 0x8000)
	{

		_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
		_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
		_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

		_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

		_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight)); // ������ �ϸ� ���� 2�� ����� �ϴµ� ��.. �̰� ���߿� ����ϱ�.		

		// vLook�� ĳ������ look �� �������ϱ� 
		_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));
		_vector PlayerRight = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerRight));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerRight, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}


		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian);

		pGameObject->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, vCurPosition);

	}

	//dynamic_cast<CPlayer*>(pGameObject)->Set_ParentPhaseState(CPlayer::PHASE_IDLE); 

}

void CAttack_LButton_1::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

void CAttack_LButton_1::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{

}

CAttack_LButton_1* CAttack_LButton_1::Create()
{
	CAttack_LButton_1* pInstance = new CAttack_LButton_1();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : Attack_LButton_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAttack_LButton_1::Free()
{
	__super::Free();
}
