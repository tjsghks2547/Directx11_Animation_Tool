#include "..\Public\GameInstance.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"

#include "Prototype_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include "Calculator.h"

#include "Light_Manager.h"
#include "Target_Manager.h"
#include "Font_Manager.h"



IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(HINSTANCE hInst,const ENGINE_DESC & EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext)
{
	m_iViewportWidth  = EngineDesc.iViewportWidth;
	m_iViewportHeight = EngineDesc.iViewportHeight;


	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iViewportWidth, EngineDesc.iViewportHeight, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL; 

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pInput_Device = CInput_Device::Create(hInst,EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
		return E_FAIL;

	m_pCalculator = CCalculator::Create(*ppDevice, *ppContext, EngineDesc.hWnd);
	if (nullptr == m_pCalculator)
		return E_FAIL;

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)	
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	/* �� ���ӳ��� �ʿ���  �ټ� ��ü�� �����۾��� ��� ��Ƽ� ������ �ҰŴ�. */

	/*�ٷ� ��ǲ�� ����Ƽ�� */

	if (nullptr == m_pLevel_Manager)
		return;

	m_pInput_Device->Update_InputDev();

	m_pLevel_Manager->Update(fTimeDelta);


	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);

	m_pPipeLine->Update();		
	m_pCalculator->Store_MouseRay(m_pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_PROJ)
		, m_pPipeLine->Get_Transform_Matrix_Inverse(CPipeLine::D3DTS_VIEW));	

	//m_pCalculator->Compute_PickingPoint();

	m_pObject_Manager->Late_Update(fTimeDelta);	

}

HRESULT CGameInstance::Render_Begin(const _float4& vClearColor)
{
	m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
	m_pGraphic_Device->Clear_DepthStencil_View();

	return S_OK;
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Render();	

	m_pLevel_Manager->Render();

	return S_OK;
}

HRESULT CGameInstance::Render_End()
{
	m_pGraphic_Device->Present();

	return E_NOTIMPL;
}

void CGameInstance::Clear(_uint iLevelIndex)
{
	/* ������ ������ ������ü�� Ŭ�����Ѵ�.*/
	m_pPrototype_Manager->Clear(iLevelIndex);

	/* ������ ������ �纻��ü�� ��� �ִ� ���̾���� Ŭ�����Ѵ�. */
	m_pObject_Manager->Clear(iLevelIndex);
}

ID3D11RenderTargetView* CGameInstance::Get_RTV_Textrue()
{
	return m_pGraphic_Device->Get_RTV_Textrue();
}

#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Open_Level(_uint iNewLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Open_Level(iNewLevelIndex, pNewLevel);
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER
HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{

	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}
CBase* CGameInstance::Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(eType, iLevelIndex, strPrototypeTag, pArg);
}

map<const _wstring, CBase*>* CGameInstance::Get_Prototypes_Map()
{
	return m_pPrototype_Manager->Get_Prototypes_Map();
}

#pragma endregion

#pragma region OBJECT_MANAGER
HRESULT CGameInstance::Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject_To_Layer(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

HRESULT CGameInstance::Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, const _char* ObjectName)
{
	return m_pObject_Manager->Sub_GameObject_To_Layer(iLevelIndex, strLayerTag, ObjectName);		
}

CComponent* CGameInstance::Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName)
{
	/* �̰� ����� �� �����������δ� �ش� ������ ȭ�鿡 �־�� �ϰ� �̰� ���纻�� ã�ƿ��°���.*/
	return m_pObject_Manager->Find_Component(iLevelIndex, strLayerTag, strComponentName);
}

map<const _wstring, class CLayer*>* CGameInstance::Get_Layers()
{
	return m_pObject_Manager->Get_Layers();	
}

_uint CGameInstance::Get_NumLevel()
{
	return m_pObject_Manager->Get_NumLevel();
}

list<class CGameObject*>* CGameInstance::Get_Layer_List(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_Layer_List(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Get_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _char* ObjectName)
{
	return m_pObject_Manager->Get_GameObject_To_Layer(iLevelIndex, strLayerTag, ObjectName);	
}


HRESULT CGameInstance::Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroupID, CGameObject* pGameObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroupID, pGameObject);
}

