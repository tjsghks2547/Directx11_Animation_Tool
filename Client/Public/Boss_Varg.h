#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CModel; /* 루트 모션 때매 추가 */
class CCollider;
class CNavigation;
END



BEGIN(Client)

class CBoss_Varg final : public CContainerObject
{
private:
	CBoss_Varg(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBoss_Varg(const CBoss_Varg& Prototype);
	virtual ~CBoss_Varg() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_Components();
	HRESULT Ready_PartObjects();


public:

private:
	//CModel*			 m_pModel		  = { nullptr };
	const _float4x4* m_pRootMatrix = { nullptr };
	const _float4x4* m_CombinedMatrix = { nullptr };



	_bool								m_bIsRootMotionApplied = { false };
	_float4x4 					        m_CurrentTransformWorldMatrix = {};
	_float4x4 							m_FinalTransformWorldMatrix = {};

	_bool								m_bRootMotion = { false };


	_bool								m_bFalling = { false };
	_bool								m_bGround = { true };

private:
	_float								m_fTimeDelta = { 0.f };


public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CBoss_Varg* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END