#pragma once

#include "Component.h"


BEGIN(Engine)



class ENGINE_DLL CModel final : public CComponent
{
public:
	enum MODEL { MODEL_NONANIM, MODEL_ANIM, MODEL_END};

private:
	CModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	CModel(const CModel& Prototype);
	virtual ~CModel() = default;

public:
	_uint Get_NumMeshes() const {
		return m_iNumMeshes;
	}

	_uint Get_BoneIndex(const _char* pBoneName) const;
	const _float4x4* Get_BoneMatrix(const _char* pBoneName) const;	

public:
	virtual HRESULT Initialize_Prototype(MODEL eModelType,const _char* pModelFilePath, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;
	virtual HRESULT Render(_uint iMeshIndex);


public:
	void  SetUp_Animation(_uint iAnimIndex, _bool isLoop);		
	_bool Play_Animation(_float fTimeDelta);	
	HRESULT Bind_Material(class CShader* pShader, _uint iMeshIndex, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex = 0);
	HRESULT Bind_BoneMatrices(class CShader* pShader, _uint iMeshIndex, const _char* pConstantName);
	vector<class CMesh*>& Get_Meshes() { return m_Meshes; }	

	vector<class CAnimation*>& Get_VecAnimation() { return  m_Animations; }
	vector<class CBone*>& Get_Bones() { return 	m_Bones; }	
	vector<vector<_uint>>& Get_CurrentKeyFrameIndices() { return m_CurrentKeyFrameIndices; }	

	_bool			 GetAniFinish() { return m_bFinished; }
	void			 Reset_CurAnimation();
	void			 Reset_Animation(_uint _iAnimationIndex);
					 
	_float			 Get_CurrentAnmationTrackPosition() { return m_fCurrentTrackPosition; }	

	const _float4x4* Get_RootMotionMatrix(const _char* pBoneName) const;	
	_bool			 Get_LerpFinished() { return m_bLerpFinished; }	
	_uint			 Get_CurrentAnimIndex() { return m_iCurrentAnimIndex; }
private:
	/* 가져온 정보를 저장한다. */
	const aiScene* m_pAIScene = { nullptr };

	/* 파일을 뜯어서 읽고 정리해준다. */
	Assimp::Importer				m_Importer = {};	

private:
	_uint							m_iNumMeshes = {};
	vector<class CMesh*>			m_Meshes;

	MODEL							m_eModelType = MODEL_END;	

private:
	_uint                           m_iNumMaterials = {};	
	vector<class CMaterial*>	    m_Materials;


private:
	vector<class CBone*>			m_Bones; 

private:
	_uint							m_iNumAnimations = {};
	_uint							m_iCurrentAnimIndex = {};
	_uint							m_iPreviousAnimIndex = {};

	_float							m_fCurrentTrackPosition = {};
	_float4x4						m_PreTransformMatrix;	


	_bool							m_isAnimLoop = {};
	_bool							m_bFirst = { true };	

	vector<class CAnimation*>       m_Animations;	
	vector<vector<_uint>>			m_CurrentKeyFrameIndices; /* 애니메이션을 관리 하는 인덱스 */	

private:	
	/* 선형보간 추가작업*/
	CAnimation* m_pNextAnimation         = nullptr;	
	CAnimation* m_pCurrentAnimation      = nullptr;	

	_bool       m_bFinished              = { false };	
	_bool       m_bLerpFinished          = { false };	

	_uint       m_iNextAnimationIndex    = {};	
	_uint       m_iCurrentAnimationIndex = {};		



/* 바이너리화 관련*/
private:
	string						    m_fullpath;		
	//_float4x4					    m_PreTransformMatrix;	
	_bool						    isFile = false;
private:
	HRESULT		Save_Model(const _char* pModelFilePath);	
	HRESULT     Load_Model(_fmatrix PreTransformMatrix);	


private:
	HRESULT Ready_Meshes(_fmatrix PreTransformMatrix);
	HRESULT Ready_Materials(const _char* pModelFilePath);
	HRESULT Ready_Bones(aiNode* pAINode, _int iParentBoneIndex = -1);
	HRESULT Ready_Animations();

public:
	MODEL Get_ModelType() { return m_eModelType; }

	HRESULT Save_Model_File(); /* ImGui  용도의 세이브 기능 */

public:
	static CModel* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pModelFilePath, MODEL eModelType,_fmatrix PreTransformMatrix = XMMatrixIdentity());
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END