#include "pch.h"
#include "ImguiManager.h"
#include "Client_Defines.h"
#include "Imgui_File.h"
#include "Imgui_Model.h"
#include "Imgui_Camera.h"

IMPLEMENT_SINGLETON(CImguiManager)


CImguiManager::CImguiManager()
{
}

CImguiManager::~CImguiManager()
{
}

void CImguiManager::Init(ID3D11Device* pGraphicDev, ID3D11DeviceContext* pGraphicDeviceContext)
{
	{
		m_pDevice = pGraphicDev;	
		m_pContext = pGraphicDeviceContext;	
		m_pGameInstance = CGameInstance::GetInstance();

		Safe_AddRef(m_pDevice);	
		Safe_AddRef(m_pContext);	
		Safe_AddRef(m_pGameInstance);
	}
	
	IMGUI_CHECKVERSION();					
	ImGui::CreateContext();			
	ImGuiIO& io = ImGui::GetIO(); (void)io;			
	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls	
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();	
	
	ImGui_ImplWin32_Init(g_hWnd);	
	ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	/* ��Ʈ������ ���ٰ� �� */
	io.Fonts->AddFontFromFileTTF("../TTF/Pretendard-Regular.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesKorean());	

	ImGui::SetNextWindowSize(ImVec2(800, 800)); // Imgui â ũ��	
	ImGui::SetNextWindowPos(ImVec2(0, 0));	

	ZeroMemory(&m_RasterDesc, sizeof(D3D11_RASTERIZER_DESC));	


#pragma region Imgui_File ��ü ����
	m_pImguiFileMenu = CImgui_File::Create();
	m_pImguiFileMenu->Init();
#pragma endregion 


#pragma region Imgui_Model  ��ü ���� 
	m_pImguiModelMenu = CImgui_Model::Create();		
	m_pImguiModelMenu->Initialize();		
#pragma endregion 

#pragma region Imgui_Camera ��ü ���� 
	m_pImguiCameraMenu = CImgui_Camera::Create();		
	m_pImguiCameraMenu->Initialize();		
#pragma endregion 

}

void CImguiManager::Update(_float fTimeDelta)	
{
	// DX11 IMGUI update 

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	

	if (ImGui::Begin("Animation Tool Manager", NULL, ImGuiWindowFlags_MenuBar))
	{
		
		/* Imgui ���� �޴�*/
		m_pImguiFileMenu->Update();

		/* Imgui �ִϸ��̼� �� ���� �޴� */
		m_pImguiModelMenu->Update(fTimeDelta);
	
		/* Imgui ī�޶� �� ���� �޴� */
		m_pImguiCameraMenu->Update(fTimeDelta);	
		
	}

	ImGui::End(); // â ����	

}

void CImguiManager::Render()
{
	ImGui::Render();		
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	ImGui::EndFrame();		
}

void CImguiManager::Free()	
{

	ImGui_ImplWin32_Shutdown();	
	ImGui_ImplDX11_Shutdown();	

	ImGui::DestroyContext();
	
	Safe_Release(m_pImguiFileMenu);
	Safe_Release(m_pImguiModelMenu);	
	Safe_Release(m_pImguiCameraMenu);
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);	
	Safe_Release(m_pContext);	
	
}
