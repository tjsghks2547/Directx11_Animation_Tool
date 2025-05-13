#pragma once

#include "Base.h"	

BEGIN(Engine)	

class CShader;	
class CGameInstance;	 
class CVIBuffer_Cube;	


class ENGINE_DLL CCube final : public CBase		
{
private:
	CCube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCube() = default; 


public:
	virtual HRESULT Initialize(_float3* pPoints);	
	virtual HRESULT Render(); 

public:
	CVIBuffer_Cube* Get_Buffer()	  { return m_pVIBufferCom; }	
	_float4x4*		Get_WorldMatrix() { return &m_WorldMatrix; }

private:
	ID3D11Device*				m_pDevice  = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	
	CVIBuffer_Cube*				m_pVIBufferCom  = { nullptr }; 
	CShader*					m_pShaderCom    = { nullptr };
	CGameInstance*				m_pGameInstance = { nullptr };

	_uint					    m_iPass = { 0 };

public:
	void Setting_Shader_CubePass(_uint _PassNumber) { m_iPass = _PassNumber; }	

private:
	_float4x4					m_WorldMatrix = {};

public:
	static CCube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _float3* pPoints);
	virtual void Free() override;			
};

END	
