#include "pch.h"
#include "FreeCamera.h"
#include "GameInstance.h"

CFreeCamera::CFreeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CCamera(pDevice,pContext)
{
}

CFreeCamera::CFreeCamera(const CFreeCamera& Prototype)
    :CCamera(Prototype)
{
}



HRESULT CFreeCamera::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	

    return S_OK;
}

HRESULT CFreeCamera::Initialize(void* pArg)
{
	strcpy_s(m_szName, "Free_Camera");

    FREECAMERA_DESC* pDesc = static_cast<FREECAMERA_DESC*>(pArg);
    
    m_fSensor = pDesc->fSensor;

     if (FAILED(__super::Initialize(pArg))) 
		 
        return E_FAIL;  



    return S_OK;
}

void CFreeCamera::Priority_Update(_float fTimeDelta)
{

	if (m_pGameInstance->isKeyEnter(DIK_F1))	
	{
		m_bFreeCameraStop = !m_bFreeCameraStop;
	}

	if (m_pGameInstance->isKeyEnter(DIK_F2))	
	{
		m_bFreeCameraStop = true;	
	}

	if (m_pGameInstance->isKeyEnter(DIK_F3))
	{
		m_bCameraSpeedUp = !m_bCameraSpeedUp;		
	}
	
	if (m_pGameInstance->isKeyEnter(DIK_F5))
	{
		m_bTest = !m_bTest;
	}
	
	if (m_bTest)
	{


		if (!m_bFreeCameraStop)
		{
			if (!m_bCameraSpeedUp)
			{
				if (GetKeyState('W') & 0x8000)
					m_pTransformCom->Go_Straight(fTimeDelta);
				if (GetKeyState('S') & 0x8000)
					m_pTransformCom->Go_BackWard(fTimeDelta);
				if (GetKeyState('A') & 0x8000)
					m_pTransformCom->Go_Left(fTimeDelta);
				if (GetKeyState('D') & 0x8000)
					m_pTransformCom->Go_Right(fTimeDelta);
			}
			else
			{
				if (GetKeyState('W') & 0x8000)
					m_pTransformCom->Go_Straight(fTimeDelta * 0.1f);
				if (GetKeyState('S') & 0x8000)
					m_pTransformCom->Go_BackWard(fTimeDelta * 0.1f);
				if (GetKeyState('A') & 0x8000)
					m_pTransformCom->Go_Left(fTimeDelta * 0.1f);
				if (GetKeyState('D') & 0x8000)
					m_pTransformCom->Go_Right(fTimeDelta * 0.1f);
			}


			if (m_pGameInstance->isKeyEnter(DIK_TAB))
			{
				if (m_bStop)
					m_bStop = false;
				else
					m_bStop = true;
			}

			if (!m_bStop)
			{
				_long		MouseMove = { 0 };

				if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_X))
				{
					m_pTransformCom->Turn(XMVectorSet(0.f, 1.f, 0.f, 0.f), fTimeDelta * m_fSensor * MouseMove);
				}

				if (MouseMove = m_pGameInstance->Get_DIMouseMove(DIMS_Y))
				{
					m_pTransformCom->Turn(m_pTransformCom->Get_State(CTransform::STATE_RIGHT), fTimeDelta * m_fSensor * MouseMove);
				}

			}

			__super::Priority_Update(fTimeDelta);
		}
	}
}

void CFreeCamera::Update(_float fTimeDelta)
{

}

void CFreeCamera::Late_Update(_float fTimeDelta)
{
}

CFreeCamera* CFreeCamera::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CFreeCamera* pInstance = new CFreeCamera(pDevice,pContext);
	
	if(FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : FreeCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CFreeCamera::Clone(void* pArg)
{
	CFreeCamera* pInstance = new CFreeCamera(*this);

	if(FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : FreeCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFreeCamera::Free()
{
	__super::Free();
}
