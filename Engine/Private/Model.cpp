#include "Model.h"
#include "Mesh.h"
#include "Shader.h"	
#include "Material.h"
#include "Bone.h"
#include "Animation.h"	

CModel::CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice,pContext)
{
}

CModel::CModel(const CModel& Prototype)
	:CComponent(Prototype)
	,m_iNumMeshes (Prototype.m_iNumMeshes)	
	,m_Meshes(Prototype.m_Meshes)	
	,m_iNumMaterials{ Prototype.m_iNumMaterials }	
	,m_Materials{ Prototype.m_Materials }	
	,m_eModelType{ Prototype.m_eModelType }	
	/* 11�� 25�� �߰� ( ���� )*/
	,m_iNumAnimations{ Prototype.m_iNumAnimations}	
	,m_Animations{Prototype.m_Animations}	
	/* 12�� 2�� �߰�*/
	,m_fullpath{Prototype.m_fullpath}		
	,m_CurrentKeyFrameIndices{ Prototype.m_CurrentKeyFrameIndices }	
	,m_PreTransformMatrix{ Prototype.m_PreTransformMatrix }	
{

	for (auto& pAnimation : m_Animations)	
		Safe_AddRef(pAnimation);	

	for (auto& pPrototypeBone : Prototype.m_Bones)	
		m_Bones.push_back(pPrototypeBone->Clone());	

	for (auto& pMesh : m_Meshes)
		Safe_AddRef(pMesh);

	for (auto& pMaterial : m_Materials)
		Safe_AddRef(pMaterial);

}

_uint CModel::Get_BoneIndex(const _char* pBoneName) const
{
	_uint  iBoneIndex = {};
	/* ���⼭ �� �ε����� ã�� ������ m_Bones ���� �ȿ� ����ִ� m_szName ������( �� �̸�)*/
	/* ����� �� �� �̸��� aiNode���� ������*/

	/* find_if�� ������ �� 3��° �Ű������� iterator�� �Ѱ��ش� */
	/* pBoneName �Ű� ���� ����� �� �ϱ� []�ȿ� & �־��ֱ� */
	auto iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			return true;
		++iBoneIndex;
		return false; });

	return iBoneIndex;
}

const _float4x4* CModel::Get_BoneMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {
		if (false == strcmp(pBone->Get_Name(), pBoneName))
			return true;
		return false;
		});

	if (iter == m_Bones.end())
		return nullptr;

	return (*iter)->Get_CombinedTransformationPtr();
}

HRESULT CModel::Initialize_Prototype(MODEL eModelType, const _char* pModelFilePath, _fmatrix PreTransformMatrix)
{
	_uint			iFlag = {};

	/* ���̳ʸ�ȭ ���� �ּ� ����*/
	m_fullpath = pModelFilePath;	

	iFlag = aiProcess_ConvertToLeftHanded | aiProcessPreset_TargetRealtime_Fast;

	if (MODEL_NONANIM == eModelType)	
		iFlag |= aiProcess_PreTransformVertices;	

	m_eModelType = eModelType;

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);	

	/* �� ���̳ʸ�ȭ �� ��������.*/	
	if (FAILED(Load_Model(PreTransformMatrix)))											
		return E_FAIL;					


	/* �̰͸����� ��� �ε��۾��� �����Ŵ�. */
	//m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);
	//if (nullptr == m_pAIScene)
	//	return E_FAIL;

	/* ��Ŭ������ �ָ��� : ���� �����̰� �׸���. */
	/* �ε����� �����͸� ���̽����ؼ� �����̰� �׸������� �� ������ �°� �����ϴ� �۾��� �ʿ��ϴ�. */

	/* Ready_Bones������ ���� �����̽������� ���� ������ �ִ� ����� ������� m_CombinedMatrix�� ����*/
	/* Initialize����*/	
				
	
	/* �� ���� ������ �ִ� �Ž��� ���� ��ŭ �ݺ����� ���Ƽ� mesh �� �� ��ҿ� �� ���� � */
	/* ���� �ε����� ������ �޴��� �� ��ŭ ������ �޴����� ���� ����ġ�� ������*/
	/* �׸��� �� �Ž��� � material�� ������ �ִ��� �� ���� ������ ������*/
											
	
	/* �� ���� ������ �ִ� �ؽ�ó �� �����ͼ� �ش� fbx����*/
	/* ��� matrieal�� ����ϴ��� �ľ��ϰ� �ش� material�� �� ��ŭ �ݺ����� ����*/
	/* �ش� ���͸����� �� 18���� �ڷḦ ���� ( diffuse , specluar, ��Ÿ ��� )*/
	/* �̷��� �͵� �� m_Materials�� �������� (���͸��� Ŭ������ �����̳� ���ͷ� )*/
	/* ���͸��� Ŭ������ �ش� �ڷḦ ShaderResourceView�� ���� �ȿ� ����*/
	

	/* �̰͸����� ��� �ε��۾��� �����Ŵ�. */
	//m_pAIScene = m_Importer.ReadFile(pModelFilePath, iFlag);					
	//if (nullptr == m_pAIScene)	
	//	return E_FAIL;		
	//
	//if (FAILED(Ready_Bones(m_pAIScene->mRootNode)))				
	//	return E_FAIL;	
	//
	//if (FAILED(Ready_Meshes(PreTransformMatrix)))		
	//	return E_FAIL;	
	//
	//if (FAILED(Ready_Materials(pModelFilePath)))		
	//	return E_FAIL;		
	//
	//if (FAILED(Ready_Animations()))				
	//	return E_FAIL;			
	//
	//
	//if (FAILED(Save_Model(pModelFilePath)))												
	//	return E_FAIL;										

	return S_OK;
}

