#include "Bone.h"

CBone::CBone()
{
}

HRESULT CBone::Initialize(const aiNode* pAINode, _int iParentBoneIndex)
{
	/* 여기서 Create 하고 난 뒤의 호출되는 함수니깐 bone의 transformation과 combinedTrasnformation 정보를 채워줘야할듯*/
	strcpy_s(m_szName, pAINode->mName.data);	

	/*pAINode->mTransformation**은 해당 노드의 로컬 변환 행렬*/
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

	/* 루트 모션 애니메이션 이동 행렬 저장하기*/
	if (m_iParentBoneIndex == 1)
	{

		XMMATRIX matrix = XMMatrixIdentity();


		if (fabs(m_CombinedTransformationMatrix._41) < 1e-5f &&	
			fabs(m_CombinedTransformationMatrix._42) < 1e-5f &&	
			fabs(m_CombinedTransformationMatrix._43) < 1e-5f)	
		{
			// 단위 행렬로 초기화		
			XMStoreFloat4x4(&m_pPreRootMatrix, XMMatrixIdentity());

			/* 보간 때 발생하는 문제 해결해야함 */
		}


		// 단위 행렬의 회전/스케일 부분 (_11 ~ _33)을 유
		matrix.r[3] = XMVectorSubtract(XMLoadFloat4x4(&m_CombinedTransformationMatrix).r[3], XMLoadFloat4x4(&m_pPreRootMatrix).r[3]);
		matrix.r[3] = XMVectorSetW(matrix.r[3], 1.f); // W 값을 1로 고정	

		// 나머지 이동 값은 그대로 유지
		// 네 번째 행 (_41, _42, _43, _44)은 변경하지 않음


		// 결과를 저장
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
