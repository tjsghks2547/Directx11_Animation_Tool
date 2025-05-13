#pragma once

#include "Base.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel;
END

class CImgui_Camera : public CBase
{


private:
	CImgui_Camera();
	virtual ~CImgui_Camera() = default;


public:

	HRESULT Initialize();
	HRESULT Update(_float _fTiemDelta);
	HRESULT Camera_Save();
	HRESULT Camera_Load();
	

	void  Camera_Window(); 
	void  Cube_Install_Method();	
	void  Installed_Cube_List();
	void  Play_Camera(); 
	void  Move_Camera();
	void  Rotation_Camera();	

private:
	_vector CatmullRom_Rotation_Lerp(vector<Camera_Event>* pvecCameraEvent, _float _fRatio);	
	_vector CatmullRom_Position_Lerp(vector<Camera_Event>* pvecCameraEvent, _float _fRatio);	


private:
	CGameInstance* m_pGameInstance       = { nullptr };
	CGameObject*   m_pPlayerGameObject   = { nullptr };
	CTransform*    m_pCameraTransformCom       = { nullptr }; // 카메라 위치 조절 기능 
	CModel*        m_pPlayerModel        = { nullptr }; // 플레이어 


	CGameObject*   m_pCurSelectedCam		= { nullptr };
	
private:
	_float		   m_fTimeDelta         = { 0.f };
	_bool          m_bCameraWindowOnOff = { false };
	_bool		   m_bCameraSaveOnOff   = { false };
	_bool		   m_bCameraLoadOnOff   = { false };
	_bool          m_bPlayOnOff = { false };
	_uint		   m_iIndex = {1}; 
	_uint		   m_iSelected_row = {};	
	_uint		   m_iPlayCamera_Index = {};

	_float		   m_fCurTime     = { 0.f };
	_float		   m_fLerpSpeed   = { 0.f };
	_float		   m_fLerpTime    = { 0.f };
	

	_char					  m_szDeleteCamName[MAX_PATH] = {};
	_char					  m_szSaveCamName[MAX_PATH] = {};
	_char					  m_szCameraName[MAX_PATH] = {}; 
	vector<_float4x4>		  m_vecCameraMatrix;	
	vector<Camera_Event>      m_vecCameraEvent;	 
	list<class CGameObject*>* m_pListGameObject = { nullptr };	


	_bool		   m_bRadioButton = {false}; 

	_uint          m_iSelectLerpMethod = {};
	string		   m_fullpath;



public:
	static CImgui_Camera* Create();
	virtual void Free() override;
};

