#include "VIBuffer.h"

CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CComponent(pDevice,pContext)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& Prototype)
	:CComponent(Prototype)
	, m_pVB(Prototype.m_pVB)
	, m_pIB(Prototype.m_pIB)
	, m_BufferDesc(Prototype.m_BufferDesc)
	, m_InitialData(Prototype.m_InitialData)
	, m_ePrimitiveTopology(Prototype.m_ePrimitiveTopology)
	, m_eIndexFormat(Prototype.m_eIndexFormat)
	, m_iNumVertexBuffers(Prototype.m_iNumVertexBuffers)	
	, m_iVertexStride(Prototype.m_iVertexStride)	
	, m_iNumVertices(Prototype.m_iNumVertices)	
	, m_iIndexStride(Prototype.m_iIndexStride)	
	, m_iNumIndices(Prototype.m_iNumIndices)	
	, m_iNumPrimitive(Prototype.m_iNumPrimitive)	
	, m_pVertexPositions(Prototype.m_pVertexPositions)	
	, m_iCntX(Prototype.m_iCntX)
	, m_iCntY(Prototype.m_iCntY)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
}



HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Bind_InputAssembler()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] =
	{
		m_pVB,
	};

	/*ID3D11Buffer* const* ppVertexBuffers;*/
	// ID3D11Buffer* const* ppVertexBuffers;-> 왜 이렇게 선언해줬는지에 대해서 
	// 알아보기. 
	_uint		iVertexStrides[] =
	{
		m_iVertexStride,
	};

	_uint       iOffsets[] =
	{
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iVertexStrides, iOffsets);	
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);	
	/* 입력 레이아웃 설정 다시 한번 확인해보기(여기서 오류가 발생한다면) */ 
	m_pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);	

	
	return S_OK;
}

HRESULT CVIBuffer::Create_Buffer(ID3D11Buffer** ppVIBuffer)
{
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppVIBuffer)))
	{
		return E_FAIL;
	}

	return S_OK; 
}

HRESULT CVIBuffer::Create_BufferZ(ID3D11Buffer** ppVIBuffer, D3D11_BUFFER_DESC* meshbuffer)
{
	if (FAILED(m_pDevice->CreateBuffer(meshbuffer, &m_InitialData, ppVIBuffer)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVertexPositions);

	Safe_Release(m_pIB);
	Safe_Release(m_pVB);
}
