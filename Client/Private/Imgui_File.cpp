#include "pch.h"
#include "Imgui_File.h"
#include "Client_Defines.h"
#include "ImguiManager.h"
#include <Commdlg.h>
#include "Layer.h"
#include "GameObject.h"
#include "Navigation.h"
#include "Cell.h"



CImgui_File::CImgui_File()
	:m_pGameInstance(CGameInstance::GetInstance())
	,m_pDevice(CImguiManager::GetInstance()->Get_Device())
	,m_pContext(CImguiManager::GetInstance()->Get_DeviceContext())
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImgui_File::Init()
{
	m_pLayer = m_pGameInstance->Get_Layers();	

	return S_OK;
}

HRESULT CImgui_File::Update()	
{
	if (ImGui::BeginMenuBar())	
	{
		if (ImGui::BeginMenu("File"))	
		{		
			if (ImGui::MenuItem("Model Open", nullptr, m_bMapOpenMenu))	
			{
				ModelFile_Read();		
			}	

			if (ImGui::MenuItem("Model Save", nullptr, m_bMapOpenMenu))		
			{
				ModelFile_Save();		
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	return S_OK;
}

HRESULT CImgui_File::ModelFile_Read()
{

	OPENFILENAME ofn;       // OPENFILENAME 구조체 선언	
	wchar_t szFile[MAX_PATH] = {};   // 파일 이름을 저장할 버퍼 (최대 260자)	

	// 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // 부모 윈도우 핸들	
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // 초기 파일 이름 (빈 문자열)	
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";// 파일 형식 필터	
	ofn.nFilterIndex = 1;      // 기본 선택 파일 형식
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // 초기 디렉토리
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetOpenFileName(&ofn))
	{
		_tchar szEXT[MAX_PATH] = {};

		_ulong dwByte = {};
		/* bin 파일일시 아닐시 조건문 걸기 */
		_wsplitpath_s(szFile, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);


		HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		/* 이제 하나하나 정보를 빼오자*/

		while (true)
		{
			_tchar Layer_Name[MAX_PATH] = {}; //레이어 이름										
			ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

			if (dwByte == 0)
				break;
			/* 이름 작업 */
			_char   Prototype_Name[MAX_PATH] = {};

			ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);


			_float4x4 WorldMatrix = {};
			ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);
			//int a = 4;

			_tchar Translate_wchar[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, Prototype_Name, strlen(Prototype_Name), Translate_wchar, MAX_PATH);

			/* 이제 TRANSFORM만 건들면 될듯함.*/
			//int b = 4;
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Translate_wchar, LEVEL_GAMEPLAY, Layer_Name)))
				return E_FAIL;

			//CTransform* pTrasnform = dynamic_cast<CTransform*>(
			//* Find GaemObject 만들어야 할듯
			// 구분할 수 있는 방법을 생각해봐야할듯.
			map<const _wstring, class CLayer*>* Level_Layers = m_pGameInstance->Get_Layers();

			auto& Level_GamePlay = Level_Layers[3];

			for (auto& Layers : Level_GamePlay)
			{
				//auto& iter = find(Level_GamePlay.begin(), Level_GamePlay.end(), Layer_Name);	
				auto iter = Level_GamePlay.find(Layer_Name);

				if (iter == Level_GamePlay.end())
					return E_FAIL;

				else
				{
					CTransform* pTranform = dynamic_cast<CTransform*>(
						iter->second->Get_GameObject_List().back()->Find_Component(TEXT("Com_Transform")));

					pTranform->Set_WorldMatrix(WorldMatrix);



				}
			}
		}
		CloseHandle(hFile);
	}


	return S_OK;
}

HRESULT CImgui_File::ModelFile_Save()
{
	OPENFILENAME ofn;           // OPENFILENAME 구조체 선언		
	wchar_t szFile[MAX_PATH] = {};   // 파일 이름을 저장할 버퍼 (최대 260자)		

	// 구조체 초기화
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // 부모 윈도우 핸들		
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // 초기 파일 이름 (빈 문자열)		
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;      // 기본 선택 파일 형식
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // 초기 디렉토리
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetSaveFileName(&ofn))
	{
		/* 파일 저장할려면 이름이 필요*/
		/* 일단 확장자 검사하기 Height 맵은 bmp파일로 저장해야 하니깐*/

		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		_ulong dwByte = {};

		/* 여기서는 그러면 해당 이름과 transformMatrix를 저장해야한다.*/

		/* 일단 그러면 GaemPlay 레벨 가져오자 */
		auto& Layer = m_pLayer[3];

		/* 이제 해당 GamePlay안에 레이어들 안에 있는 객체들을 순환하면서 저장*/
		for (auto& Layers : m_pLayer[3])	// 이게 GAMEPLAY_LEVEL
		{
			/* 레이어 이름 저장하기 */
			const _tchar* Layer_Name = Layers.first.c_str();


			if (0 != lstrcmp(Layer_Name, TEXT("FreeCamera"))
				&& 0 != lstrcmp(Layer_Name, TEXT("Layer_Terrain"))
				&& 0 != lstrcmp(Layer_Name, TEXT("Layer_Player")))// 여기서 레이어( 카메라, 플레이어 , 지형등은 빼주기)
			{
				for (auto& GameObject : Layers.second->Get_GameObject_List())
				{

					WriteFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

					//_tchar* Object_Name = TEXT("");	
					//MultiByteToWideChar(CP_ACP, 0, GameObject->Get_Name(), strlen(GameObject->Get_Name()), Object_Name, strlen(GameObject->Get_Name()) + 1);	

					_char  Object_Name[MAX_PATH];
					memcpy(&Object_Name, GameObject->Get_Name(), sizeof(_char) * MAX_PATH);

					WriteFile(hFile, Object_Name, MAX_PATH, &dwByte, nullptr); // 이게 오브젝트 Prototype_GameObject_Mushroom 이거임

					_float4x4 Worldmatrix = {}; // 모든 맴버를 0으로 초기화
					CTransform* pTransform = dynamic_cast<CTransform*>(GameObject->Find_Component(TEXT("Com_Transform")));
					memcpy(&Worldmatrix, pTransform->Get_WorldFloat4X4(), sizeof(_float4x4));
					WriteFile(hFile, &Worldmatrix, sizeof(_float4x4), &dwByte, nullptr);

					int a = 4;
				}
			}
		}

		CloseHandle(hFile);
		MSG_BOX("파일이 성공적으로 저장되었습니다.");
	}

	return S_OK;
}

CImgui_File* CImgui_File::Create()
{
	CImgui_File* pInstance = new CImgui_File();
	
	if(FAILED(pInstance->Init()))
	{
		MSG_BOX("Failed to Created : Imgui_File_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImgui_File::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
