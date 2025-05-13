#pragma once

#include "Base.h"

/* ���� ������(aiNode, aiBone, aiAniNode) �� ǥ���Ѵ�. */
/* aiNode :  ������� ���� ������ ǥ�����ش�. ���� ������ ������ ���� ���� ����� ���ŵǾ�� �ϱ� ������.*/

BEGIN(Engine)

class CBone final : public CBase
{
private:
	CBone();
	virtual ~CBone() = default;


public:
	const _char* Get_Name()
	{
		return m_szName;	
	}


	_matrix Get_CombinedTrasnformationMatrix() const
	{
		return XMLoadFloat4x4(&m_CombinedTransformationMatrix);	
	}

	const _float4x4* Get_CombinedTransformationPtr() const {
		return &m_CombinedTransformationMatrix;	
	}

	const _float4x4* Get_CombinedRootMotionTransformationPtr() const {	
		return &m_RootMotionMatrix;	
	}	


	void Set_TransformationMatrix(_fmatrix TransformationMatrix) {	
		XMStoreFloat4x4(&m_TransformationMatrix, TransformationMatrix);	
	}


public:
	HRESULT Initialize(const aiNode* pAINode, _int iParentBoneIndex);
	void Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix);	
	/* const vector<class CBone*>* Bones �� �ϸ� �ȵǴ� ������ ���Ϳ� push back�� �� �� ������ ũ�⺸�� ũ�� ���ʹ�
	�ƿ� �� �����ϰ� �ٽ� �޸� �Ҵ��ؼ� ���ο� �޸� ������ �����ϱ� ������ �����ͷ� ������ �ش� ������ ù�ּ��̱� ������
	ù �ּҰ� nullptr�� �� �� �ִ�.*/

	/* �׿��� ��������! �̿���!*/

private:
	_char			   m_szName[MAX_PATH] = {};

	/* �θ�������� ǥ���� �ڽĻ�(�����)���� ��������� ǥ���Ѵ�. �� �θ�������� �ڽ��� ���� �����̽� ��� �ǹ� */
	_float4x4          m_TransformationMatrix = {};


	/* �� �� ��� * �θ��� ������� ( �ڽ��� ���ý����̽� ��� * �θ��� ���� �����̽� ��� )  */
	_float4x4          m_CombinedTransformationMatrix = {};

	_int               m_iParentBoneIndex = { -1 };


	/* ��Ʈ ��� �̵���� */
	_float4x4		   m_RootMotionMatrix = {};	
	_float4x4		   m_pPreRootMatrix = {};	

public:
	HRESULT Save_Bone(ostream& os);
	HRESULT Load_Bone(istream& os);
	static CBone* Create(const aiNode* pAINode, _int iParentBoneIndex);
	static CBone* LoadCreate(std::istream& os);

	CBone* Clone();	
	virtual void Free() override;

};

END