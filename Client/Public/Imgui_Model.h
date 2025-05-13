#pragma once

#include "Base.h"
#include "GameInstance.h"
#include "GameObject.h"
#include "Client_Defines.h"

BEGIN(Engine)
class CModel; 
END

class CImgui_Model : public CBase
{
private:
	CImgui_Model();
	virtual ~CImgui_Model() = default;


public:

	HRESULT Initialize();
	HRESULT Update(_float _fTiemDelta);


	void  Model_Window();
	void  Enroll_Object();
	void  Transform_Method();
	void  Animaiton_Function();
	void  Weapon_Select_Function();
	void  Model_EventFrame_Function();

	HRESULT Model_Save();

private:
	_bool          m_bModelWindowOnOff = { false };
	_bool		   m_bModelWeaponWindow = { false };
	_bool          m_bEventFrameWindow = { false };
	_bool          m_bModelSaveOnOff = { false };
	_bool		   m_bSystemOnOff = { false };
	_bool          m_bRotationValueReset = { false };
	_bool          m_bAnimationPlay = { false };
	_bool		   m_bAnimationStop = { false };
	_bool		   m_bLoop = { false };
	_bool          m_bKeyFrameOnOff = { false };

	/* Weapon Select  member variable */
	_int           m_iSelect_option = { };


	CGameInstance* m_pGameInstance = { nullptr };
	CGameObject* m_pGameObject = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CModel* m_pModel = { nullptr };


	_int		   m_iFrameIndex = {};
	_int           m_iCurTrackPosition = {};
	_uint		   m_iCurSelectAnimationIndex = {};
	_uint		   m_iNextSelectAnimationIndex = {};
	_uint		   m_iPreSelectAnimationIndex = {};

	_float		   m_fTimeDetla = {};

	_float		   m_fFirstAnimationSpeed = { 1.f };
	_float		   m_fSecondAnimationSpeed = { 1.f };

	_float		   m_fFirstAnimationLerpSpeed = { 0.2f };
	_float		   m_fSecondAnimationLerpSpeed = { 0.2f };

	_float		   m_fFirstAnimation_KeySpeed = { 0.f };
	_float		   m_fSecondAnimation_KeySpeed = { 0.f };


	_float		   m_fFirstAnimation_KeyFrameSpeed = { 0.f };
	_float		   m_fSecondAnimation_KeyFrameSpeed = { 0.f };

	_float		   m_fStartFramePosition = {};

	_int		   m_iFirstAnimationStartKeyIndex = {};
	_int		   m_iFirstAnimationSecondKeyIndex = {};

	_int		   m_iSecondAnimationStartKeyIndex = {};
	_int		   m_iSecondAnimationSecondKeyIndex = {};


	/* 특정 프레임 일때 애니메이션 변경 가능하게 해주기 위한 변수 */
	_int		   m_iAniChangeTrackPosition = {};
	_bool		   m_bAniChangeTrackOnOff = { false };


	_float		   m_fStartEventFrame = {};
	_float		   m_fEndEventFrame = {};

	_char		   m_szEventName[MAX_PATH] = {};
	_char		   m_szDeleteEventName[MAX_PATH] = {};

	_float3        m_fRotationXYZ = {};	
	_float3        m_fPreRotationXYZ = {};	
	
	vector<const char*>   m_vecAnimationName;	
	 
public:
	static CImgui_Model* Create(); 
	virtual void Free() override; 


};

