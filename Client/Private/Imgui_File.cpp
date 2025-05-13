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

	OPENFILENAME ofn;       // OPENFILENAME ����ü ����	
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)	

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�	
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)	
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";// ���� ���� ����	
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetOpenFileName(&ofn))
	{
		_tchar szEXT[MAX_PATH] = {};

		_ulong dwByte = {};
		/* bin �����Ͻ� �ƴҽ� ���ǹ� �ɱ� */
		_wsplitpath_s(szFile, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);


		HANDLE hFile = CreateFile(szFile, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		/* ���� �ϳ��ϳ� ������ ������*/

		while (true)
		{
			_tchar Layer_Name[MAX_PATH] = {}; //���̾� �̸�										
			ReadFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

			if (dwByte == 0)
				break;
			/* �̸� �۾� */
			_char   Prototype_Name[MAX_PATH] = {};

			ReadFile(hFile, Prototype_Name, MAX_PATH, &dwByte, nullptr);


			_float4x4 WorldMatrix = {};
			ReadFile(hFile, &WorldMatrix, sizeof(_float4x4), &dwByte, nullptr);
			//int a = 4;

			_tchar Translate_wchar[MAX_PATH] = {};
			MultiByteToWideChar(CP_ACP, 0, Prototype_Name, strlen(Prototype_Name), Translate_wchar, MAX_PATH);

			/* ���� TRANSFORM�� �ǵ�� �ɵ���.*/
			//int b = 4;
			if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Translate_wchar, LEVEL_GAMEPLAY, Layer_Name)))
				return E_FAIL;

			//CTransform* pTrasnform = dynamic_cast<CTransform*>(
			//* Find GaemObject ������ �ҵ�
			// ������ �� �ִ� ����� �����غ����ҵ�.
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
	OPENFILENAME ofn;           // OPENFILENAME ����ü ����		
	wchar_t szFile[MAX_PATH] = {};   // ���� �̸��� ������ ���� (�ִ� 260��)		

	// ����ü �ʱ�ȭ
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = g_hWnd;   // �θ� ������ �ڵ�		
	ofn.lpstrFile = szFile;
	ofn.lpstrFile[0] = '\0';  // �ʱ� ���� �̸� (�� ���ڿ�)		
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = L"Binary Files\0*.bin\0All Files\0*.*\0";
	ofn.nFilterIndex = 1;      // �⺻ ���� ���� ����
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"..\\";  // �ʱ� ���丮
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;


	if (GetSaveFileName(&ofn))
	{
		/* ���� �����ҷ��� �̸��� �ʿ�*/
		/* �ϴ� Ȯ���� �˻��ϱ� Height ���� bmp���Ϸ� �����ؾ� �ϴϱ�*/

		HANDLE hFile = CreateFile(ofn.lpstrFile, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

		_ulong dwByte = {};

		/* ���⼭�� �׷��� �ش� �̸��� transformMatrix�� �����ؾ��Ѵ�.*/

		/* �ϴ� �׷��� GaemPlay ���� �������� */
		auto& Layer = m_pLayer[3];

		/* ���� �ش� GamePlay�ȿ� ���̾�� �ȿ� �ִ� ��ü���� ��ȯ�ϸ鼭 ����*/
		for (auto& Layers : m_pLayer[3])	// �̰� GAMEPLAY_LEVEL
		{
			/* ���̾� �̸� �����ϱ� */
			const _tchar* Layer_Name = Layers.first.c_str();


			if (0 != lstrcmp(Layer_Name, TEXT("FreeCamera"))
				&& 0 != lstrcmp(Layer_Name, TEXT("Layer_Terrain"))
				&& 0 != lstrcmp(Layer_Name, TEXT("Layer_Player")))// ���⼭ ���̾�( ī�޶�, �÷��̾� , �������� ���ֱ�)
			{
				for (auto& GameObject : Layers.second->Get_GameObject_List())
				{

					WriteFile(hFile, Layer_Name, MAX_PATH, &dwByte, nullptr);

					//_tchar* Object_Name = TEXT("");	
					//MultiByteToWideChar(CP_ACP, 0, GameObject->Get_Name(), strlen(GameObject->Get_Name()), Object_Name, strlen(GameObject->Get_Name()) + 1);	

					_char  Object_Name[MAX_PATH];
					memcpy(&Object_Name, GameObject->Get_Name(), sizeof(_char) * MAX_PATH);

					WriteFile(hFile, Object_Name, MAX_PATH, &dwByte, nullptr); // �̰� ������Ʈ Prototype_GameObject_Mushroom �̰���

					_float4x4 Worldmatrix = {}; // ��� �ɹ��� 0���� �ʱ�ȭ
					CTransform* pTransform = dynamic_cast<CTransform*>(GameObject->Find_Component(TEXT("Com_Transform")));
					memcpy(&Worldmatrix, pTransform->Get_WorldFloat4X4(), sizeof(_float4x4));
					WriteFile(hFile, &Worldmatrix, sizeof(_float4x4), &dwByte, nullptr);

					int a = 4;
				}
			}
		}

		CloseHandle(hFile);
		MSG_BOX("������ ���������� ����Ǿ����ϴ�.");
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
