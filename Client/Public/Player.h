#pragma once
#include "Client_Defines.h"
#include "ContainerObject.h"

BEGIN(Engine)
class CModel; /* ��Ʈ ��� ���� �߰� */
class CCollider;
class CNavigation;
END

class CStateMgr;	

BEGIN(Client)

class CPlayer final : public CContainerObject
{
public:	
	enum STATE
	{
		STATE_IDLE,
		STATE_RUN,
		STATE_WALK,
		STATE_ATTACK,
		STATE_ATTACK_L1,
		STATE_ATTACK_L2,
		STATE_ATTACK_L3,
		STATE_ATTACK_LONG_CLAW_01,
		STATE_ATTACK_LONG_CLAW_02,
	};


	enum PLAYER_PHASE	
	{
		PHASE_IDLE  = 1,	
		PHASE_FIGHT = 1 << 1,
	};

private:
	CPlayer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

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
	void Set_ParentPhaseState(_uint _PhaseState) { m_iPhaseState = _PhaseState; }	

private:
	CStateMgr*							m_pStateMgr      = { nullptr };	


private:
	_uint								m_iState         = { STATE_IDLE };
	_uint								m_iPreState      = { STATE_IDLE };

	_uint								m_iPhaseState    = { PHASE_IDLE };
	_uint								m_iPrePhaseState = { PHASE_IDLE };	
	
	const _float4x4*					m_pRootMatrix    = { nullptr };
	const _float4x4*					m_CombinedMatrix = { nullptr };


	_float								m_fTimeDelta = { 0.f };

	_bool								m_bPlayerOnOff = { false };

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

public:
	static CPlayer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END