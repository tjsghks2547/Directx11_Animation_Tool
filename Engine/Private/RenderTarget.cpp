#include "RenderTarget.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"

CRenderTarget::CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
{
	Safe_AddRef(pDevice);	
	Safe_AddRef(pContext);	
}

HRESULT CRenderTarget::Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	D3D11_TEXTURE2D_DESC TextureDesc{};

	TextureDesc.Width = iSizeX;
	TextureDesc.Height = iSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1; 
	TextureDesc.Format = ePixelFormat;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1; 

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	TextureDesc.CPUAccessFlags = 0; 
	TextureDesc.MiscFlags = 0; 

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(m_pTexture2D, nullptr, &m_pRTV)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateShaderResourceView(m_pTexture2D, nullptr, &m_pSRV)))
		return E_FAIL;
	
	m_vClearColor = vClearColor;

	return S_OK;	
}

HRESULT CRenderTarget::Clear()
{
	m_pContext->ClearRenderTargetView(m_pRTV, (_float*)&m_vClearColor);
	return S_OK;
}

HRESULT CRenderTarget::Bind_SRV(CShader* pShader, const _char* pConstantName)
{
	return pShader->Bind_SRV(pConstantName,m_pSRV);	
}

HRESULT CRenderTarget::Copy_Resource(ID3D11Texture2D* pDesc)
{
	m_pContext->CopyResource(pDesc, m_pTexture2D);

	return S_OK;
}

#ifdef _DEBUG
HRESULT CRenderTarget::Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY)
{
	D3D11_VIEWPORT		ViewportDesc{};
	_uint				iNumViewports = 1; 

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());

	m_WorldMatrix._11 = fSizeX;
	m_WorldMatrix._22 = fSizeY;

	m_WorldMatrix._41 = fX - ViewportDesc.Width  * 0.5f;
	m_WorldMatrix._42 = fY - ViewportDesc.Height * 0.5f;

	return S_OK;
}
HRESULT CRenderTarget::Render_Debug(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{

	pShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix);

	pShader->Bind_SRV("g_Texture", m_pSRV);

	pShader->Begin(0);

	pVIBuffer->Bind_InputAssembler();

	pVIBuffer->Render();

	return S_OK;
}
#endif // _DEBUG
CRenderTarget* CRenderTarget::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	CRenderTarget* pInstance = new CRenderTarget(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iSizeX, iSizeY, ePixelFormat, vClearColor)))
	{
		MSG_BOX("Failed to Created : RenderTarget");
		Safe_Release(pInstance);
	};

	return pInstance;	
}

void CRenderTarget::Free()
{
	__super::Free();
	
	Safe_Release(m_pSRV);
	Safe_Release(m_pRTV);	
	Safe_Release(m_pTexture2D);	
	
	Safe_Release(m_pDevice);	
	Safe_Release(m_pContext);	

}
