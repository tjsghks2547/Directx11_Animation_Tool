#pragma once

#include "Client_Defines.h"	
#include "GameObject.h"	


BEGIN(Engine)	
class CShader;	
class CModel;
END

class CCameraModel final : public CGameObject
{

private:
	CCameraModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCameraModel(const CCameraModel& Prototype);
	virtual ~CCameraModel() = default;

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
	CShader*   m_pShaderCom   = { nullptr };
	CModel*    m_pModelCom    = { nullptr };



public:
	static CCameraModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;	
	virtual void Free() override;	
};

