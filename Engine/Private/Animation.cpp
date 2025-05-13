#include "Animation.h"
#include "Channel.h"

CAnimation::CAnimation()
{
}

HRESULT CAnimation::Initiailize(const aiAnimation* pAIAnimation, const CModel* pModel, vector<_uint>& CurrentKeyFrameIndices)
{
	/* �� �ִϸ��̼��� �̸�*/	
	memcpy(m_szName, pAIAnimation->mName.data, sizeof(_char) * MAX_PATH);	

	/* �� �ִϸ��̼��� ��ü ���� */
	m_fDuration = static_cast<_float>(pAIAnimation->mDuration);

	/* �� �ִϸ��̼��� �ʴ� ��� �Ÿ�*/
	m_fTickPerSecond = static_cast<_float>(pAIAnimation->mTicksPerSecond);

	/* �� �ִϸ��̼��� �����ϴµ� �־� �ʿ��� ���� ���� */
	m_iNumChannels = pAIAnimation->mNumChannels; 

	CurrentKeyFrameIndices.resize(m_iNumChannels);	

	m_CurrentKeyFameIndices = &CurrentKeyFrameIndices;	

	for (size_t i =0; i< m_iNumChannels; i++)
	{
		CChannel* pChannel = CChannel::Create(pAIAnimation->mChannels[i], pModel);
		if (pChannel == nullptr)
			return E_FAIL;

		m_Channels.push_back(pChannel); 
	}
	
	int test = (int)m_fDuration; 
	m_vecKeyFrameAnimationSpeed.resize(((int)(m_fDuration + 3)));		
	std::fill(m_vecKeyFrameAnimationSpeed.begin(), m_vecKeyFrameAnimationSpeed.end(), 1.f);	

	m_vecAnimFrameEvent.reserve(100);

	return S_OK;
}

_bool CAnimation::Update_TransformationMatrix(_float fTimeDelta, const vector<class CBone*>& Bones, _float* pCurrentTrackPoisiton, vector<_uint>& CurrentKeyFrameIndices, _bool isLoop)
{

	if(m_bFirst)
	{
		*pCurrentTrackPoisiton += m_fSetStartOffSetTrackPosition + m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed * m_vecKeyFrameAnimationSpeed.at((int)*pCurrentTrackPoisiton);		
		m_fCurrentKeyFramePosition = *pCurrentTrackPoisiton;	
		
		if (*pCurrentTrackPoisiton <= m_fDuration)
			m_fCurrentKeyFramePosition = m_fDuration;

		m_bFirst = false;			

 	}	

	else if (*pCurrentTrackPoisiton <= m_fDuration)
	{
		*pCurrentTrackPoisiton += m_fTickPerSecond * fTimeDelta * m_fAnimationSpeed * m_vecKeyFrameAnimationSpeed.at((int)*pCurrentTrackPoisiton);
		m_fCurrentKeyFramePosition = *pCurrentTrackPoisiton;
	}

	if (true == isLoop && *pCurrentTrackPoisiton >= m_fDuration)
	{
		m_isFinished = true;
		*pCurrentTrackPoisiton = 0.f;
	}


	/* ���� �ִϸ��̼����� �̵������� ���� �ɷ��� true�� ��ȯ�ɰ���.*/
	else if (*pCurrentTrackPoisiton >= m_fDuration)
	{
		
		m_fCurrentTrackPosition = m_fDuration;	
		m_fCurrentKeyFramePosition = m_fDuration; // �̰� ���� �̿����� .	
		
		m_isFinished = true;
		return true;
	}

	else
		m_isFinished = false; 
		



	_uint			iNumChannels = {};

	for (auto& pChannel : m_Channels)
	{
		pChannel->Update_TransformationMatrix(*pCurrentTrackPoisiton, &CurrentKeyFrameIndices[iNumChannels++], Bones);
	}

	
	//�ִϸ��̼� �̺�Ʈ
	
	for (auto& pEvent : m_vecAnimFrameEvent)	
	{
		if (pEvent.fStartTime < *pCurrentTrackPoisiton && pEvent.fEndTime > *pCurrentTrackPoisiton)
		{
			/* ���⼭ ���� ������� ��*/
			pEvent.isEventActivate = true; 
		}

		else
		{
			pEvent.isEventActivate = false;		
		}
	}
	


	return false;
}

