#pragma once

#include "Client_Defines.h"	
#include "GameObject.h"	


BEGIN(Engine)
class CShader;
class CModel;
END

class CCubeObject final : public CGameObject
{

private:
	CCubeObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCubeObject(const CCubeObject& Prototype);
	virtual ~CCubeObject() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual HRESULT Bind_ShaderResources();
	virtual HRESULT Ready_Components();

public:
	virtual void OnCollisionEnter(CGameObject* _pOther);
	virtual void OnCollision(CGameObject* _pOther);
	virtual void OnCollisionExit(CGameObject* _pOther);

private:
	CShader* m_pShaderCom = { nullptr };
	CModel* m_pModelCom = { nullptr };
	_float4x4 m_CameraMatrix = {};	



public:
	static CCubeObject* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