HRESULT CModel::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CModel::Render(_uint iMeshIndex)
{
	m_Meshes[iMeshIndex]->Bind_InputAssembler();	
	m_Meshes[iMeshIndex]->Render();

	return S_OK;
}

void CModel::SetUp_Animation(_uint iAnimIndex, _bool isLoop)
{
	if (m_bFirst)
	{
		m_iCurrentAnimIndex = iAnimIndex;
		m_bFirst = false;
	}

	if ((m_iCurrentAnimIndex == iAnimIndex || m_bLerpFinished == false) && !m_bFirst)	
	{
		m_isAnimLoop = isLoop;	
		return;
	}

	m_iCurrentAnimationIndex = m_iCurrentAnimIndex;		
	/* �ٸ��ٸ�  �ִϸ��̼� �ε����� ��ü���ش�.*/	
	m_pNextAnimation = m_Animations[iAnimIndex];	
	m_pCurrentAnimation = m_Animations[m_iCurrentAnimationIndex];	

	m_iCurrentAnimIndex = iAnimIndex;	
	m_Animations[m_iCurrentAnimIndex]->Reset(m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], &m_fCurrentTrackPosition);
	m_bFinished = false;	

	m_isAnimLoop = isLoop;	
}

_bool CModel::Play_Animation(_float fTimeDelta)	
{
	/* ������ ���������� üũ������ҰŰ��� �׷��� �ִϸ��̼��� �Ⱦ����*/


	if (m_pNextAnimation)
	{
		m_bLerpFinished = false;
		/* ���⼭ ���� ���� �������ְ�!*/
		if (m_pCurrentAnimation->Lerp_NextAnimation(fTimeDelta, m_pNextAnimation, m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimationIndex]))	
		{
			m_pNextAnimation = nullptr;
		}
		/* ������ ������ �� ���ǹ��� ������ϴϱ�.*/
	}

	/* �� ������ duration�� ���������� �ִϸ��̼� ���� �߱� ������ acctime�� ���Ƽ� ������ �߻� */

	else
	{
		/* ���� �ִϸ��̼��� ����ð��� �´� ���� ���¸� �޾ƿͼ� ������ ������ �ִ� TrnasformationMatrix�� �������ش�. */
		/*_bool   isFinished = */
		if(m_Animations[m_iCurrentAnimIndex]->Update_TransformationMatrix(fTimeDelta, m_Bones, &m_fCurrentTrackPosition, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], m_isAnimLoop))
		{
			m_bFinished = true;
		}
		else
		{
			m_bFinished = false; 
		}

		m_bLerpFinished = true;	
	}


	/* ��� ���� CombinedTransformationMatrix�� �����Ѵ�. */
	/* CombinedTransformationMatrix = ���� TrnasformationMatrix * �θ��� Combined */

	for (auto& pBone : m_Bones)
	{
		pBone->Update_CombinedTransformationMatrix(m_Bones, XMLoadFloat4x4(&m_PreTransformMatrix));
	}

	//return isFinished;
	return true;
}

