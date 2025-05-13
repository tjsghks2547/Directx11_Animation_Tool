#include "pch.h"
#include "PlayerRun.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Navigation.h"

CPlayerRun::CPlayerRun()
{
}

HRESULT CPlayerRun::Initialize()
{
	return S_OK;
}

void CPlayerRun::Priority_Update(CGameObject* pGameObject, class CNavigation* pNavigation, _float fTimeDelta)
{
	_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
	_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);

	_vector vCurPosition = pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

	_vector vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	_vector CamRight = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]);
	_vector CamLeft = XMVector3Normalize(m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW).r[0]) * -1.f;

	_vector vLook = XMVector3Normalize(XMVector3Cross(CamRight, vUp)); // ������ �ϸ� ���� 2�� ����� �ϴµ� ��.. �̰� ���߿� ����ϱ�. 
	_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

	if ((GetKeyState('W') & 0x8000))
	{
		// vLook�� ĳ������ look �� �������ϱ� 
		//_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));	

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		//������ ���� ���ϱ�


		if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
		{
			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.5f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);

			else
			{
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f);
			}
		}
		else
		{
			if (m_pGameInstance->isKeyEnter(DIK_A))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // �̷��� �ȵǰ� ī�޶�����̳�

			if (GetKeyState('A') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);
			}
			if (m_pGameInstance->isKeyEnter(DIK_D))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f));

			if (GetKeyState('D') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);	
			}
		}

		//pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta, pNavigation);

		/* ���� ���� */

	}

	if ((GetKeyState('S') & 0x8000))
	{
		_vector vLook = XMVector3Normalize(XMVector3Cross(vUp, CamRight)); // ������ �ϸ� ���� 2�� ����� �ϴµ� ��.. �̰� ���߿� ����ϱ�. 

		// vLook�� ĳ������ look �� �������ϱ� 
		//_vector PlayerLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));

		float dotResult = XMVectorGetX(XMVector3Dot(vLook, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, vLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		//������ ���� ���ϱ�

		if (!(GetKeyState('A') & 0x8000) && !(GetKeyState('D') & 0x8000))
		{
			pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);

			if (abs(Radian) <= 0.5f)
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);

			else
			{
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed * 0.1f);
			}
		}

		else
		{
			if (m_pGameInstance->isKeyEnter(DIK_A))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // �̷��� �ȵǰ� ī�޶�����̳�

			if (GetKeyState('A') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);
			}

			if (m_pGameInstance->isKeyEnter(DIK_D))
				pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f));

			if (GetKeyState('D') & 0x8000)
			{
				pGameObject->Get_Transfrom()->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * MouseMoveX * 0.05f);
				pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);	
			}
		}

	}



	if (!(GetKeyState('W') & 0x8000)
		&& !(GetKeyState('S') & 0x8000)
		&& (GetKeyState('A') & 0x8000))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));		

		// ���� ������ ĳ���Ͱ� �̵��Ҷ� ������ �ٲ�ϱ� �� �� ���´� ����ҰŰ����� 
		float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamLeft, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), -Radian * m_fTurnSpeed * fTimeDelta);


		if (abs(Radian) <= 0.5f)
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);

		else
		{
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f);
		}


	}

	if (m_pGameInstance->isKeyPressed(DIK_A)
		&& m_pGameInstance->isKeyEnter(DIK_W))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));	

		// ���� ������ ĳ���Ͱ� �̵��Ҷ� ������ �ٲ�ϱ� �� �� ���´� ����ҰŰ����� 
		float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamLeft, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // �̷��� �ȵǰ� ī�޶�����̳�	

	}

	if (m_pGameInstance->isKeyPressed(DIK_A)
		&& m_pGameInstance->isKeyEnter(DIK_S)
		&& (!(GetKeyState('D') & 0x8000)))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


		// ���� ������ ĳ���Ͱ� �̵��Ҷ� ������ �ٲ�ϱ� �� �� ���´� ����ҰŰ����� 
		float dotResult = XMVectorGetX(XMVector3Dot(CamLeft, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamLeft, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // �̷��� �ȵǰ� ī�޶�����̳�	


	}



	if (!(GetKeyState('W') & 0x8000) &&
		!(GetKeyState('S') & 0x8000) &&
		(GetKeyState('D') & 0x8000))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));


		/* ���� ����� �Ǻ�*/


		float dotResult = XMVectorGetX(XMVector3Dot(CamRight, PlayerLook));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);

		_vector crossResult = XMVector3Cross(PlayerLook, CamRight);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), Radian * m_fTurnSpeed * fTimeDelta);


		if (abs(Radian) <= 0.5f)
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * m_fWalkSpeed);

		else
		{
			pGameObject->Get_Transfrom()->Go_Straight(fTimeDelta * 0.1f);
		}


	}

	if (m_pGameInstance->isKeyPressed(DIK_D)
		&& m_pGameInstance->isKeyEnter(DIK_W))
	{
		//_vector vLook = XMVector3Normalize(pGameObject->Get_Transfrom()->Get_State(CTransform::STATE_LOOK));	



		// ���� ������ ĳ���Ͱ� �̵��Ҷ� ������ �ٲ�ϱ� �� �� ���´� ����ҰŰ����� 
		float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, CamRight));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamRight, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-45.f)); // �̷��� �ȵǰ� ī�޶�����̳�	

	}


	if (m_pGameInstance->isKeyPressed(DIK_D)
		&& m_pGameInstance->isKeyEnter(DIK_S))
	{

		// ���� ������ ĳ���Ͱ� �̵��Ҷ� ������ �ٲ�ϱ� �� �� ���´� ����ҰŰ����� 
		float dotResult = XMVectorGetX(XMVector3Dot(PlayerLook, CamRight));
		dotResult = max(-1.0f, min(dotResult, 1.0f));
		float Radian = acosf(dotResult);


		_vector crossResult = XMVector3Cross(CamRight, PlayerLook);
		float crossY = XMVectorGetY(crossResult);
		if (crossY < 0.0f) {
			Radian = -Radian;
		}

		pGameObject->Get_Transfrom()->Turn_Degree(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(45.f)); // �̷��� �ȵǰ� ī�޶�����̳�	

	}
}

void CPlayerRun::Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

void CPlayerRun::Late_Update(CGameObject* pGameObject, CNavigation* pNavigation, _float fTimeDelta)
{
}

CPlayerRun* CPlayerRun::Create()
{
	CPlayerRun* pInstance = new CPlayerRun();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : PlayerRun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayerRun::Free()
{
	__super::Free();
}
