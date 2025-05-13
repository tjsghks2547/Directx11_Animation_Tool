#include "Picking.h"
#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
	,m_pGameInstance(CGameInstance::GetInstance())
	
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

_bool CPicking::Get_PickPos(_float4* pPickPos)
{
	*pPickPos = m_vPickPos;

	return m_isSuccess;
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	_uint		iNumViews = { 1 };

	D3D11_VIEWPORT		ViewportDesc{};

	m_pContext->RSGetViewports(&iNumViews, &ViewportDesc);

	m_iWinSizeX = ViewportDesc.Width;
	m_iWinSizeY = ViewportDesc.Height;

	D3D11_TEXTURE2D_DESC		TextureDesc{};

	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_iWinSizeX;
	TextureDesc.Height = m_iWinSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_STAGING;
	TextureDesc.BindFlags = 0;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	return S_OK;

	
}

void CPicking::Update()
{
	m_isSuccess = true;

	POINT		ptMouse = m_pGameInstance->Get_Mouse_Point();



	if (ptMouse.x < 0)
		ptMouse.x = 0;
	if (ptMouse.x >= m_iWinSizeX)
		ptMouse.x = m_iWinSizeX;
	if (ptMouse.y < 0)
		ptMouse.y = 0;
	if (ptMouse.y >= m_iWinSizeY)
		ptMouse.y = m_iWinSizeY;

	_vector		vMousePos = XMVectorZero();

	/* ������������ ��ǥ��. = ������ġ * ������� * ����� * ������� / w */
	vMousePos = XMVectorSetX(vMousePos, ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	vMousePos = XMVectorSetY(vMousePos, ptMouse.y / -(m_iWinSizeY * 0.5f) + 1.f);
	vMousePos = XMVectorSetZ(vMousePos, Compute_ProjZ(ptMouse.x - 1, ptMouse.y - 1));
	vMousePos = XMVectorSetW(vMousePos, 1.f);

	if (XMVectorGetZ(vMousePos) < 0.0f)
		m_isSuccess = false;

	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ));
	vMousePos = XMVector3TransformCoord(vMousePos, m_pGameInstance->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW));

	XMStoreFloat4(&m_vPickPos, vMousePos);

}


_float CPicking::Compute_ProjZ(_float fX, _float fY)
{
	if (fX < 0.f)
		fX = 0.f;
	if (fY < 0.f)
		fY = 0.f;

	m_pGameInstance->Copy_Resource(TEXT("Target_Depth"), m_pTexture2D);

	if (nullptr == m_pTexture2D)
		return 0.f;

	D3D11_MAPPED_SUBRESOURCE		SubResources{};

	m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResources);

	/* �� ���콺 ��ǥ�� �����ϴ� ��ġ�� �ִ� �ؽ��� �ȼ��� �ε��� */
	_uint		iIndex = fY * (m_iWinSizeX)+fX;

	_float4		vResult = ((_float4*)SubResources.pData)[iIndex];

	m_pContext->Unmap(m_pTexture2D, 0);


	if (0.0f == vResult.w)
		return -1.f;

	return vResult.x;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CPicking* pInstance = new CPicking(pDevice, pContext,hWnd);

	if(FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed to Created : Picking");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

void CPicking::Free()
{
	__super::Free();

	Safe_Release(m_pTexture2D);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