const _float4x4* CGameInstance::Get_Transform_float4x4(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4x4* CGameInstance::Get_Transform_float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_float4x4_Inverse(eState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix_Inverse(eState);
}

const _float4* CGameInstance::Get_CamPosition_float4()
{
	return m_pPipeLine->Get_CamPosition_float4();
}

_vector CGameInstance::Get_CamPosition()
{
	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix)
{
	m_pPipeLine->Set_Transform(eState, TransformMatrix);
}

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

_bool CGameInstance::isKeyEnter(_int _iKey)
{
	return m_pInput_Device->isKeyEnter(_iKey);
}

_bool CGameInstance::isKeyPressed(_int _iKey)
{
	return m_pInput_Device->isKeyPressed(_iKey);
}

_bool CGameInstance::isKeyReleased(_int _iKey)
{
	return m_pInput_Device->isKeyReleased(_iKey);
}

_bool CGameInstance::isMouseEnter(_uint _iKey)
{
	return m_pInput_Device->isMouseEnter(_iKey);
}

_bool CGameInstance::isMouseRelease()
{
	return m_pInput_Device->isMouseRelease();	
}


_vector CGameInstance::Get_RayPos()
{
	return m_pCalculator->Get_RayPos();	
}

_vector CGameInstance::Get_RayDir()
{
	return m_pCalculator->Get_RayDir();	
}

void CGameInstance::Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, CTransform* pTransform)
{
	return m_pCalculator->Compute_LocalRayInfo(pRayDir, pRayPos, pTransform);	
}

void CGameInstance::Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, XMMATRIX WorldInvers)
{
	return m_pCalculator->Compute_LocalRayInfo(pRayDir, pRayPos, WorldInvers);	
}

POINT CGameInstance::Get_Mouse_Point()
{
	return m_pCalculator->Get_Mouse_Point();	
}

_float3 CGameInstance::Get_WorldRayPos()
{
	return m_pCalculator->Get_WorldRayPos();		
}

_float3 CGameInstance::Get_WorldRayDir()
{
	return m_pCalculator->Get_WorldRayDir();		
}

_float3 CGameInstance::Compute_PickingPoint(_float3 _point1, _float3 _point2, _float3 _point3)
{
	return m_pCalculator->Compute_PickingPoint(_point1, _point2, _point3);
}

const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iIndex);
}
HRESULT CGameInstance::Add_Light(const LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}
HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render(pShader,pVIBuffer);
}
HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
HRESULT CGameInstance::Render_Font(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRotation, const _float2& vOrigin, const _float fScale)
{
	return m_pFont_Manager->Render(strFontTag, pText, vPosition, vColor, fRotation, vOrigin, fScale);
}
HRESULT CGameInstance::Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iSizeX, iSizeY, ePixelFormat, vClearColor);
}
HRESULT CGameInstance::Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}
HRESULT CGameInstance::Begin_MRT(const wstring& strMRTTag, _bool isClear)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, isClear);
}
HRESULT CGameInstance::Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSView)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, pDSView);
}
HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}
HRESULT CGameInstance::Bind_RenderTargetSRV(const wstring& strTargetTag, CShader* pShader, const _char* pConstantName)
{
	return m_pTarget_Manager->Bind_RenderTargetSRV(strTargetTag, pShader, pConstantName);
}
HRESULT CGameInstance::Copy_Resource(const wstring& strTargetTag, ID3D11Texture2D* pDesc)
{
	return m_pTarget_Manager->Copy_Resource(strTargetTag, pDesc);
}
#ifdef _DEBUG
HRESULT CGameInstance::Ready_RTDebug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}
HRESULT CGameInstance::Render_RTDebug(const wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render_Debug(strMRTTag, pShader, pVIBuffer);
}
#endif

void CGameInstance::Release_Engine()
{
	Safe_Release(m_pGraphic_Device);	
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);


	Safe_Release(m_pRenderer);	
	Safe_Release(m_pObject_Manager);	
	Safe_Release(m_pPrototype_Manager);	
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pCalculator);
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pTarget_Manager);
	
	

	CGameInstance::GetInstance()->DestroyInstance();
}

void CGameInstance::Free()
{
	__super::Free();


}
