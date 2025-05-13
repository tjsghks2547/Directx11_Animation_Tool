#pragma once

#include "Base.h"


/* 채널에서 해야할 일들
   해당 애니메이션의 뼈행렬의 정보를 입력해야함 */

/* 뼈행렬은 3개로 (크기,회전,이동)으로 나뉨*/
/* 그래서 해당 뼈의 Transformatrix 의 정보를 갱신해줄거임.*/

BEGIN(Engine)
class CChannel : public CBase
{
private:
	CChannel();
	virtual ~CChannel() = default; 

public:
	HRESULT Initialize(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	void Update_TransformationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones);
	void Lerp_TransformationMatrix(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex);
	void Lerp_TransformationMatrix_Offset(const vector<class CBone*>& Bones, CChannel* pNextChannel, _float LerpTime, _float LerpTimeAcc, _uint* pCurrentKeyFrameIndex, _float OffSet);
	void Reset_TransformationMatrix(const vector<class CBone*>& Bones, _uint* pCurrentKeyFrameIndex);
	
	/* 2월 20일 */
	void Set_TransfromationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones);	

	const _char* Get_Name() const {	
		return m_szName;	
	}

private:
	/* 이 뼈가 특정 애니메이션을 구동하기 위해 특정 시간마다의 상태를 가진다*/
	char				m_szName[MAX_PATH] = {};

	/* KeyFrame : 특정 시간에 표현해야할 뼈의 상태. */
	_uint				m_iNumKeyframes = {};
	vector<KEYFRAME>	m_Keyframes;

	/* 이 채널(뼈)의 이름과 같은 이름을 가진 모델(전체뼈)중의 뼈를 찾아서 그 인덱스를 저장한다.*/
	_uint               m_iBoneIndex = {};

public:
	HRESULT Save_Channel(ostream& os);	
	HRESULT Load_Channel(istream& is);
	static CChannel* LoadCreate(istream& is);	
	static CChannel* Create(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	virtual void Free() override; 


};

END