HRESULT CModel::Bind_Material(CShader* pShader, _uint iMeshIndex, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex)
{
	_uint  iMaterialIndex = m_Meshes[iMeshIndex]->Get_MaterialIndex();


	return m_Materials[iMaterialIndex]->Bind_Material(pShader, eType, pConstantName,iTextureIndex);
}

HRESULT CModel::Bind_BoneMatrices(CShader* pShader, _uint iMeshIndex, const _char* pConstantName)
{
	/* �� �Ž� Ŭ������ �� �Ž��� �ش��ϴ� ���� ����� �迭���·� ������*/
	if (iMeshIndex >= m_iNumMeshes)	
		E_FAIL;	

	return m_Meshes[iMeshIndex]->Bind_BoneMatrices(pShader,pConstantName,m_Bones);
}

void CModel::Reset_CurAnimation()
{
	m_Animations[m_iCurrentAnimIndex]->Reset(m_Bones, m_CurrentKeyFrameIndices[m_iCurrentAnimIndex], &m_fCurrentTrackPosition);	
}

void CModel::Reset_Animation(_uint _iAnimationIndex)
{
	m_Animations[_iAnimationIndex]->Reset(m_Bones, m_CurrentKeyFrameIndices[_iAnimationIndex], &m_fCurrentTrackPosition);	
}

const _float4x4* CModel::Get_RootMotionMatrix(const _char* pBoneName) const
{
	auto	iter = find_if(m_Bones.begin(), m_Bones.end(), [&](CBone* pBone)->_bool {		
		if (false == strcmp(pBone->Get_Name(), pBoneName))	
			return true;	
		return false;
		});	

	if (iter == m_Bones.end())
		return nullptr;

	return (*iter)->Get_CombinedRootMotionTransformationPtr();	
}

HRESULT CModel::Save_Model(const _char* pModelFilePath)
{
	string binaryFile = "../Bin/BinaryFile/";
	// ���� �̸��� ���Ե� ���ڿ�


	// ���� �̸����� ���丮 ��θ� ������ �κ� ���� 
	string filenamePart(m_fullpath);	
	size_t lastSlashIndex = filenamePart.find_last_of('/');
	if(lastSlashIndex != string::npos)
	{
		filenamePart = filenamePart.substr(lastSlashIndex + 1);	
	}

	size_t lastDotIndex = filenamePart.find_last_of('.');
	if(lastDotIndex != string::npos)
	{
		filenamePart = filenamePart.substr(0, lastDotIndex) + ".bin";
	}

	// �������� ��� ����
	binaryFile += filenamePart;	

	ofstream file(binaryFile, ios::out | std::ios::binary);
	//binaryFile = ��� + �̸�, ���⿡ �Ȱ��� ������ ������ �����, ������ ���� �����.

	if(file.is_open())
	{
		//���� ���� �κ�
		file.write(reinterpret_cast<char*>(&m_PreTransformMatrix), sizeof(_float4x4));
	
		/* �� ���� */
		_uint bonesize = m_Bones.size();	
		file.write((char*)&bonesize, sizeof(_uint));	
		for (auto& bone : m_Bones)	
			bone->Save_Bone(file);	
		//


		/* �Ž� ���� */
		file.write((char*)&m_iNumMeshes, sizeof(_uint));	
		for (auto& Mesh : m_Meshes)	
			Mesh->Save_Mesh(file);
		//


		/* Material ���� */
		file.write((char*)&m_iNumMaterials, sizeof(_uint));
		/*for (auto& Material : m_Materials) 
		{*/
		for(size_t i =0; i<m_iNumMaterials; i++)
		{
			m_Materials[i]->Save_Material(file, m_pAIScene->mMaterials[i], pModelFilePath);
		}
		//}
		//

		/* �ִϸ��̼� �� ä�� ����*/
		file.write((char*)&m_iNumAnimations, sizeof(_uint));	

		_uint AnimSize = m_Animations.size();	
		file.write((char*)&AnimSize, sizeof(_uint));
		for (auto& Anim : m_Animations)
			Anim->Save_Anim(file);
		//

		file.close();	
	}

	else
	{
		MSG_BOX("Failed To OpenFile");	
		file.close();	
		return E_FAIL;	
	}


	return S_OK;
}