void CAnimation::Reset(const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices, _float* pCurrentTrackPoisiton)
{
	*pCurrentTrackPoisiton = 0.0f;
	m_isFinished = false;
	m_fCurrentKeyFramePosition = 0.f;
	m_bFirst = true;	

	_uint  iChannelIndex = { 0 };
	for (auto& pChannel : m_Channels)
	{
		pChannel->Reset_TransformationMatrix(Bones, &CurrentKeyFrameIndices[iChannelIndex++]);

	}


	for (auto& Pair : m_mapEventFrame)
	{
		Pair.second = false;	
	}

	for (auto& pEvent : m_vecAnimFrameEvent)
	{
		pEvent.isPlay = false; 
		pEvent.isEventActivate = false; 
	}
}

_uint CAnimation::Get_ChannelIndex(const _char* pChannelName)	
{
	_uint	iChannelIndex = {};

	auto iter = find_if(m_Channels.begin(), m_Channels.end(), [&](CChannel* pChannel)->_bool {
		if (false == strcmp(pChannel->Get_Name(), pChannelName))
			return true;

		++iChannelIndex;
		return false;
		});

	return iChannelIndex;
}

_bool CAnimation::Lerp_NextAnimation(_float fTimeDelta, CAnimation* pNextAnimation, const vector<class CBone*>& Bones, vector<_uint>& CurrentKeyFrameIndices)
{
	m_LerpTimeAcc += fTimeDelta;

	if (pNextAnimation && m_LerpTimeAcc <= m_LerpTime)
	{
		_uint iChannelIndex = 0;


		if(pNextAnimation->Get_StartOffSetTrackPosition() > 0.f)	
		{
			for (auto& pChannel : m_Channels)
			{
				pChannel->Lerp_TransformationMatrix_Offset(Bones, pNextAnimation->m_Channels[iChannelIndex], m_LerpTime, m_LerpTimeAcc, &CurrentKeyFrameIndices[iChannelIndex], pNextAnimation->Get_StartOffSetTrackPosition());	
				iChannelIndex++;
			}
		}
		else
		{
			for (auto& pChannel : m_Channels)
			{
				pChannel->Lerp_TransformationMatrix(Bones, pNextAnimation->m_Channels[iChannelIndex], m_LerpTime, m_LerpTimeAcc, &CurrentKeyFrameIndices[iChannelIndex]);
				iChannelIndex++;
			}
		}
		
		return false;
	}

	else
	{
		m_LerpTimeAcc = 0.0f;
		return true;
	}


}

void CAnimation::Set_TrackPosition(_float _TrackPosition, vector<_uint>& CurrentKeyFrameIndices, const vector<class CBone*>& Bones)
{
	m_fCurrentTrackPosition = _TrackPosition;

	_uint			iNumChannels = {};

	for (auto& pChannel : m_Channels)	
	{
		pChannel->Set_TransfromationMatrix(m_fCurrentTrackPosition, &CurrentKeyFrameIndices[iNumChannels++], Bones);
	}	

}

void CAnimation::Set_TrackKeySpeed(_uint _istartTrackPosition, _uint _iEndTrackPosition, _float _fTrackSpeed)
{
	for(int i = _istartTrackPosition; i<= _iEndTrackPosition; i++)
	{
		m_vecKeyFrameAnimationSpeed.at(i) = _fTrackSpeed;	
	}
}

HRESULT CAnimation::Add_EventFrame(ANIMEVENT _event)
{
	
	for (auto iter = m_vecAnimFrameEvent.begin(); iter != m_vecAnimFrameEvent.end(); iter++)
	{
		if (!strcmp(iter->szName, _event.szName))
		{
			MSG_BOX("���� �̸��� �̺�Ʈ �����մϴ�.");	
			return E_FAIL;	
		}
	}

	m_vecAnimFrameEvent.push_back(_event);	
	
	return S_OK;	
}

HRESULT CAnimation::Sub_EventFrame(_char* _EventName)		
{	
	for (auto iter = m_vecAnimFrameEvent.begin(); iter != m_vecAnimFrameEvent.end(); )	
	{
		if (!strcmp(iter->szName, _EventName))
		{
			iter = m_vecAnimFrameEvent.erase(iter); // ���� �� ��ȿ�� iterator ��ȯ	
			break; 
		}
		else
		{
			++iter; // �������� �ʾ����� ���� ��ҷ� �̵�	
		}
	} 

	return S_OK;
}

