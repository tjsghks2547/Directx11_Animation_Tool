#pragma once

#include "Base.h"

BEGIN(Engine)
class CGameInstance; 
class CShader; 
class CVIBuffer_Rect;	

class CRenderer final: public CBase
{
public:
	enum RENDERGROUP { RG_PRIORITY, RG_NONBLEND, RG_BLEND, RG_UI, RG_END};

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderer() = default;


public:
	HRESULT Initialize();
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroupID, class CGameObject* pGameObject);
	HRESULT Render();

private:
	ID3D11Device*		 m_pDevice       = { nullptr };
	ID3D11DeviceContext* m_pContext      = { nullptr };
	CGameInstance*		 m_pGameInstance = { nullptr };

	list<class CGameObject*>   m_RenderObjects[RG_END];

private:
	CShader* m_pShader           = { nullptr };
	CVIBuffer_Rect* m_pVIBuffer  = { nullptr };

	_float4x4				      m_WorldMatrix = {};
	_float4x4				      m_ViewMatrix = {};
	_float4x4				      m_ProjMatrix = {};
	_vector					      vPosition = {};
	ID3D11DepthStencilView*       m_pLightDepthStencilView = { nullptr };



private:
	HRESULT Render_Priority();
	HRESULT Render_NonBlend();
	HRESULT Render_Blend(); 
	HRESULT Render_UI();
	HRESULT Render_LightAcc();
	HRESULT Render_DeferredResult();

#ifdef _DEBUG
private:
	list<class CComponent*>  m_DebugComponents;
private:
	void Render_Debug();
#endif // _DEBUG


public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override; 

};

END