HRESULT CModel::Load_Model(_fmatrix PreTransformMatrix)
{
	string binaryFile = "../Bin/BinaryFile/";
	// ���� �̸��� ���Ե� ���ڿ�


	// ���� �̸����� ���͸� ��θ� ������ �κ� ����
	string filenamePart(m_fullpath);
	size_t lastSlashIndex = filenamePart.find_last_of('/');
	if(lastSlashIndex != string::npos)
	{
		filenamePart = filenamePart.substr(lastSlashIndex + 1);
	}

	// Ȯ���ڸ� bin ���� ���� 
	size_t lastDotIndex = filenamePart.find_last_of('.');
	if(lastDotIndex != string::npos)
	{
		filenamePart = filenamePart.substr(0, lastDotIndex) + ".bin";
	}

	// �������� ��� ���� 
	binaryFile += filenamePart;	

	ifstream inFile(binaryFile, std::ios::binary);
	if (!inFile.good()) // ��� �ȿ� ������ ������
	{
		isFile = false;  // isFile �� false�ϰ� ����
		return S_OK;
	}
	else
		isFile = true;   // isFile�� true�ϰ� ��� ����


	if(!inFile.is_open())
	{
		// ���� ���� ���� ����
		MSG_BOX("Failed to Open File");
		return E_FAIL;
	}

	else
	{
		inFile.read(reinterpret_cast<char*>(&m_PreTransformMatrix), sizeof(_float4x4));	
		

		_uint bonenum = 0; 
		inFile.read((char*)&bonenum, sizeof(_uint));
		for(size_t i=0; i < bonenum; ++i)
		{
			CBone* bone = CBone::LoadCreate(inFile);
			m_Bones.push_back(bone);	
		}


		inFile.read((char*)&m_iNumMeshes, sizeof(_uint));			
		for (size_t i = 0; i < m_iNumMeshes; ++i)		
		{
			CMesh* mesh = CMesh::LoadCreate(m_pDevice, m_pContext, inFile);		
			m_Meshes.push_back(mesh);			
		}


		inFile.read((char*)&m_iNumMaterials, sizeof(_uint));
		for (size_t i =0 ; i < m_iNumMaterials; ++i)
		{
			CMaterial* Material = CMaterial::LoadCreate(m_pDevice, m_pContext, inFile);
			m_Materials.push_back(Material);
		}


		inFile.read((char*)&m_iNumAnimations, sizeof(_uint));		
		_uint AnimSize = 0;	
		inFile.read((char*)&AnimSize, sizeof(_uint));
		m_CurrentKeyFrameIndices.resize(m_iNumAnimations);	
		for (size_t i = 0; i < AnimSize; ++i)	
		{
			CAnimation* Anim = CAnimation::LoadCreate(inFile,&m_CurrentKeyFrameIndices[i]);
			m_Animations.emplace_back(Anim);	
		}

	}

	inFile.close();		

	XMStoreFloat4x4(&m_PreTransformMatrix, PreTransformMatrix);	


	return S_OK;	
}

HRESULT CModel::Ready_Meshes(_fmatrix PreTransformMatrix)
{
	m_iNumMeshes = m_pAIScene->mNumMeshes;

	for (size_t i = 0; i < m_iNumMeshes; i++)
	{
		CMesh* pMesh = CMesh::Create(m_pDevice, m_pContext,m_eModelType,this, m_pAIScene->mMeshes[i],PreTransformMatrix);
		if (nullptr == pMesh)
			return E_FAIL;

		m_Meshes.push_back(pMesh);
	};

	return S_OK;
}

HRESULT CModel::Ready_Materials(const _char* pModelFilePath)
{
	m_iNumMaterials = m_pAIScene->mNumMaterials;

	for (size_t i = 0; i< m_iNumMaterials; i++)
	{
		CMaterial* pAIMaterial = CMaterial::Create(m_pDevice, m_pContext, m_pAIScene->mMaterials[i], pModelFilePath);

		if (nullptr == pAIMaterial)
			return E_FAIL;
		
		m_Materials.push_back(pAIMaterial);
	}

	return S_OK;	
}

