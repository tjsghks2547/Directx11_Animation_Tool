#pragma once

#include "VIBuffer.h"
#include "Model.h"

BEGIN(Engine)

class CMesh final : public CVIBuffer
{
private:
	CMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMesh(const CMesh& Prototype);
	virtual ~CMesh() = default;

public:
	_uint Get_MaterialIndex() const {
		return m_iMaterialIndex;	
	}

public:
	virtual HRESULT Initialize_Prototype(CModel::MODEL eModelType, CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_BoneMatrices(class CShader* pShdaer, const _char* pConstantName, const vector<class CBone*>& Bones);


private:
	_char			m_szName[MAX_PATH] = {};
	_uint			m_iMaterialIndex = {};	
	/* �� �޽ÿ� ������ �ִ� ���� ���� */
	_uint			m_iNumBones = {};
	/* �� �޽ÿ� ������ �ִ� ���� �ε���*/
	/* ���� ������ �ִ� ��ü �� ������ �ε���( �� ���� m_Bones�� ����ִ� ������ ���°�� ����ִ����� �����Ѵٴ� �ǹ�)*/
	vector<_uint>   m_BoneIndices;
		
	/* ���� ����� ī�� ������ �Ž� �ϳ��� ���� 273���� 300���� ���� */
	_float4x4		      m_BoneMatrices[700] = {};
	vector<_float4x4>	  m_OffsetMatrices;

	/* ���̳ʸ�ȭ ���� �ɹ� ����*/	
	D3D11_BUFFER_DESC	  m_MeshBufferdesc = {};
	VTXMESH*			  m_nonAnimpVertices;
	VTXANIMMESH*		  m_AnimVertices;
	CModel::MODEL         m_ModelType; 
	_uint*				  pNewIndices;
private:
	HRESULT Ready_VertexBuffer_ForNonAnim(const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	HRESULT Ready_VertexBuffer_ForAnim(CModel* pModel, const aiMesh* pAIMesh);	


public:
	HRESULT Save_Mesh(ostream& os);	
	HRESULT Load_Mesh(istream& is);	


public:
	static CMesh* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CModel::MODEL eModelType, CModel* pModel, const aiMesh* pAIMesh, _fmatrix PreTransformMatrix);
	static CMesh* LoadCreate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, istream& is);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END