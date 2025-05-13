#include "pch.h"
#include "..\Public\MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ImguiManager.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	ENGINE_DESC		EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.iNumLevels = LEVEL_END;
	EngineDesc.isWindowed = true;
	EngineDesc.iViewportWidth = g_iWinSizeX;
	EngineDesc.iViewportHeight = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(g_hInst,EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	
	/* Imgui init 부분 ( 초기화 단계 ) */
	CImguiManager::GetInstance()->Init(m_pDevice, m_pContext);


	if (FAILED(Start_Level(LEVEL_LOGO)))
		return E_FAIL;



	/* 폰트테스트*/
	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Gulim_Default"), TEXT("../Bin/Resources/Fonts/148ex.spritefont"))))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	/* Imgui 카메라 뷰 업데이트대매 이거 먼저해야함.*/
	//CImguiManager::GetInstance()->Update();	

	CImguiManager::GetInstance()->Update(fTimeDelta);	

	m_pGameInstance->Update_Engine(fTimeDelta);	



#ifdef _DEBUG
	m_fTimerAcc += fTimeDelta;
#endif


}

HRESULT CMainApp::Render()
{
	

	if (FAILED(m_pGameInstance->Render_Begin(_float4(0.f, 0.f, 1.f, 1.f))))
		return E_FAIL;


	m_pGameInstance->Draw();

	CImguiManager::GetInstance()->Render();

#ifdef _DEBUG
	++m_iDrawCount;
	
	if (1.f <= m_fTimerAcc)
	{
		wsprintf(m_szFPS, TEXT("fps : %d"), m_iDrawCount);
		m_fTimerAcc = 0.f;
		m_iDrawCount = 0;
	}


	m_pGameInstance->Render_Font(TEXT("Font_Gulim_Default"), m_szFPS, _float2(0.f, 0.f));

	
#endif

	m_pGameInstance->Render_End();

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVELID eStartLevelID)
{
	if (LEVEL_LOADING == eStartLevelID)
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eStartLevelID))))
		return E_FAIL;

	return S_OK;
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{
	/* 부모의 멤버를 정리하라고 이야기한다. */
	__super::Free();

	
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	m_pGameInstance->Release_Engine();
	
	/*내 멤버변수를 정리한다.*/
	Safe_Release(m_pGameInstance);
}

