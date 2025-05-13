#pragma once

#include "Client_Defines.h"
#include "GameObject.h"


BEGIN(Engine)
class CShader;
class CModel;
END

class CMonster abstract : public CGameObject
{
protected:	
	CMonster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CMonster(const CMonster& Prototype);	
	virtual ~CMonster() = default;	
public:
	virtual HRESULT Initialize_Prototype() override;	
	virtual HRESULT Initialize(void* pArg) override;	
	virtual void Priority_Update(_float fTimeDelta) override;	
	virtual void Update(_float fTimeDelta) override;	
	virtual void Late_Update(_float fTimeDelta) override;	
	virtual HRESULT Render() override;	

protected:	
	CShader* m_pShaderCom = { nullptr };
	CModel*  m_pModelCom  = { nullptr };

public:
	virtual HRESULT Ready_Components() = 0;				
	virtual HRESULT Bind_ShaderResources() = 0;		


public:	
	virtual void Free() override;	

};

