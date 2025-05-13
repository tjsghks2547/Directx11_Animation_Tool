#pragma once	
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CShader;
class CTexture;
class CVIBuffer_Terrain;
END


BEGIN(Client)
class CTerrain final : public CGameObject
{
private:
	CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTerrain(const CTerrain& Prototype);
	virtual ~CTerrain() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;

	virtual HRESULT Render() override;

	/* 10월 31일 추가*/
	virtual HRESULT Ready_Component();

private:
	HRESULT Bind_ShaderResources();

private:
	CShader* m_pShaderCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CVIBuffer_Terrain* m_pVIBufferCom = { nullptr };

/* 11월 16일 임시*/
private:
	VTXNORTEX* pVertices  = { nullptr };
	_uint*     pIndices   = { nullptr };
	bool       m_btest = { true };
	size_t     m_iTotalTriangle = {};
	CVIBuffer_Terrain* pTerrain = { nullptr };
	bool	   m_isButtonPressed = { false };

/* 11월 15일 임시 */
private:
	ID3D11Texture2D* m_pTexture2D = { nullptr };
	_uint							m_iWinSizeX, m_iWinSizeY;
public:
	static CTerrain* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};
END
