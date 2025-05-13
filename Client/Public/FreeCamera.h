#pragma once

#include "Client_Defines.h"	
#include "Camera.h"

BEGIN(Engine)

class CShader; 
class CTransfor;

END

BEGIN(Client)
class CFreeCamera final : public CCamera
{
public:
	struct FREECAMERA_DESC :public CCamera::CAMERA_DESC 
	{
		_float  fSensor = { 0.f };
	};

private:
	CFreeCamera(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFreeCamera(const CFreeCamera& Prototype);
	virtual ~CFreeCamera() = default;
	
public:

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;


private:
	_float m_fSensor		= { 0.0f };
	_bool  m_bStop			= false; 

	_bool  m_bFreeCameraStop = { false };
	_bool  m_bCameraSpeedUp = { false };

	_bool  m_bTest = { false };

public:
	static CFreeCamera* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;	
};
END