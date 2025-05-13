#include "EventMgr.h"
#include "GameObject.h"
#include "GameInstance.h"


CEventMgr::CEventMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	, m_pContext(pContext)
	, m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEventMgr::Initialize()
{

	return S_OK;
}

HRESULT CEventMgr::Delete_All_In_Layer(_uint iLevelIndex, const _wstring& strLayerTag)	
{

	return S_OK;	
}



CEventMgr* CEventMgr::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEventMgr* pInstance = new CEventMgr(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CEventMgr");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEventMgr::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
