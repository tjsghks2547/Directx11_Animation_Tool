#pragma once
#include "Base.h"

BEGIN(Engine)
class CPipeLine;
class CTransform; 

class ENGINE_DLL CCalculator final : public CBase	
{
private:
	CCalculator();
	virtual ~CCalculator() = default;

public:
	POINT Get_Mouse_Point() { return m_MousePoint; } /* (2차원) 화면상의 좌표*/
	void  Store_MouseRay(XMMATRIX _mat_ProjInverse, XMMATRIX _mat_viewInverse);

public:
	_vector Get_RayPos() { return XMLoadFloat3(&m_vRayPos_Local);}
	_vector Get_RayDir() { return XMLoadFloat3(&m_vRayDir_Local);}
	void Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, CTransform* pTransform);
	void Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, XMMATRIX WorldInverse);

	_float3 Get_WorldRayPos() { return m_vRayPos; }	
	_float3 Get_WorldRayDir() { return m_vRayDir; }	

	
	_float3 Compute_PickingPoint(_float3 _point1, _float3 _point2, _float3 _point3);

private:
	HRESULT Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);	

private:
	D3D11_VIEWPORT m_viewport;

	_float3   m_vRayDir, m_vRayPos; /* 월드 상의 마우스 좌표*/
	_float3   m_vRayDir_Local, m_vRayPos_Local;
	POINT     m_MousePoint;
	HWND      m_hWnd; 

public:
	static CCalculator* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void Free() override;
};

END