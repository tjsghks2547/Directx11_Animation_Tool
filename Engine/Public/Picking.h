#pragma once

#include "Base.h"

BEGIN(Engine)

class CGameInstance;

class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);	
	virtual ~CPicking() = default;

public:
	_bool Get_PickPos(_float4* pPickPos);
	
	HRESULT Initialize(HWND hWnd);
	void Update();
	ID3D11Texture2D* Get_DepthTexture() { return m_pTexture2D; }
	


private:
	ID3D11Device*        m_pDevice  = { nullptr }; 
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance*       m_pGameInstance  = { nullptr };
	ID3D11Texture2D*     m_pTexture2D = { nullptr };	

	_uint				 m_iWinSizeX, m_iWinSizeY;
	HWND				 m_hWnd = {};
	
	_bool				 m_isSuccess = { false };
	_float4				 m_vPickPos  = {};

private:
	_float Compute_ProjZ(_float fX, _float fY);

public:
	static CPicking* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

END