HRESULT CModel::Ready_Bones(aiNode* pAINode, _int iParentBoneIndex)
{
	CBone* pBone = CBone::Create(pAINode, iParentBoneIndex);
	if (nullptr == pBone)
		return E_FAIL;

	m_Bones.push_back(pBone);

	_int iParent = m_Bones.size() - 1; 

	for(size_t i =0; i< pAINode->mNumChildren; i++)
	{
		Ready_Bones(pAINode->mChildren[i], iParent);
	}

	return S_OK;	
}

HRESULT CModel::Ready_Animations()
{
	m_iNumAnimations = m_pAIScene->mNumAnimations; 

	m_CurrentKeyFrameIndices.resize(m_iNumAnimations);

	for(size_t i=0; i<m_iNumAnimations; i++)
	{
		CAnimation* pAnimation = CAnimation::Create(m_pAIScene->mAnimations[i], this, m_CurrentKeyFrameIndices[i]);	
		if (nullptr == pAnimation)
			return E_FAIL; 

		m_Animations.push_back(pAnimation);
	}

	return S_OK;
}



CModel* CModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, MODEL eModelType, _fmatrix PreTransformMatrix)
{
	CModel* pInstance = new CModel(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eModelType, pModelFilePath, PreTransformMatrix)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CModel::Clone(void* pArg)
{
	CModel* pInstance = new CModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}




/* ���⼭ ���̺� �۾� IMGUI ���� �۾� ���� */
HRESULT CModel::Save_Model_File()
{
		
	/* ���� Save �۾� ���� */

	string binaryFile = "../Bin/BinaryFile/";
	// ���� �̸��� ���Ե� ���ڿ�


	// ���� �̸����� ���丮 ��θ� ������ �κ� ���� 
	string filenamePart(m_fullpath);
	size_t lastSlashIndex = filenamePart.find_last_of('/');
	if (lastSlashIndex != string::npos)
	{
		filenamePart = filenamePart.substr(lastSlashIndex + 1);
	}

	size_t lastDotIndex = filenamePart.find_last_of('.');
	if (lastDotIndex != string::npos)
	{
		filenamePart = filenamePart.substr(0, lastDotIndex) + ".bin";	
	}

	// �������� ��� ����
	binaryFile += filenamePart;

	ofstream file(binaryFile, ios::out | std::ios::binary);
	//binaryFile = ��� + �̸�, ���⿡ �Ȱ��� ������ ������ �����, ������ ���� �����.

	if (file.is_open())
	{
		//���� ���� �κ�
		file.write(reinterpret_cast<char*>(&m_PreTransformMatrix), sizeof(_float4x4));

		/* �� ���� */
		_uint bonesize = m_Bones.size();
		file.write((char*)&bonesize, sizeof(_uint));
		for (auto& bone : m_Bones)
			bone->Save_Bone(file);
		//


		/* �Ž� ���� */
		file.write((char*)&m_iNumMeshes, sizeof(_uint));
		for (auto& Mesh : m_Meshes)
			Mesh->Save_Mesh(file);
		//


		/* Material ���� */
		file.write((char*)&m_iNumMaterials, sizeof(_uint));
		/*for (auto& Material : m_Materials)
		{*/
		for (size_t i = 0; i < m_iNumMaterials; i++)
		{
			m_Materials[i]->Save_Pass_Material(file);	
		}
		//}
		//

		/* �ִϸ��̼� �� ä�� ����*/
		file.write((char*)&m_iNumAnimations, sizeof(_uint));

		_uint AnimSize = m_Animations.size();
		file.write((char*)&AnimSize, sizeof(_uint));
		for (auto& Anim : m_Animations)
			Anim->Save_Anim(file);
	

		file.close();
	}

	else
	{
		MSG_BOX("Failed To OpenFile");
		file.close();
		return E_FAIL;
	}

	MSG_BOX("Success Save File");

	return S_OK;
}
/* ========================================= */

void CModel::Free()
{
	__super::Free();

	for (auto& pAnimation : m_Animations)	
		Safe_Release(pAnimation);	

	for (auto& pBone : m_Bones)	
		Safe_Release(pBone);	

	for (auto& pMaterial : m_Materials)	
		Safe_Release(pMaterial);	

	for (auto& pMesh : m_Meshes)	
		Safe_Release(pMesh);	

	m_Animations.clear();	
	m_Bones.clear();	
	m_Materials.clear();		
	m_Meshes.clear();	

}
