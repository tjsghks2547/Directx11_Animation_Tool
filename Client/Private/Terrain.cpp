#include "pch.h"
#include "Terrain.h"
#include "GameInstance.h"
#include "ImguiManager.h"


using namespace TriangleTests;

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CGameObject(pDevice,pContext)
{
}

CTerrain::CTerrain(const CTerrain& Prototype)
	:CGameObject(Prototype)
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	if (FAILED(__super::Initialize_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL; 

	if (FAILED(Ready_Component()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ -25.f,0.f,-25.f,1.f });

	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	int  a = 10;
}

void CTerrain::Update(_float fTimeDelta)
{
	///* 이걸 imgui에서 하는방안으로 가자 */
	////if(m_btest==true)		
	////	{
	//	CComponent* pComponent = m_pGameInstance->Find_Component(LEVEL_GAMEPLAY, TEXT("Layer_Terrain"), TEXT("Terrain_Com"));
	//
	//	pTerrain = dynamic_cast<CVIBuffer_Terrain*>(pComponent);
	//
	//
	//	D3D11_MAPPED_SUBRESOURCE mapped_VB_Resource;
	//	ZeroMemory(&mapped_VB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//
	//	D3D11_MAPPED_SUBRESOURCE mapped_IB_Resource;
	//	ZeroMemory(&mapped_IB_Resource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	//
	//	m_iTotalTriangle = (pTerrain->Get_Width_X()-1) * (pTerrain->Get_Height_Z()-1) * 2;
	//	
	//	
	//	HRESULT hr = m_pContext->Map(pTerrain->Get_VB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_VB_Resource);
	//
	//	HRESULT hr2 = m_pContext->Map(pTerrain->Get_IB_Buffer(), 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_IB_Resource);
	//
	//	//정점버퍼
	//	pVertices = reinterpret_cast<VTXNORTEX*>(mapped_VB_Resource.pData);
	//		
	//	//인덱스버퍼
	//	pIndices = reinterpret_cast<_uint*>(mapped_IB_Resource.pData);
	//
	//
	//	//m_pContext->Unmap(pTerrain->Get_VB_Buffer(), 0);
	//	//m_pContext->Unmap(pTerrain->Get_IB_Buffer(), 0);
	//
	//	m_btest = false;
	//// }
	//
	//
	////if ((m_pGameInstance->isKeyDown(VK_LBUTTON) && CImguiManager::GetInstance()->Get_TerrainPicking_bool())
	////	|| ((m_pGameInstance->isKeyDown(VK_LBUTTON))&& CImguiManager::GetInstance()->Get_Object_Picking_bool()))
	//if((m_pGameInstance->isKeyDown(VK_LBUTTON)&& CImguiManager::GetInstance()->Get_Object_Picking_bool()))
	//{
	//	//CImguiManager::GetInstance()->Set_MousePicking(true);
	//
	//	_float3 mouseRayPos = m_pGameInstance->Get_WorldRayPos();
	//	_float3 mouseRayDir = m_pGameInstance->Get_WorldRayDir();
	//
	//	FXMVECTOR mousePos = { mouseRayPos.x,mouseRayPos.y,mouseRayPos.z,1.f };
	//	FXMVECTOR mouseRay = { mouseRayDir.x,mouseRayDir.y,mouseRayDir.z,0.f };
	//
	//
	//	float dist = 0.f;
	//
	//	_uint index = { 0 };
	//
	//
	//	_uint half = m_iTotalTriangle / 2;
	//
	//
	//	for (size_t i = 0; i < m_iTotalTriangle; i++)
	//	{
	//		bool PickingOnOff = Intersects(mousePos, mouseRay,
	//			XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
	//			XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
	//			XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
	//			dist);
	//
	//
	//		if (PickingOnOff == true)
	//		{
	//			CImguiManager::GetInstance()->Set_MousePicking(true);
	//			/* 세게의 점 저장하기*/
	//			//CImguiManager::GetInstance()->Set_PickingPoint(
	//			//	pVertices[pIndices[index - 3]].vPosition,
	//			//	pVertices[pIndices[index - 2]].vPosition,
	//			//	pVertices[pIndices[index - 1]].vPosition);
	//
	//			_float3 intersection = m_pGameInstance->Compute_PickingPoint(
	//				pVertices[pIndices[index - 3]].vPosition,
	//				pVertices[pIndices[index - 2]].vPosition,
	//				pVertices[pIndices[index - 1]].vPosition);
	//
	//			CImguiManager::GetInstance()->Set_Object_Picking_Point(intersection.x, intersection.y, intersection.z);				
	//
	//			if (CImguiManager::GetInstance()->Get_TerrainPicking_bool())
	//			{
	//				/* 피킹으로 지형 높이기 */
	//				pVertices[pIndices[index - 3]].vPosition.y += 1.f * fTimeDelta;
	//				pVertices[pIndices[index - 2]].vPosition.y += 1.f * fTimeDelta;
	//				pVertices[pIndices[index - 1]].vPosition.y += 1.f * fTimeDelta;
	//			}
	//
	//
	//			break;
	//		}
	//	}
	//
	//	
	//}
	//
	//if(GetAsyncKeyState(VK_LBUTTON)&0x8000 && CImguiManager::GetInstance()->Get_TerrainPicking_bool())
	//{
	//	_float3 mouseRayPos = m_pGameInstance->Get_WorldRayPos();	
	//	_float3 mouseRayDir = m_pGameInstance->Get_WorldRayDir();	
	//
	//	FXMVECTOR mousePos = { mouseRayPos.x,mouseRayPos.y,mouseRayPos.z,1.f };	
	//	FXMVECTOR mouseRay = { mouseRayDir.x,mouseRayDir.y,mouseRayDir.z,0.f };	
	//
	//
	//	float dist = 0.f;
	//	_uint index = { 0 };	
	//
	//	for (size_t i = 0; i < m_iTotalTriangle; i++)
	//	{
	//		bool PickingOnOff = Intersects(mousePos, mouseRay,
	//			XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
	//			XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
	//			XMVectorSetW(XMLoadFloat3(&pVertices[pIndices[index++]].vPosition), 1.f),
	//			dist);
	//
	//
	//		if (PickingOnOff == true)
	//		{
	//			if (CImguiManager::GetInstance()->Get_TerrainPicking_bool())
	//			{
	//				/* 피킹으로 지형 높이기 */
	//				pVertices[pIndices[index - 3]].vPosition.y += 1.f * fTimeDelta;
	//				pVertices[pIndices[index - 2]].vPosition.y += 1.f * fTimeDelta;
	//				pVertices[pIndices[index - 1]].vPosition.y += 1.f * fTimeDelta;
	//			}
	//			break;
	//		}
	//	}
	//}
	//
	//
	////if (GetKeyState('M') & 0x8000)
	////{
	//	m_pContext->Unmap(pTerrain->Get_VB_Buffer(), 0);
	//	m_pContext->Unmap(pTerrain->Get_IB_Buffer(), 0);
	////}
	//
	//int a = 4;  /* 수정완료 viewport 변환 에서 잘못됬었음 */

	
	

}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(CRenderer::RG_PRIORITY, this);
}

HRESULT CTerrain::Render()
{

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Bind_InputAssembler();
 	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CTerrain::Ready_Component()
{
	/* 버퍼 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, TEXT("Terrain_Com"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* 셰이더 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Shader")
		, TEXT("Shader_Com"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
		return E_FAIL;

	/* 텍스처 */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture")
		, TEXT("Texture_Com"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	

	return S_OK;
}

HRESULT CTerrain::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_ShaderResource(m_pShaderCom, "g_WorldMatrix")))	
		return E_FAIL;	

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;	
	
	if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

#pragma region 빛연산 
	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;


#pragma endregion



	return S_OK;
}

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	/* 여기서 부터 다시 작성*/
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if(FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);
	
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();
	
	/*11월 15일 임시*/
	Safe_Release(m_pTexture2D);
	////////////////////////

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
