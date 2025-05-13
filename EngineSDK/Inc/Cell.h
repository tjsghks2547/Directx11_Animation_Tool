#pragma once

#include "GameObject.h"


BEGIN(Engine)

class CVIBuffer_Cell;

class CCell final : public CGameObject 
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END};
	enum LINE  { LINE_AB, LINE_BC, LINE_CA, LINE_END};

private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default; 

public:
	HRESULT Initialize(const _float3* pPoints);	


private:
	CGameInstance*			m_pGameInstance = { nullptr };

	_float3					m_vPoints[3] = {};
	_int					m_iIndex	 = {};
	_int					m_iNeighborIndices[LINE_END] = { -1,-1,-1 };



private:
	CVIBuffer_Cell*   m_pVIBuffer  = { nullptr };
	

public:
	_float3*			Get_Points()			{ return m_vPoints; }
	CVIBuffer_Cell*		Get_Buffer()			{ return m_pVIBuffer; }
	HRESULT				Render();


public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints);
	virtual CGameObject* Clone(void* pArg);
	virtual void Free() override; 

};
END