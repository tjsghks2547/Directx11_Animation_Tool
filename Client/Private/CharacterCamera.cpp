#include "pch.h" 
#include "CharacterCamera.h"	
#include "GameInstance.h"
#include "Player.h"
#include "Layer.h"

CCharacterCamera::CCharacterCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CCamera{ pDevice, pContext }
{

}

CCharacterCamera::CCharacterCamera(const CCharacterCamera& Prototype)
	: CCamera(Prototype)
{

}

HRESULT CCharacterCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCharacterCamera::Initialize(void* pArg)
{
	FREE_CAMERA_DESC* pDesc = static_cast<FREE_CAMERA_DESC*>(pArg);	

	m_fMouseSensor = pDesc->fMouseSensor;

	if (FAILED(__super::Initialize(pDesc)))
		return E_FAIL;

	/* �÷��̾ ���� ���̾ ���ϱ�*/
	/* �÷��̾� ��ġ�� �������� �ɵ�*/


	return S_OK;
}


void CCharacterCamera::Priority_Update(_float fTimeDelta)
{
	if (m_pGameInstance->isKeyEnter(DIK_F1))	
	{
		m_bCharacterCameraOnOff = false; 
	}


	if (m_pGameInstance->isKeyEnter(DIK_F2))	
	{
		m_bCharacterCameraOnOff = !m_bCharacterCameraOnOff;

		/* �����ͷ� �������� .*/
		map<const _wstring, class CLayer*>* mapLayer = m_pGameInstance->Get_Layers();
		auto& LevelLayer = mapLayer[3];	/* �ְ� LEVEL_GAMEPLAY�� Layer ���� �̰� */

		CLayer* pLayer = LevelLayer.find(TEXT("Layer_Model"))->second;
		CPlayer* pPlayer = dynamic_cast<CPlayer*>(pLayer->Get_GameObject_List().front());

		m_pPlayerTransformCom = dynamic_cast<CTransform*>(pPlayer->Find_Component(TEXT("Com_Transform")));	
		
		_fvector CameraPos =	
		{
			 XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),	
			 XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 4.0f, // �Ӹ� ���� ����	
			 XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) - 3.0f,	
			 1.0f
		};

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, CameraPos);	
	}


	if (m_bCharacterCameraOnOff)
	{
		// �÷��̾��� �浹ü�� ���������ұ�.
		_vector vPlayerHeadPos = XMVectorSet(
			XMVectorGetX(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
			XMVectorGetY(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)) + 2.f, // �Ӹ� ���� ����
			XMVectorGetZ(m_pPlayerTransformCom->Get_State(CTransform::STATE_POSITION)),
			1.0f
		);

		//const float distance = 50.0f; // �÷��̾�� ī�޶� �Ÿ�	
		const float distance = 10.0f; // �÷��̾�� ī�޶� �Ÿ�	

		// ���� ���
		_float t = fTimeDelta;

		// ���� �ؾߵǴ� ī�޶��� ������ �Ÿ��� ī�޶� ���������ؼ� �����ϼ� �ְ� �ؼ� �ڿ������� ȭ�� ��ȯ�� �� �� �ֵ�������. 

		// ī�޶� �̵� ó��
		_vector vCamDir = XMVector3Normalize(m_pTransformCom->Get_State(CTransform::STATE_LOOK));
		_vector vNewCamPos = vPlayerHeadPos - vCamDir * distance;

		m_pTransformCom->Set_State(CTransform::STATE_POSITION, vNewCamPos);

		_long MouseMoveX = m_pGameInstance->Get_DIMouseMove(DIMS_X);
		_long MouseMoveY = m_pGameInstance->Get_DIMouseMove(DIMS_Y);




		float CosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, 1.f, 0.f, 0.f)));
		float OppostieCosTheta = XMVectorGetX(XMVector3Dot(m_pTransformCom->Get_State(CTransform::STATE_LOOK) * -1.f, XMVectorSet(0.f, -1.f, 0.f, 0.f)));
		//angleRadians = acosf(cosTheta); // ���� ��	
		float AngleDegrees = XMConvertToDegrees(acosf(CosTheta)); // ������ ��(degree)�� ��ȯ						
		float OppositeAngleDegrees = XMConvertToDegrees(acosf(OppostieCosTheta)); // ������ ��(degree)�� ��ȯ				

		if (m_pGameInstance->isKeyEnter(DIK_TAB))
		{
			if (m_bStop)
				m_bStop = false;
			else
				m_bStop = true;
		}

		if (!m_bStop)
		{

			if (MouseMoveX != 0 || MouseMoveY != 0)
			{
				// X��(Y�� ���� ȸ��)
				if (MouseMoveX != 0)
				{
					m_pTransformCom->Orbit_Move(XMVectorSet(0.f, 1.f, 0.f, 0.f), MouseMoveX * fTimeDelta * m_fMouseSensor, vPlayerHeadPos);
				}

				// Y��(Right �� ���� ȸ��)
				if (MouseMoveY != 0)
				{

					if (AngleDegrees > 60.f && OppositeAngleDegrees > 80.f)
						m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, vPlayerHeadPos);


					else if (AngleDegrees < 60.f && MouseMoveY < 0.f)
					{
						m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, vPlayerHeadPos);
					}

					else if (OppositeAngleDegrees < 80.f && MouseMoveY > 0.f)
					{
						m_pTransformCom->Orbit_Move(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), MouseMoveY * fTimeDelta * m_fMouseSensor, vPlayerHeadPos);
					}
				}
			}
		}

		// Look ���� ���� (�÷��̾� �Ӹ� ��ġ�� �ٶ󺸵��� ����)

		_vector CamDir = XMVectorSetW(XMVector3Normalize(vPlayerHeadPos - m_pTransformCom->Get_State(CTransform::STATE_POSITION)), 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_LOOK, CamDir);

		// Right ���� ���� (Up �� Look)
		_vector vUp = m_pPlayerTransformCom->Get_State(CTransform::STATE_UP);
		_vector vRight = XMVector3Normalize(XMVector3Cross(vUp, CamDir));

		// Up ���� ���� (Look �� Right)
		_vector vNewUp = XMVector3Normalize(XMVector3Cross(CamDir, vRight));

		// Transform ����
		m_pTransformCom->Set_State(CTransform::STATE_RIGHT, vRight);
		m_pTransformCom->Set_State(CTransform::STATE_UP, vNewUp);


		__super::Priority_Update(fTimeDelta);

	}
}

void CCharacterCamera::Update(_float fTimeDelta)
{

}

void CCharacterCamera::Late_Update(_float fTimeDelta)
{


}

HRESULT CCharacterCamera::Render()
{
	return S_OK;
}

CCharacterCamera* CCharacterCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCharacterCamera* pInstance = new CCharacterCamera(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CharacterCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCharacterCamera::Clone(void* pArg)
{
	CCharacterCamera* pInstance = new CCharacterCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CharacterCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCharacterCamera::Free()
{
	__super::Free();

	//Safe_Release(m_pPlayerTransformCom);
}
