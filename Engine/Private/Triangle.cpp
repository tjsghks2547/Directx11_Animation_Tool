#include "Triangle.h"

CTriangle::CTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice,pContext)
{

}

CTriangle::CTriangle(const CTriangle& Prototype)
	:CVIBuffer(Prototype)
{
	 
}


HRESULT CTriangle::Initialize_Prototype()
{

	m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;	
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_iNumVertexBuffers = 1;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = 3;

	m_iIndexStride = 2;
	m_iNumIndices = 3;

#pragma region VERTEX_BUFFER
	
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;	
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0; 
	m_BufferDesc.MiscFlags = 0; 
	m_BufferDesc.StructureByteStride = m_iVertexStride;	

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];	
	ZeroMemory(pVertices, sizeof(VTXPOSTEX)* m_iNumVertices);	
		
	pVertices[0].vPosition = _float3{ 0.0f, 0.5f, 0.0f };		
	pVertices[0].vTexcoord = _float2{ 0.f, 0.f };

	pVertices[1].vPosition = _float3{ -0.5f, -0.5f, 0.0f };
	pVertices[1].vTexcoord = _float2{ 0.f, 0.f };	

	pVertices[2].vPosition = _float3{ 0.5f, -0.5f, 0.0f };
	pVertices[2].vTexcoord = _float2{ 0.f,0.f };	

	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL; 

	
	Safe_Delete_Array(pVertices);	
#pragma endregion

#pragma region INDEX_BUFFER
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0; 
	m_BufferDesc.StructureByteStride = 0; 

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	pIndices[0] = 0; 
	pIndices[1] = 1; 
	pIndices[2] = 2; 

	m_InitialData.pSysMem = pIndices; 

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);


#pragma endregion

	return S_OK;
}

HRESULT CTriangle::Initialize(void* pArg)
{
	return S_OK;	
}


CTriangle* CTriangle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTriangle* pInstance = new CTriangle(pDevice, pContext);
	
	if(FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed Create CTriangle");
		Safe_Release(pInstance); 
	}

	return pInstance;		
}

CComponent* CTriangle::Clone(void* pArg)
{
	CTriangle* pInstance = new CTriangle(*this);

	if(FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed Cloned CTriangle");
		Safe_Release(pInstance);
	}

	return pInstance; 
}

void CTriangle::Free()
{
	__super::Free();	

}
