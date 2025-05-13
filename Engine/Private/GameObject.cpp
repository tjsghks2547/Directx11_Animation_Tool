#include "..\Public\GameObject.h"
#include "GameInstance.h"


CGameObject::CGameObject(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

CGameObject::CGameObject(const CGameObject & Prototype)
	: m_pDevice { Prototype.m_pDevice }
	, m_pContext { Prototype.m_pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CGameObject::Initialize(void * pArg)
{
	/* 여기서 추가적인 정보들 넣는거 TRANSFROM_DESC에 없던것들*/
	GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
	
	/* 여기서 이제 월드 매트릭스를 항등핼렬로 만들어주고*/
	m_pTransformCom = CTransform::Create(m_pDevice, m_pContext);		
	if (m_pTransformCom == nullptr)
		return E_FAIL; 
	
	if(FAILED(m_pTransformCom->Initialize(pArg)))
		return E_FAIL;	


	m_Components.emplace(TEXT("Com_Transform"), m_pTransformCom);

	if (pDesc->_ParentMatrix != nullptr)
		XMStoreFloat4x4(&m_CameraWorldMatrix, XMLoadFloat4x4(&(*pDesc->_ParentMatrix)));	

	Safe_AddRef(m_pTransformCom);	

	return S_OK;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{
	
	/*
	for(auto& pComponents : m_Components)
	{
		pComponents->Update(); 
	}

	이런 코드가 들어갈듯. 
	*/
}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
	return S_OK;
}

HRESULT CGameObject::Ready_Component()
{
	return S_OK;
}

CComponent* CGameObject::Find_Component(const _wstring& strComponentTag)
{
	auto iter = m_Components.find(strComponentTag);

	if (iter == m_Components.end())
	{
		return nullptr;
	}

	return iter->second;

}

void CGameObject::Set_CameraWorldMatrix(_fmatrix _CameWorldMatrix)
{
	XMStoreFloat4x4(&m_CameraWorldMatrix, _CameWorldMatrix);	
}

HRESULT CGameObject::Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototype, const _wstring& strComponentTag, CComponent** ppOut, void* pArg)
{

	CComponent* pComponent = dynamic_cast<CComponent*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iPrototypeLevelIndex, strPrototype, pArg));

	if (pComponent == nullptr) /* 원형이 없을 때 */
		return E_FAIL;

	if (nullptr != Find_Component(strComponentTag)) /* 이미 들어가 있을때 컴포넌트가*/
		return E_FAIL;

	m_Components.emplace(strComponentTag, pComponent);

	*ppOut = pComponent;
	
	Safe_AddRef(pComponent);

	return S_OK;
}

void CGameObject::Free()
{
	__super::Free();

	for(auto& Pair : m_Components)
	{
		Safe_Release(Pair.second);
	}
	m_Components.clear();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