HRESULT CAnimation::Save_Anim(ostream& os)
{
	os.write((char*)&m_szName, sizeof(_char) * MAX_PATH);	
	/* �ִϸ��̼� �� ���� �߰� �۾� */
	os.write((char*)&m_LerpTime, sizeof(_float));	
	os.write((char*)&m_fAnimationSpeed, sizeof(_float));	
	/* ============================== */
	os.write((char*)&m_fDuration, sizeof(_float));		

	/* �ִϸ��̼� �� ���� �߰� �۾� */
	for (int i = 0; i <= (int)m_fDuration; i++)
	{
		os.write((char*)&m_vecKeyFrameAnimationSpeed.at(i), sizeof(_float));
	}

	/* ================================*/
	os.write((char*)&m_fTickPerSecond, sizeof(_float));	
	os.write((char*)&m_fCurrentTrackPosition, sizeof(_float));
	os.write((char*)&m_isFinished, sizeof(_bool));
	os.write((char*)&m_iNumChannels, sizeof(_uint));	

	for (auto& channel : m_Channels)
		channel->Save_Channel(os);

	m_iCountFrameEvent = m_vecAnimFrameEvent.size();	

	os.write((char*)&m_iCountFrameEvent, sizeof(int));	

	for (auto& iter : m_vecAnimFrameEvent)
	{
		os.write((char*)&iter.eType, sizeof(EVENT_FRAME_TYPE));
		os.write((char*)&iter.fStartTime, sizeof(_float));
		os.write((char*)&iter.fEndTime, sizeof(_float));
		os.write((char*)&iter.isEventActivate, sizeof(_bool));
		os.write((char*)&iter.isPlay, sizeof(_bool));
		os.write((char*)&iter.szName, sizeof(_char) * MAX_PATH);	
	}

	return S_OK;	
}

HRESULT CAnimation::Load_Anim(istream& is, vector<_uint>& CurrentKeyFrameIndices)
{
	is.read((char*)&m_szName, sizeof(_char) * MAX_PATH);	
	/* �ִϸ��̼� �� ���� �߰� �۾� */
	is.read((char*)&m_LerpTime, sizeof(_float));	
	is.read((char*)&m_fAnimationSpeed, sizeof(_float));	
	/* ============================== */
	is.read((char*)&m_fDuration, sizeof(_float));		
	/* �ִϸ��̼� �� ���� �߰� �۾� */
	m_vecKeyFrameAnimationSpeed.resize(((int)m_fDuration)+1);			

	for (int i = 0; i <= (int)m_fDuration; i++)		
	{
		is.read((char*)&m_vecKeyFrameAnimationSpeed.at(i), sizeof(_float));			
	}	
	/* ============================= */
	is.read((char*)&m_fTickPerSecond, sizeof(_float));	
	is.read((char*)&m_fCurrentTrackPosition, sizeof(_float));	
	is.read((char*)&m_isFinished, sizeof(_bool));	
	is.read((char*)&m_iNumChannels, sizeof(_uint));	
	/*readVectorFromBinary(is, m_CurrentKeyFrameIndices);*/
	CurrentKeyFrameIndices.resize(m_iNumChannels);			

	for (_uint i = 0; i < m_iNumChannels; ++i)
	{
		CChannel* pChannel = CChannel::LoadCreate(is);
		m_Channels.push_back(pChannel);
	}

	is.read((char*)&m_iCountFrameEvent, sizeof(int));

	for (_uint i = 0; i < m_iCountFrameEvent; i++)	
	{
		ANIMEVENT Event;	
		is.read((char*)&Event.eType, sizeof(EVENT_FRAME_TYPE));	
		is.read((char*)&Event.fStartTime, sizeof(_float));		
		is.read((char*)&Event.fEndTime, sizeof(_float));	
		is.read((char*)&Event.isEventActivate, sizeof(_bool));	
		is.read((char*)&Event.isPlay, sizeof(_bool));	
		is.read((char*)&Event.szName, sizeof(_char) * MAX_PATH);	

		m_vecAnimFrameEvent.push_back(Event);	
	}

	return S_OK;
}

CAnimation* CAnimation::Create(const aiAnimation* pAIAnimation, const CModel* pModel, vector<_uint>& CurrentKeyFrameIndices)
{
	CAnimation* pInstance = new CAnimation();

	if (FAILED(pInstance->Initiailize(pAIAnimation, pModel,CurrentKeyFrameIndices)))	
	{
		MSG_BOX("Failed to Created : Animation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CAnimation* CAnimation::LoadCreate(istream& is, vector<_uint>* _CurrentKeyFrameIndices)
{
	CAnimation* pInstance = new CAnimation();	
	if (FAILED(pInstance->Load_Anim(is, *_CurrentKeyFrameIndices)))	
	{
		MSG_BOX("Failed To LoadCreated : CAnimation");	
		Safe_Release(pInstance);
	}

	return pInstance;	
}

void CAnimation::Free()
{
	__super::Free();
	
	for (auto& pChannel : m_Channels)
		Safe_Release(pChannel);

	/*for (auto& pVecEvent : m_vecAnimFrameEvent)
		delete[] pVecEvent.szName;	*/

	m_vecAnimFrameEvent.clear();

}
