#pragma once

#include "Transform.h"

BEGIN(Engine)
class CModel; 

class ENGINE_DLL CGameObject abstract : public CBase
{
public:

	struct GAMEOBJECT_DESC : public CTransform::TRANSFORM_DESC
	{
		const _char* szName      = {};
		_float4x4*  _ParentMatrix = {};	
		_float4     _fPosition      = {};
		_float4     _fScaling       = {};	
		_float      _fLerpTime      = {};
		_float      _fLerpTimeSpeed = {}; 
		_float4     _fRelativePos = {};		
		_uint       _iLerpMethod = {}; 
	};	

protected:
	CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGameObject(const CGameObject& Prototype);
	virtual ~CGameObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);

	virtual HRESULT Render();

	/* 10월 31일 추가*/
	virtual HRESULT Ready_Component();

	/* 11월 11일 추가*/
	map<const wstring, class CComponent*>* Get_Components() { return &m_Components;}

	const _char* Get_Name()
	{
		return m_szName;
	}

	class CComponent* Find_Component(const _wstring& strComponentTag);	

	CModel* Get_Model_Ptr() { return m_pModel; }
	
	class CTransform* Get_Transfrom() { return m_pTransformCom; }

	/* 카메라 관련 함수 설정 */
	_float4x4 Get_CameraWorldMatrix() { return m_CameraWorldMatrix; }		
	void	  Set_CameraWorldMatrix(_fmatrix _CameWorldMatrix);	

	_float    Get_CameraLerpTime()      {return m_fCameraLerpTime;}	
	_float    Get_CameraLerpTimeSpeed() {return m_fCameraLerpTimeSpeed;}
	_float4   Get_CameraRelativePos()   {return m_fCameraRelativePos; }
	_uint     Get_CameraLerpMethod() { return m_iLerpMethod; }

	void      Set_CameraLerpTime(_float _CamLerpTime)           { m_fCameraLerpTime = _CamLerpTime;}	
	void      Set_CameraLerpTimeSpeed(_float _CamLerpTimeSpeed) { m_fCameraLerpTimeSpeed = _CamLerpTimeSpeed; }	
	void      Set_CameraRelativePos(_float4 _CameraRelativePos) { m_fCameraRelativePos = _CameraRelativePos;  }	
	void      Set_CameraLerpMethod(_uint _iLerpMethod) { m_iLerpMethod = _iLerpMethod; }

protected:
	CModel*						m_pModel = {nullptr}; 
	_char				        m_szName[MAX_PATH] = {};
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };
	class CGameInstance*		m_pGameInstance = { nullptr };
	class CTransform*			m_pTransformCom = { nullptr };
	
	
	/* 카메라 툴 관련 */
	_float4x4					m_CameraWorldMatrix    = {};		

	_float4						m_fCameraRelativePos   = {};
	_float					    m_fCameraLerpTime      = {1.f};		
	_float					    m_fCameraLerpTimeSpeed = {1.f};		
	
	_uint						m_iLerpMethod = {}; 


protected:
	map<const wstring, class CComponent*>	m_Components; //다른 게임오브젝트가 찾을 수 있으므로. 맵설정
	
protected:
	
	HRESULT Add_Component(_uint iPrototypeLevelIndex, const _wstring& strPrototype, 
		const _wstring& strComponentTag, CComponent** ppOut, void* pArg = nullptr);
	

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END