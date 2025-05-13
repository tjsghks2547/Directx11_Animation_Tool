#include "VIBuffer_Rect_Texcood2.h"

CVIBuffer_Rect_Texcood2::CVIBuffer_Rect_Texcood2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Rect_Texcood2::CVIBuffer_Rect_Texcood2(const CVIBuffer_Rect_Texcood2& Prototype)
    :CVIBuffer(Prototype)
{
}

HRESULT CVIBuffer_Rect_Texcood2::Initialize_Prototype()
{
    m_iNumVertices = 4;
    m_iVertexStride = sizeof(VTXPOSTEX2);
    m_iNumIndices = 6;
    m_iIndexStride = 2;
    m_iNumVertexBuffers = 1;
    m_eIndexFormat = DXGI_FORMAT_R16_UINT;

#pragma region VERTEX_BUFFER
    ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
    m_BufferDesc.ByteWidth = m_iNumVertices * m_iVertexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iVertexStride;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;


    VTXPOSTEX2* pVertices = new VTXPOSTEX2[m_iNumVertices];
    ZeroMemory(pVertices, sizeof(VTXPOSTEX2) * m_iNumVertices);


    pVertices[0].vPosition  = _float3(-0.5f, 0.5f, 0.f);
    pVertices[0].vTexcoord  = _float2(0.f, 0.f);
    pVertices[0].vTexcoord2 = _float2(0.f, 0.f);

    pVertices[1].vPosition  = _float3(0.5f, 0.5f, 0.f);
    pVertices[1].vTexcoord  = _float2(1.f, 0.f);
    pVertices[1].vTexcoord2 = _float2(1.f, 0.f);

    pVertices[2].vPosition  = _float3(0.5f, -0.5f, 0.f);
    pVertices[2].vTexcoord  = _float2(1.f, 1.f);
    pVertices[2].vTexcoord2 = _float2(1.f, 1.f);

    pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
    pVertices[3].vTexcoord = _float2(0.f, 1.f);
    pVertices[3].vTexcoord2 = _float2(0.f, 1.f);

    ZeroMemory(&m_InitialData, sizeof(m_InitialData));
    m_InitialData.pSysMem = pVertices;

    if (FAILED(__super::Create_Buffer(&m_pVB)))
        return E_FAIL;


    Safe_Delete_Array(pVertices);



#pragma endregion

#pragma region INDEX_BUFFER
    ZeroMemory(&m_BufferDesc, sizeof(m_BufferDesc));
    m_BufferDesc.ByteWidth = m_iNumIndices * m_iIndexStride;
    m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
    m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    m_BufferDesc.StructureByteStride = m_iIndexStride;
    m_BufferDesc.CPUAccessFlags = 0;
    m_BufferDesc.MiscFlags = 0;
    m_ePrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


    _ushort* pIndices = new _ushort[m_iNumIndices];
    ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

    pIndices[0] = 0;
    pIndices[1] = 1;
    pIndices[2] = 2;

    pIndices[3] = 0;
    pIndices[4] = 2;
    pIndices[5] = 3;

    ZeroMemory(&m_InitialData, sizeof(m_InitialData));
    m_InitialData.pSysMem = pIndices;

    if (FAILED(__super::Create_Buffer(&m_pIB)))
        return E_FAIL;

    Safe_Delete_Array(pIndices);

#pragma endregion

    return S_OK;
}

HRESULT CVIBuffer_Rect_Texcood2::Initialize(void* pArg)
{
    return S_OK;
}

CVIBuffer_Rect_Texcood2* CVIBuffer_Rect_Texcood2::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CVIBuffer_Rect_Texcood2* pInstance = new CVIBuffer_Rect_Texcood2(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CVIBuffer_Rect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CVIBuffer_Rect_Texcood2::Clone(void* pArg)
{
    CVIBuffer_Rect_Texcood2* pInstance = new CVIBuffer_Rect_Texcood2(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CVIBuffer_Rect");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CVIBuffer_Rect_Texcood2::Free()
{
    __super::Free();
}
