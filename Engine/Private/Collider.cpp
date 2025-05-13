#include "Collider.h"

#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"
#include "GameInstance.h"

CCollider::CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice,pContext)
{
}

CCollider::CCollider(const CCollider& Prototype)
	:CComponent(Prototype)
	, m_eType{ Prototype.m_eType }
#ifdef _DEBUG
	, m_pBatch{ Prototype.m_pBatch }	
	, m_pEffect{ Prototype.m_pEffect }	
	, m_pInputLayout{ Prototype.m_pInputLayout }	
#endif
{
#ifdef _DEBUG
	Safe_AddRef(m_pInputLayout);	
#endif
}

HRESULT CCollider::Initialize_Prototype(TYPE eColliderType)
{
	m_eType = eColliderType; 

#ifdef _DEBUG
	m_pBatch  = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);		

	const void* pShaderByteCode = { nullptr };
	size_t      iShaderByteCodeLength = {};

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);	

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount, pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;

#endif // _DEBUG

	return S_OK;

}

HRESULT CCollider::Initialize(void* pArg)
{
	CBounding::BOUNDING_DESC* pBoundingDesc = static_cast<CBounding::BOUNDING_DESC*>(pArg);

	switch (m_eType)
	{
	case TYPE_AABB:
		m_pBounding = CBounding_AABB::Create(m_pDevice, m_pContext, pBoundingDesc);
		break;
	case TYPE_OBB:
		//m_pBounding = CBounding_OBB::Create(m_pDevice, m_pContext, pBoundingDesc);
		break;
	case TYPE_SPHERE:
		m_pBounding = CBounding_Sphere::Create(m_pDevice, m_pContext, pBoundingDesc);
		break;
	}

	if (nullptr == m_pBounding)
		return E_FAIL;

	return S_OK;
}

void CCollider::Update(_fmatrix WorldMatrix)
{
	m_pBounding->Update(WorldMatrix);		
}

_bool CCollider::Intersect(CCollider* pTargetCollider)
{
	m_isColl = m_pBounding->Intersect(pTargetCollider->m_eType, pTargetCollider->m_pBounding);	

	return m_isColl;	
}


HRESULT CCollider::Render()
{
	if (nullptr == m_pBounding)
		return E_FAIL;

	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(CPipeLine::D3DTS_PROJ));

	m_pEffect->Apply(m_pContext);

	m_pContext->IASetInputLayout(m_pInputLayout);

	
	m_pBatch->Begin();


	//m_pBounding->Render(m_pBatch, true == m_isColl ? XMVectorSet(1.f, 0.f, 0.f, 1.f) : XMVectorSet(0.f, 1.f, 0.f, 1.f));

	m_pBounding->Render(m_pBatch, XMVectorSet(1.f, 1.f, 1.f, 1.f));
	
	
	m_pBatch->End();
	

	return S_OK;
}

CCollider* CCollider::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType)
{
	CCollider* pInstance = new CCollider(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eColliderType)))
	{
		MSG_BOX("Failed To Created : CCollider");	
		Safe_Release(pInstance);	
	}

	return pInstance;	
}

CComponent* CCollider::Clone(void* pArg)
{
	CCollider* pInstance = new CCollider(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CCollider");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollider::Free()
{
	__super::Free();	

	Safe_Release(m_pBounding);	

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);	

	if (false == m_isCloned)	
	{
		Safe_Delete(m_pBatch);	
		Safe_Delete(m_pEffect);	
	}	
#endif
}
