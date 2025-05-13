#include "Renderer.h"
#include "GameObject.h"
#include "GameInstance.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
	,m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CRenderer::Initialize()
{
	//D3D11_VIEWPORT ViewportDesc{};
	//_uint		   iNumViewports = 1; 
	//
	//m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);	

	/* Target_Diffuse*/
	//if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_Diffuse"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, _float4(0.f, 0.f, 1.f, 1.f))))
	//	return E_FAIL;	


	/* Target World */
	/*if (FAILED(m_pGameInstance->Add_RenderTarget(TEXT("Target_World"), ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, _float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;*/

	//if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Result"), TEXT("Target_Diffuse"))))	
	//	return E_FAIL;	

	/* ·»´õÅ¸°Ù 1°³ »©º¸±â*/
	/*if (FAILED(m_pGameInstance->Add_MRT(TEXT("MRT_Result"), TEXT("Target_World"))))
		return E_FAIL;*/


	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroupID, CGameObject* pGameObject)
{
	if (nullptr == pGameObject ||
		eRenderGroupID >= RG_END)
		return E_FAIL; 


	m_RenderObjects[eRenderGroupID].push_back(pGameObject);	
	
	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Render()
{
	if (FAILED(Render_DeferredResult()))	
		return E_FAIL;	

	if (FAILED(Render_Priority()))
		return E_FAIL; 

	if (FAILED(Render_NonBlend()))	
		return E_FAIL;

	if (FAILED(Render_Blend()))	
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	
	//if (FAILED(m_pGameInstance->Begin_MRT(TEXT("MRT_Result"), nullptr)))	
	//	return E_FAIL;	

	//D3D11_VIEWPORT pDesc{};
	//UINT numViewports = 1;
	//
	//m_pContext->RSGetViewports(&numViewports, &pDesc);


	//if (FAILED(m_pGameInstance->End_MRT()))
	//	return E_FAIL;
	 
	/*D3D11_VIEWPORT* pDesc2{};	
	UINT numViewports2 = 1;	

	m_pContext->RSGetViewports(&numViewports2, pDesc2);*/

	for(auto& pRenderObject : m_RenderObjects[RG_PRIORITY])
	{

		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_PRIORITY].clear();	


	return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
	for (auto& pRenderObject : m_RenderObjects[RG_NONBLEND])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL; 

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	for(auto& pRenderObejct : m_RenderObjects[RG_BLEND])
	{

		if (FAILED(pRenderObejct->Render()))
			return E_FAIL;

		Safe_Release(pRenderObejct);
	}

	m_RenderObjects[RG_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for(auto& pRenderObject : m_RenderObjects[RG_UI])
	{
		if (FAILED(pRenderObject->Render()))
			return E_FAIL;

		Safe_Release(pRenderObject);
	}

	m_RenderObjects[RG_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{
	return E_NOTIMPL;
}

HRESULT CRenderer::Render_DeferredResult()
{


	return S_OK;	
}

void CRenderer::Render_Debug()
{
	for(auto& pDebugCom : m_DebugComponents)
	{
		pDebugCom->Render();	
	}


}

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

	CRenderer* pInstance = new CRenderer(pDevice, pContext);

	if(FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;	

}

void CRenderer::Free()
{
	__super::Free();	


	for(auto& RenderObjects : m_RenderObjects)	
	{
		for (auto& pRenderObject : RenderObjects)
		{
			Safe_Release(pRenderObject);	
		}
		RenderObjects.clear();	
	}

	Safe_Release(m_pContext);		
	Safe_Release(m_pDevice);	
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pLightDepthStencilView);
}
