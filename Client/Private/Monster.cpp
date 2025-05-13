#include "pch.h"
#include "Monster.h"
#include "GameInstance.h"
#include "Animation.h"

CMonster::CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CMonster::CMonster(const CMonster& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CMonster::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;


	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{


}

void CMonster::Update(_float fTimeDelta)
{
	
	/*if(m_pModel->Get_VecAnimation().at(0)->Get_CurrentEventFrame() == 20)
	{
		
	}*/
		
}

void CMonster::Late_Update(_float fTimeDelta)
{
	//m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);	
}

HRESULT CMonster::Render()
{
	return S_OK;
}


void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);

}
