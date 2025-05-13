#pragma once


#include "Base.h"
#include "GameInstance.h"

class CImgui_File;
class CImgui_Model; 
class CImgui_Camera; 



class CImguiManager : public CBase
{
	DECLARE_SINGLETON(CImguiManager)

private:
	CImguiManager();
	virtual ~CImguiManager(); 

public:

	void Init(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pGraphicDeviceContext);	
	void Update(_float fTimeDelta);	
	void Render(); 
	void Free(); 

public:
	ID3D11Device*		   Get_Device()        { return m_pDevice; }
	ID3D11DeviceContext*   Get_DeviceContext() { return m_pContext; }
	CGameInstance*		   Get_GameInstance()  { return m_pGameInstance; }

private:

	ID3D11Device*			m_pDevice		   = { nullptr };		
	ID3D11DeviceContext*	m_pContext		   = { nullptr };	
	CGameInstance*			m_pGameInstance    = { nullptr };

	
	CImgui_File*			 m_pImguiFileMenu     = { nullptr };
	CImgui_Model*			 m_pImguiModelMenu    = { nullptr };
	CImgui_Camera*			 m_pImguiCameraMenu   = { nullptr };

public:


	_float3 Get_Object_Picking_Point()
	{
		return  m_fPicking_Point;
	}



	void Set_Object_Picking_Point(_float _x, _float _y, _float _z)
	{
		m_fPicking_Point.x = _x;
		m_fPicking_Point.y = _y;
		m_fPicking_Point.z = _z;
	}


	void Set_PickingPoint(_float3 _point1, _float3 _point2, _float3 _point3) 
	{
		m_fPicking_Trianle_Point[0] = &_point1;
		m_fPicking_Trianle_Point[1] = &_point2;
		m_fPicking_Trianle_Point[2] = &_point3;
	}
	
	_float3** Get_Triangle_Pont() { return m_fPicking_Trianle_Point; }


	void Set_MousePicking(bool _bool)
	{
		m_bMousePicking = _bool;
	}

	_bool Get_MousePicking()
	{
		return m_bMousePicking;
	}


private:
	D3D11_RASTERIZER_DESC  m_RasterDesc = {};
	ID3D11RasterizerState* m_pRasterRiseState = { nullptr };
	
	
	bool    WireFrameOnOff = { false };
	/* 마우스 피킹이 눌렸는지 확인하기 위한 작업*/
	bool    m_bMousePicking = { false };


	_float3* m_fPicking_Trianle_Point[3];
	_float3  m_fPicking_Point = {};
	

};

