#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAnimation : public CBase	
{
public:
	
private:
	CAnimation();
	virtual ~CAnimation() = default; 

public:
	HRESULT Initiailize(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	_bool Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop);

	/* 11월 27일 추가한 코드 */
	void  Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton);
	_uint Get_ChannelIndex(const _char* pChannelName);	
	_bool Lerp_NextAnimation(_float fTimeDelta, class CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices);
	_char* Get_AnimationName() { return m_szName; }	
	vector<class CChannel*>& Get_AnimationChannel() { return m_Channels; }

	void Set_TrackPosition(_float _TrackPosition, vector<_uint>& CurrentKeyFrameIndices, const vector<class CBone*>& Bones);
	void Set_AnimationSpeed(_float _fSpeed) { m_fAnimationSpeed = _fSpeed; }
	void Set_LerpTime(_float _fLerpSpeed) { m_LerpTime = _fLerpSpeed; }

	_float Get_AnimationSpeed() { return  m_fAnimationSpeed; }		
	_float Get_AnimationLerpTime() { return  m_LerpTime; }	
	_float Get_Duration() { return m_fDuration; }
	_bool  isAniMationFinish() { return m_isFinished; }

	_float Get_CurrentKeySpeed() { return m_vecKeyFrameAnimationSpeed.at((int)m_fCurrentKeyFramePosition);}
	void   Set_TrackKeySpeed(_uint _istartTrackPosition, _uint _iEndTrackPosition, _float _fTrackSpeed);
	_float Get_CurrentKeyFramePosition() { return m_fCurrentKeyFramePosition; }	

	void   Set_StartOffSetTrackPosition(_float _fOffsetTrackValue) { m_fSetStartOffSetTrackPosition = _fOffsetTrackValue; }		
	_float Get_StartOffSetTrackPosition() { return m_fSetStartOffSetTrackPosition; }
	HRESULT			 Add_EventFrame(ANIMEVENT _event);	
	HRESULT			 Sub_EventFrame(_char* _EventName);	
	_int			 Get_CurrentEventFrame() { return m_iCurrentEventFrame; }	
	map<_int, bool>* Get_MapEventFrame() { return &m_mapEventFrame; }

	vector<ANIMEVENT>* Get_vecEvent() { return &m_vecAnimFrameEvent; }	

private:
	_float					   m_fDuration = {};	
	_float					   m_fTickPerSecond = {};
	_float					   m_fCurrentKeyFramePosition = {};
	_float					   m_fCurrentTrackPosition = {};
	_float					   m_fSetStartOffSetTrackPosition = {};		
	_uint					   m_iNumChannels = {};	
	_float					   m_fAnimationSpeed = {1.f};	

	vector<class CChannel*>    m_Channels;  // 이걸 가져오자 

	_char					   m_szName[MAX_PATH];	

	vector<_float>			   m_vecKeyFrameAnimationSpeed;		

	/*11월 27일 추가 코드*/
	/* 그러면 저기서 애니메이션을 초기화 시켜줘야하는데.*/

	bool                       m_bReset = { false };	

	_float					   m_LerpTime = 0.2f;	
	_float					   m_LerpTimeAcc = 0.f;	

	_bool					   m_isFinished = { false };	
	_bool					   m_bFirst = { true };

	vector<_uint>*             m_CurrentKeyFameIndices;		
	
	map<_int, bool>			   m_mapEventFrame;		
	_int					   m_iCurrentEventFrame = {};
	
	

	vector<ANIMEVENT>		   m_vecAnimFrameEvent;	
	_int					   m_iCountFrameEvent = {};
public:
	HRESULT Save_Anim(ostream& os);
	HRESULT Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices);	

	static CAnimation* Create(const aiAnimation* pAIAnimation, const class CModel* pModel, vector<_uint>& CurrentKeyFrameIndices);
	static CAnimation* LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices);
	virtual void Free() override; 
	
};

END