#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	/* ���⼭ Create �ϰ� �� ���� ȣ��Ǵ� �Լ��ϱ� bone�� transformation�� combinedTrasnformation ������ ä������ҵ�*/
	strcpy_s(m_szName, pAINode->mName.data);	

	/*pAINode->mTransformation**�� �ش� ����� ���� ��ȯ ���*/
	memcpy(&m_TransformationMatrix, &pAINode->mTransformation, sizeof(_float4x4));	

	XMStoreFloat4x4(&m_TransformationMatrix, XMMatrixTranspose(XMLoadFloat4x4(&m_TransformationMatrix)));	

	XMStoreFloat4x4(&m_CombinedTransformationMatrix, XMMatrixIdentity());

	m_iParentBoneIndex = iParentBoneIndex;
	
	return S_OK;
}

void CBone::Update_CombinedTransformationMatrix(const vector<class CBone*>& Bones, _fmatrix PreTransformMatrix)
{
	if (-1 == m_iParentBoneIndex)
		XMStoreFloat4x4(&m_CombinedTransformationMatrix, PreTransformMatrix * XMLoadFloat4x4(&m_TransformationMatrix));

	else
	{
		XMStoreFloat4x4(&m_CombinedTransformationMatrix,
			XMLoadFloat4x4(&m_TransformationMatrix) * XMLoadFloat4x4(&Bones[m_iParentBoneIndex]->m_CombinedTransformationMatrix));
	}

	/* ��Ʈ ��� �ִϸ��̼� �̵� ��� �����ϱ�*/
	if (m_iParentBoneIndex == 1)
	{

		XMMATRIX matrix = XMMatrixIdentity();


		if (fabs(m_CombinedTransformationMatrix._41) < 1e-5f &&	
			fabs(m_CombinedTransformationMatrix._42) < 1e-5f &&	
			fabs(m_CombinedTransformationMatrix._43) < 1e-5f)	
		{
			// ���� ��ķ� �ʱ�ȭ		
			XMStoreFloat4x4(&m_pPreRootMatrix, XMMatrixIdentity());

			/* ���� �� �߻��ϴ� ���� �ذ��ؾ��� */
		}


		// ���� ����� ȸ��/������ �κ� (_11 ~ _33)�� ��
		matrix.r[3] = XMVectorSubtract(XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3], XMLoadFloat4x4(&m_pPreRootMatrix).r[3]);
		matrix.r[3] = XMVectorSetW(matrix.r[3], 1.f); // W ���� 1�� ����	

		// ������ �̵� ���� �״�� ����
		// �� ��° �� (_41, _42, _43, _44)�� �������� ����


		// ����� ����
		XMStoreFloat4x4(&m_RootMotionMatrix, matrix);
		XMStoreFloat4x4(&m_pPreRootMatrix, XMLoadFloat4x4(&m_CombinedTransformationMatrix));


		m_CombinedTransformationMatrix._41 = 0.f;	
		m_CombinedTransformationMatrix._42 = 0.f;	
		m_CombinedTransformationMatrix._43 = 0.f;	
		m_CombinedTransformationMatrix._44 = 1.f;	
	}
}

HRESULT CBone::Save_Bone(ostream& os)
{

	os.write(m_szName, sizeof(_char) * MAX_PATH);
	os.write((char*)&m_iParentBoneIndex, sizeof(_int));
	os.write((char*)&m_TransformationMatrix, sizeof(_float4x4));

	return S_OK;	
}

HRESULT CBone::Load_Bone(istream& os)
{
	
	os.read(m_szName, sizeof(_char) * MAX_PATH);
	os.read((char*)&m_iParentBoneIndex, sizeof(_int));
	os.read((char*)&m_TransformationMatrix, sizeof(_float4x4));
	return S_OK;	
}

CBone* CBone::Create(const aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pInstance = new CBone(); 

	if(FAILED(pInstance->Initialize(pAINode,iParentBoneIndex)))
	{
		MSG_BOX("Failed to Created : Bone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CBone* CBone::LoadCreate(std::istream& os)
{
	CBone* pInstance = new CBone();

	if(FAILED(pInstance->Load_Bone(os)))
	{
		MSG_BOX("Failed to Created : Bone");
		Safe_Release(pInstance);
	}

	return pInstance;	
}

CBone* CBone::Clone()
{
	return new CBone(*this);
}

void CBone::Free()
{
	__super::Free();
}
