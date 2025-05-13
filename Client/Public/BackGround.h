#pragma once

#include "Client_Defines.h"	
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)
class CBackGround : public CUIObject
{
public:
	struct BACKGROUND_DESC: public CUIObject::UIOBJECT_DESC
	{
		
	};

private:
	CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBackGround(const CBackGround& Prototype);
	virtual ~CBackGround() = default; 

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override; 

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override; 
	virtual void Late_Update(_float fTimeDelta) override;
	
	virtual HRESULT Render() override;


private:
	virtual HRESULT Ready_Component() override;
	HRESULT Bind_ShaderResources();

private:
	CTexture*					m_pTextureCom = { nullptr };
	CVIBuffer_Rect*				m_pVIBuffer_Rect_Com = { nullptr };
	CShader*					m_pShader = { nullptr };

public:
	static CBackGround* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override; 

};

END