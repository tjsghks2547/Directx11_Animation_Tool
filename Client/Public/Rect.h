#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture; 
class CShader;
class CVIBuffer_Rect; 
END

BEGIN(Client)
class CRect final : public CGameObject
{
private:
	CRect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRect(const CRect& Prototype);
	virtual ~CRect() = default; 

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
	CTexture*		m_pTextureCom   = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom  = { nullptr };
	CShader*		m_pShader		= { nullptr };

	_float4x4       m_ViewMatrix{}, m_ProjMatrix{};

public:
	static  CRect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};
END
