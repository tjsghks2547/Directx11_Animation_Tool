#pragma once

#include "Client_Defines.h"
#include "Camera.h"

BEGIN(Engine)
class CCollider;
END

BEGIN(Client)

class CCharacterCamera final : public CCamera
{
public:
	struct FREE_CAMERA_DESC : public CCamera::CAMERA_DESC
	{
		_float			fMouseSensor = {};
	};

private:
	CCharacterCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCharacterCamera(const CCharacterCamera& Prototype);
	virtual ~CCharacterCamera() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;


private:
	CTransform* m_pPlayerTransformCom = { false };
	CCollider* m_pPlayerColliderTransformCom = { false };

private:
	_float*             m_fPlayerPosition = { nullptr };
	_float				m_fMouseSensor = {};
	_float				m_fLerpTime = { 3.f };

	_long			    m_MouseMoveAmount = {};

	_vector				m_vCurCamPos = {};
	_vector				m_vCurCamDir = {};


	_bool				m_bStop = false;		
	_bool				m_bCharacterCameraOnOff = { false };	


public:
	static  CCharacterCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END