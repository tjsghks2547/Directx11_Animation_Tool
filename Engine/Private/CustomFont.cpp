#include "CustomFont.h"

CCustomFont::CCustomFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :m_pDevice(pDevice)
    ,m_pContext(pContext)
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CCustomFont::Initialize(const _tchar* pFontFilePath)
{
    m_pFont  = new SpriteFont(m_pDevice, pFontFilePath);
    m_pBatch = new SpriteBatch(m_pContext); 

    return S_OK;
}

HRESULT CCustomFont::Render(const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, const _float fScale)
{
    if (nullptr == m_pFont ||
        nullptr == m_pBatch)
        return E_FAIL; 

    m_pBatch->Begin();

    /* 뷰포트 상의 직교투영의 형태로 그려낸다. */
    m_pFont->DrawString(m_pBatch, pText, vPosition, vColor, fRotation, vOrigin, fScale);

    m_pBatch->End();

    return S_OK;    
}

CCustomFont* CCustomFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
    CCustomFont* pInstance = new CCustomFont(pDevice, pContext);

    if(FAILED(pInstance->Initialize(pFontFilePath)))
    {
        MSG_BOX("Failed to Created : CustomFont");
        Safe_Release(pInstance);
    }
    
    return pInstance;
}

void CCustomFont::Free()
{
    __super::Free();


    Safe_Delete(m_pBatch);
    Safe_Delete(m_pFont);

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
    

}
