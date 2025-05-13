#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
public:
	enum TYPE{TYPE_AABB, TYPE_OBB, TYPE_SPHERE, TYPE_END};

private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default; 

public:
	virtual HRESULT Initialize_Prototype(TYPE eColliderType);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Update(_fmatrix WorldMatrix);


public:
	_bool Intersect(CCollider* pTargetCollider);
	/* 여기다 나중에 레이충돌 함수 따로 만들면될듯*/


#ifdef _DEBUG
public:
	virtual HRESULT Render(); 
#endif // _DEBUG

private:
	TYPE							m_eType     = { TYPE_END };
	class CBounding*				m_pBounding = { nullptr };
	_bool							m_isColl    = { false };


#ifdef _DEBUG
private:
	PrimitiveBatch<DirectX::VertexPositionColor>* m_pBatch = { nullptr };
	BasicEffect*								  m_pEffect = { nullptr };
	ID3D11InputLayout*							  m_pInputLayout = { nullptr };
#endif


public:
	static CCollider* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eColliderType);
	virtual CComponent* Clone(void* pArg) override; 
	virtual void Free() override; 

};

END