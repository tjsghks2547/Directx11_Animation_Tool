#pragma once

#include "Base.h"


/* ä�ο��� �ؾ��� �ϵ�
   �ش� �ִϸ��̼��� ������� ������ �Է��ؾ��� */

/* ������� 3���� (ũ��,ȸ��,�̵�)���� ����*/
/* �׷��� �ش� ���� Transformatrix �� ������ �������ٰ���.*/

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
	
	/* 2�� 20�� */
	void Set_TransfromationMatrix(_float fCurrentTrackPosition, _uint* pCurrentKeyFrameIndex, const vector<class CBone*>& Bones);	

	const _char* Get_Name() const {	
		return m_szName;	
	}

private:
	/* �� ���� Ư�� �ִϸ��̼��� �����ϱ� ���� Ư�� �ð������� ���¸� ������*/
	char				m_szName[MAX_PATH] = {};

	/* KeyFrame : Ư�� �ð��� ǥ���ؾ��� ���� ����. */
	_uint				m_iNumKeyframes = {};
	vector<KEYFRAME>	m_Keyframes;

	/* �� ä��(��)�� �̸��� ���� �̸��� ���� ��(��ü��)���� ���� ã�Ƽ� �� �ε����� �����Ѵ�.*/
	_uint               m_iBoneIndex = {};

public:
	HRESULT Save_Channel(ostream& os);	
	HRESULT Load_Channel(istream& is);
	static CChannel* LoadCreate(istream& is);	
	static CChannel* Create(const aiNodeAnim* pAIChannel, const class CModel* pModel);
	virtual void Free() override; 


};

END