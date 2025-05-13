#include "pch.h"    
#include "Imgui_Camera.h"
#include "GameInstance.h"
#include <Commdlg.h>

CImgui_Camera::CImgui_Camera()
    :m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CImgui_Camera::Initialize()
{
    /* 처음 초기화 될 때  만들어야 할 것들 정리해두기 */

    

    return S_OK;    
}

HRESULT CImgui_Camera::Update(_float _fTiemDelta)
{
    m_fTimeDelta = _fTiemDelta; 

    if (ImGui::BeginMenuBar())      
    {
        if (ImGui::BeginMenu("Camera"))     
        {
            if (ImGui::MenuItem("Camera Select Window", nullptr, &m_bCameraWindowOnOff))
            {
                int a = 4;
            }
            
            if (ImGui::MenuItem("Camera Information Save", nullptr, &m_bCameraSaveOnOff))
            {
                Camera_Save();  
            }

            if (ImGui::MenuItem("Camera Information Load", nullptr, &m_bCameraLoadOnOff))
            {
                Camera_Load();  
            }
         
            
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();    
    }

    Camera_Window();    

    return S_OK;    
}

HRESULT CImgui_Camera::Camera_Save()
{
    OPENFILENAME ofn;                // OPENFILENAME 구조체 선언		
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
        for(auto& iter : *m_pListGameObject)
        {
            strcpy_s(m_szSaveCamName, iter->Get_Name());
            WriteFile(hFile, m_szSaveCamName, MAX_PATH, &dwByte, nullptr);  

            /* 카메라 월드 행렬 */
            WriteFile(hFile, &iter->Get_CameraWorldMatrix(), sizeof(_float4x4), &dwByte, nullptr);  

            WriteFile(hFile, &iter->Get_CameraRelativePos(), sizeof(_float4), &dwByte, nullptr);

            float LerpTime = iter->Get_CameraLerpTime();    
            WriteFile(hFile, &LerpTime, sizeof(_float), &dwByte, nullptr); 

            float LerpTimeSpeed = iter->Get_CameraLerpTimeSpeed();      
            WriteFile(hFile, &LerpTimeSpeed, sizeof(_float), &dwByte, nullptr); 

            _uint LerpMethod = iter->Get_CameraLerpMethod();
            WriteFile(hFile, &LerpMethod, sizeof(_uint), &dwByte, nullptr); 

        }
        CloseHandle(hFile);
        MSG_BOX("파일이 성공적으로 저장되었습니다.");
    }
    
    else
    {
        MSG_BOX("파일 저장 실패");
    }

    return S_OK;
}

HRESULT CImgui_Camera::Camera_Load()
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

        /* 이제 하나하나 정보를 빼오자 */

        while (true)
        {
            _char Cam_Name[MAX_PATH] = {};  // 카메라 이름
            ReadFile(hFile, Cam_Name, MAX_PATH, &dwByte, nullptr);
            
            if (dwByte == 0)
                break; 
            
            _float4x4 Cam_WorldMatrix = {}; 
            ReadFile(hFile, &Cam_WorldMatrix, sizeof(_float4x4), &dwByte, nullptr); 

            _float4 RelativePos = {}; 
            ReadFile(hFile, &RelativePos, sizeof(_float4), &dwByte, nullptr);   

            _float LerpTime = {};
            ReadFile(hFile, &LerpTime, sizeof(_float), &dwByte, nullptr);

            _float LerpTimeSpeed = {};
            ReadFile(hFile, &LerpTimeSpeed, sizeof(_float), &dwByte, nullptr);  

            _uint  LerpMethod = {}; 
            ReadFile(hFile, &LerpMethod, sizeof(_uint), &dwByte, nullptr);

            CGameObject::GAMEOBJECT_DESC ObjectDesc{};  

            ObjectDesc._fPosition = { Cam_WorldMatrix._41,Cam_WorldMatrix._42,Cam_WorldMatrix._43,1.f };
            ObjectDesc._fScaling = { 0.0025f, 0.0025f, 0.0025f ,1.f};     

            _float4x4 CamWorldmatrix = {};          
            XMStoreFloat4x4(&CamWorldmatrix, XMLoadFloat4x4(&Cam_WorldMatrix));   
            ObjectDesc._ParentMatrix = &CamWorldmatrix;     

            ObjectDesc.szName = Cam_Name;
            ObjectDesc.fSpeedPerSec = 25.f;
            ObjectDesc.fSpeedPerSec = 1.f;
            ObjectDesc._fRelativePos = RelativePos;         
            ObjectDesc._fLerpTime = LerpTime;   
            ObjectDesc._fLerpTimeSpeed = LerpTimeSpeed; 
            ObjectDesc._iLerpMethod = LerpMethod;   

            if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeObject"),
                LEVEL_GAMEPLAY, TEXT("Layer_Cube"), &ObjectDesc)))
            {
                MSG_BOX("큐브 생성 실패");
            }

            
            

        }
        CloseHandle(hFile);
    }

    return S_OK;
}

void CImgui_Camera::Camera_Window() 
{

    if(m_bCameraWindowOnOff)
    { 
        if (ImGui::Begin("Camera Tool Window", &m_bCameraWindowOnOff))      
        {
            /* 키입력에 따른 큐브 설치 */
            Cube_Install_Method();  
            /* ===================  */

            ImGui::NewLine();   

            /* 설치된 큐브 목록 보는 창 */
            Installed_Cube_List();  
            /* ===================  */

            /* 카메리 움직이기 */
            Move_Camera();  

            /* 이동 분기점들 넣는 곳 */  
            Play_Camera();  
            
            /* 카메라 변환 테스트 */
            Rotation_Camera();


        }

        ImGui::End(); // 창 종료를 위해 반드시 호출해야 합니다.
    }
}

void CImgui_Camera::Cube_Install_Method()   
{

    if(m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("FreeCamera"), "Free_Camera")->Get_Transfrom() != nullptr)
        m_pCameraTransformCom = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("FreeCamera"), "Free_Camera")->Get_Transfrom();
    
    if (m_pGameInstance->isKeyEnter(DIK_T))     
    {   
        CGameObject::GAMEOBJECT_DESC ObjectDesc{};      

        

        _float4 vCurPos = {}; 
        XMStoreFloat4(&vCurPos, m_pCameraTransformCom->Get_State(CTransform::STATE_POSITION));    

        

        ObjectDesc._fPosition = vCurPos;    
        ObjectDesc._fScaling  = { 0.0025f, 0.0025f, 0.0025f ,1.f};      
        _float4x4 CamWorldmatrix = {}; 
        XMStoreFloat4x4(&CamWorldmatrix, m_pCameraTransformCom->Get_WorldMatrix());   
        ObjectDesc._ParentMatrix = &CamWorldmatrix;         
        ObjectDesc.fSpeedPerSec = 25.f; 
        ObjectDesc.fSpeedPerSec = 1.f;


        _vector PlayerPostiion = m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
        _vector RelativePosition = XMVectorSetW(XMLoadFloat4(&vCurPos) - PlayerPostiion, 1.f);
        _float4 fRelativePos = {}; 
        XMStoreFloat4(&fRelativePos, RelativePosition); 

        ObjectDesc._fRelativePos = fRelativePos;    

        //string name = "Cam_" + to_string(m_iIndex);  // 이걸 크기로 해야할듯 
        string name = {  };

        if (m_pListGameObject != nullptr)   
        {
            name = "Cam_" + to_string(m_pListGameObject->size() + 1);   
               
        }

        else
        {
            name = "Cam_" + to_string(0);           
          
        }
        
        ObjectDesc.szName = name.c_str();

        m_iIndex++;     

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeObject"),
            LEVEL_GAMEPLAY, TEXT("Layer_Cube"),&ObjectDesc)))   
        {
            MSG_BOX("큐브 생성 실패");
        }
    }

}

void CImgui_Camera::Installed_Cube_List()   
{
    if (ImGui::BeginTable("ScrollableTable", 7, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY, ImVec2(0, 300)))
    {
        ImGui::TableSetupColumn("ID");  
        ImGui::TableSetupColumn("X");    
        ImGui::TableSetupColumn("Y");   
        ImGui::TableSetupColumn("Z");
        ImGui::TableSetupColumn("LerpTime");
        ImGui::TableSetupColumn("LerpTimeSpeed");
        ImGui::TableSetupColumn("LerpMethod");    
        ImGui::TableHeadersRow();   

        _uint index = {};

      
        m_pPlayerGameObject = m_pGameInstance->Get_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Model"), "PLAYER");
        m_pListGameObject   = m_pGameInstance->Get_Layer_List(LEVEL_GAMEPLAY, TEXT("Layer_Cube"));

        if (m_pListGameObject != nullptr)
        {

            for (auto& iter : *m_pListGameObject)  // 데이터가 많아도 스크롤 가능    
            {

                /* 큐브 리스트를 놓자 .*/
                bool selected = (m_iSelected_row == index);
                if (ImGui::Selectable(std::to_string(index).c_str(), selected, ImGuiSelectableFlags_SpanAllColumns)) {
                    m_iSelected_row = index;
                }

                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::Text(iter->Get_Name());

                //_vector PlayerPostiion = m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);
                _vector CamPosition = iter->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

                //_vector RelativePosition = XMVectorSetW(CamPosition - PlayerPostiion, 1.f);

                ImGui::TableSetColumnIndex(1);
                ImGui::Text("%.2f", CamPosition.m128_f32[0]);

                ImGui::TableSetColumnIndex(2);
                ImGui::Text("%.2f", CamPosition.m128_f32[1]);

                ImGui::TableSetColumnIndex(3);
                ImGui::Text("%.2f", CamPosition.m128_f32[2]);

                ImGui::TableSetColumnIndex(4);      
                ImGui::Text("%.2f", iter->Get_CameraLerpTime());           

                ImGui::TableSetColumnIndex(5);  
                ImGui::Text("%.2f", iter->Get_CameraLerpTimeSpeed());    

                ImGui::TableSetColumnIndex(6);
                if (iter->Get_CameraLerpMethod() == 0)
                    ImGui::Text("Linear_Method");
                else
                    ImGui::Text("CatMullRom_Method");

                index++;
            }
        }
        

        ImGui::EndTable();
    }

    ImGui::InputText(u8" 삭제할 카메라 : ", m_szDeleteCamName, MAX_PATH);  
    
    ImGui::NewLine();   
    ImGui::Dummy(ImVec2(450.f, 0.f));   
    ImGui::SameLine();

    if(ImGui::Button(u8"삭제하기",ImVec2(100.f,25.f)))
    {
        string  test = m_szDeleteCamName;   

        m_pGameInstance->Sub_GameObject_To_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Cube"), test.c_str()); 

    }
    ImGui::NewLine();     


}

void CImgui_Camera::Play_Camera()   
{
    ImGui::NewLine();


    if(ImGui::Button(u8"등록", ImVec2(200.f,30.f)))
    {
        m_vecCameraEvent.clear();   
        
        Camera_Event pCamEventDesc = {};   

        for (auto& iter : *m_pListGameObject)   
        {
            
            strcpy_s(pCamEventDesc.szName, iter->Get_Name());    
            pCamEventDesc.LerpTime       = iter->Get_CameraLerpTime();  
            pCamEventDesc.LerpTimeSpeed  = iter->Get_CameraLerpTimeSpeed();      
            pCamEventDesc.CamWorldMatrix = iter->Get_CameraWorldMatrix();
            pCamEventDesc.LerpMethod     = iter->Get_CameraLerpMethod();
           

            //m_vecCameraMatrix.push_back(iter->Get_CameraWorldMatrix());     
            m_vecCameraEvent.push_back(pCamEventDesc);  
        }

        MSG_BOX("등록 완료");

    }


    ImGui::SameLine();  
    ImGui::Dummy(ImVec2(200.f, 0.f));   
    ImGui::SameLine();  

    
    if(ImGui::Button("Play", ImVec2(100.f,30.f)))
    {
        m_bPlayOnOff = !m_bPlayOnOff;    
        
    }
    
    if (m_pGameInstance->isKeyEnter(DIK_SPACE))
    {
        m_bPlayOnOff = !m_bPlayOnOff;
       
    }
 

    if(m_bPlayOnOff)    
    {
        
        m_fCurTime += m_vecCameraEvent.at(m_iPlayCamera_Index).LerpTimeSpeed * m_fTimeDelta;    
    
        _float  fRatio = m_fCurTime / m_vecCameraEvent.at(m_iPlayCamera_Index).LerpTime;    
       
        _uint CamCount = m_pListGameObject->size();             

       
        if (fRatio <= 1.f)
        {
        
            _vector   vScale, vRotation, vTranslation;
            XMMatrixDecompose(&vScale, &vRotation, &vTranslation, XMLoadFloat4x4(&m_vecCameraEvent.at(m_iPlayCamera_Index).CamWorldMatrix));
        
            _vector    vNextScale, vNextRotation, vNextTranslation;
            XMMatrixDecompose(&vNextScale, &vNextRotation, &vNextTranslation, XMLoadFloat4x4(&m_vecCameraEvent.at(m_iPlayCamera_Index+1).CamWorldMatrix));
            

            
            vScale = XMVectorLerp(vScale, vNextScale, fRatio);
           
            vRotation = XMQuaternionSlerp(vRotation, vNextRotation, fRatio);        

            // 선형보간 or 캣 멀롬 보간 
            if(m_vecCameraEvent.at(m_iPlayCamera_Index).LerpMethod == CAMER_LERP_METHOD::LINEAR_CAMERA)
                vTranslation = XMVectorSetW(XMVectorLerp(vTranslation, vNextTranslation, fRatio), 1.f);
            
            else if (m_vecCameraEvent.at(m_iPlayCamera_Index).LerpMethod == CAMER_LERP_METHOD::CATMULL_ROM_CAMERA)
                vTranslation = XMVectorSetW(CatmullRom_Position_Lerp(&m_vecCameraEvent,fRatio), 1.f);   

            
            _matrix TransformationMatrix = XMMatrixAffineTransformation(vScale, XMVectorSet(0.f, 0.f, 0.f, 1.f), vRotation, vTranslation);
        
            _float4x4 test = {};
            XMStoreFloat4x4(&test, TransformationMatrix);   
        
            m_pCameraTransformCom->Set_WorldMatrix(test);
     
    
        }

        else if(fRatio > 1.f)   
        {
            if (m_iPlayCamera_Index < (CamCount-2))
            {
                m_fCurTime = 0.f;   
                m_iPlayCamera_Index++;
            }
            
            else
            {
                m_bPlayOnOff = false;   
                //g_bRootOnOff = false; 
            }
        }
    }

    if(ImGui::Button("Reset",ImVec2(100.f,35.f)))
    {
        m_iPlayCamera_Index = 0;    
        m_fCurTime = 0.f;       
    }


}

void CImgui_Camera::Move_Camera()   
{
   ImGui::SetNextItemWidth(250.0f);  // 원하는 너비 설정   
   ImGui::InputFloat(u8"카메라 보간 속도 : ", &m_fLerpSpeed, 0.1f);
   ImGui::NewLine();

   ImGui::SetNextItemWidth(250.0f);  // 원하는 너비 설정   
   ImGui::InputFloat(u8"카메라 보간 시간 : ", &m_fLerpTime, 0.1f);

   ImGui::Dummy(ImVec2(400.f, 0.f));    
   ImGui::SameLine();   

   ImGui::NewLine(); 
   if(ImGui::RadioButton("Linera_Lerp", m_iSelectLerpMethod == CAMER_LERP_METHOD::LINEAR_CAMERA))    
   {
       m_iSelectLerpMethod = CAMER_LERP_METHOD::LINEAR_CAMERA;  
   }
   
   if (ImGui::RadioButton("Cat_Mull_Rom_Lerp", m_iSelectLerpMethod == CAMER_LERP_METHOD::CATMULL_ROM_CAMERA))    
   {    
       m_iSelectLerpMethod = CAMER_LERP_METHOD::CATMULL_ROM_CAMERA; 
   }


   /* 수정 완료 버튼 만들기 */
   if (ImGui::Button(u8"카메라 수정 완료", ImVec2(150.f, 25.f)))
   {
       m_pCurSelectedCam->Set_CameraLerpTime(m_fLerpTime);  
       m_pCurSelectedCam->Set_CameraLerpTimeSpeed(m_fLerpSpeed);    
       m_pCurSelectedCam->Set_CameraWorldMatrix(m_pCameraTransformCom->Get_WorldMatrix());  
       m_pCurSelectedCam->Get_Transfrom()->Set_State(CTransform::STATE_POSITION, m_pCameraTransformCom->Get_WorldMatrix().r[3]);
       m_pCurSelectedCam->Set_CameraLerpMethod(m_iSelectLerpMethod);    

       _vector PlayerPostiion = m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);    
       m_pCurSelectedCam->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);   

       _vector RelativePosition = XMVectorSetW(m_pCurSelectedCam->Get_Transfrom()->Get_State(CTransform::STATE_POSITION) - PlayerPostiion, 1.f);
       _float4 storeRelativePos = {}; 
       XMStoreFloat4(&storeRelativePos, RelativePosition);  
       m_pCurSelectedCam->Set_CameraRelativePos(storeRelativePos);  
   }  

   

    /*  움직일 수 있게 해당 카메라로 이동하고 움직이기 */

    if (m_pListGameObject != nullptr)
    {
        _uint index = {};

        for (auto& iter : *m_pListGameObject)
        {
            if (index == m_iSelected_row)
            {
                m_pCurSelectedCam = iter;
                break;
            }

            index++;
        }
    }

    ImGui::SameLine();
    ImGui::Dummy(ImVec2(200.f, 0.f));
    ImGui::SameLine();

    ImGui::NewLine();
    /* 해당 카메라로 이동 버튼 만들기 */
    if (ImGui::Button(u8"카메라 이동", ImVec2(100.f, 25.f)))
    {
        m_pCameraTransformCom->Set_WorldMatrix(m_pCurSelectedCam->Get_CameraWorldMatrix());
    }

    ImGui::NewLine();



}

void CImgui_Camera::Rotation_Camera()
{
    ImGui::NewLine();
    
      
    if(ImGui::Button(u8"회전적용", ImVec2(100.f, 20.f)))
    {
        if (m_pListGameObject != nullptr)
        {

            for (auto& iter : *m_pListGameObject)
            {
                _vector pPlayerPos = m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);

                /*플레이어의 xz좌표 */
                _vector pPlayerRightDir = { 1.f ,0.f,0.f,0.f }; 
                _vector pPlayerCurrentRightDir = XMVector3Normalize(m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT));      
                
                _float dot = XMVectorGetX(XMVector3Dot(pPlayerRightDir, pPlayerCurrentRightDir));   
                dot = max(-1.0f, min(dot, 1.0f));           
                float Radian = acosf(dot);          

                _vector crossResult = XMVector3Cross(pPlayerRightDir, pPlayerCurrentRightDir);  
                float crossY = XMVectorGetY(crossResult);       
                if (crossY < 0.0f) {    
                    Radian = -Radian;   
                }
                

                
                iter->Get_Transfrom()->Orbit_Move_Once(_fvector{ 0.f,1.f,0.f,0.f }, Radian, pPlayerPos);
            }

            for (auto& iter : m_vecCameraEvent)
            {
                _vector pPlayerPos = m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_POSITION);   

                /*플레이어의 xz좌표 */
                _vector pPlayerRightDir = { 1.f ,0.f,0.f,0.f }; 
                _vector pPlayerCurrentRightDir = XMVector3Normalize(m_pPlayerGameObject->Get_Transfrom()->Get_State(CTransform::STATE_RIGHT));  

                _float dot = XMVectorGetX(XMVector3Dot(pPlayerRightDir, pPlayerCurrentRightDir));   
                dot = max(-1.0f, min(dot, 1.0f));   
                float Radian = acosf(dot);  

                _vector crossResult = XMVector3Cross(pPlayerRightDir, pPlayerCurrentRightDir);  
                float crossY = XMVectorGetY(crossResult);   
                if (crossY < 0.0f) {    
                    Radian = -Radian;   
                }

                _matrix			RotationMatrix = XMMatrixRotationAxis(_fvector{0.f,1.f,0.f,0.f}, Radian);

                XMStoreFloat4x4(&iter.CamWorldMatrix, XMMatrixMultiply(XMLoadFloat4x4(&iter.CamWorldMatrix), RotationMatrix));
                /* 지금 플레이어가 원점이라 되는거임 이거 오해 ㄴㄴ*/

                //XMStoreFloat4x4(&iter.) 원래 여기서 공전효과 줘야함 
            }
        }
    }

}

_vector CImgui_Camera::CatmullRom_Rotation_Lerp(vector<Camera_Event>* pvecCameraEvent, _float _fRatio)  
{
    _vector   vScale_1, vRotation_1, vTranslation_1;  
    XMMatrixDecompose(&vScale_1, &vRotation_1, &vTranslation_1, XMLoadFloat4x4(&m_vecCameraEvent.at(0).CamWorldMatrix));      

    _vector    vScale_2, vRotation_2, vTranslation_2;
    XMMatrixDecompose(&vScale_2, &vRotation_2, &vTranslation_2, XMLoadFloat4x4(&m_vecCameraEvent.at(1).CamWorldMatrix));

    _vector    vScale_3, vRotation_3, vTranslation_3;
    XMMatrixDecompose(&vScale_3, &vRotation_3, &vTranslation_3, XMLoadFloat4x4(&m_vecCameraEvent.at(2).CamWorldMatrix));

    _vector    vScale_4, vRotation_4, vTranslation_4;
    XMMatrixDecompose(&vScale_4, &vRotation_4, &vTranslation_4, XMLoadFloat4x4(&m_vecCameraEvent.at(3).CamWorldMatrix));



    _vector q1 = vRotation_1;
    _vector q2 = vRotation_2;
    _vector q3 = vRotation_3;
    _vector q4 = vRotation_4;


    return XMQuaternionSlerp(
        XMQuaternionSlerp(q1, q2, _fRatio),  // q1 → q2 보간  
        XMQuaternionSlerp(q2, q3, _fRatio),  // q2 → q3 보간      
        _fRatio
    );


    return _vector();
}

_vector CImgui_Camera::CatmullRom_Position_Lerp(vector<Camera_Event>* pvecCameraEvent, _float _fRatio)
{
    _vector  p1 = { m_vecCameraEvent.at(m_iPlayCamera_Index-1).CamWorldMatrix._41,
                    m_vecCameraEvent.at(m_iPlayCamera_Index-1).CamWorldMatrix._42,
                    m_vecCameraEvent.at(m_iPlayCamera_Index-1).CamWorldMatrix._43,
                    1.f };

    _vector  p2 = { m_vecCameraEvent.at(m_iPlayCamera_Index).CamWorldMatrix._41,
                  m_vecCameraEvent.at(m_iPlayCamera_Index).CamWorldMatrix._42,
                  m_vecCameraEvent.at(m_iPlayCamera_Index).CamWorldMatrix._43,
                 1.f };

    _vector  p3 = { m_vecCameraEvent.at(m_iPlayCamera_Index+1).CamWorldMatrix._41,
                 m_vecCameraEvent.at(m_iPlayCamera_Index+1).CamWorldMatrix._42,   
                 m_vecCameraEvent.at(m_iPlayCamera_Index+1).CamWorldMatrix._43,   
                 1.f };

    _vector  p4 = { m_vecCameraEvent.at(m_iPlayCamera_Index+2).CamWorldMatrix._41,    
                m_vecCameraEvent.at(m_iPlayCamera_Index+2).CamWorldMatrix._42,      
                m_vecCameraEvent.at(m_iPlayCamera_Index+2).CamWorldMatrix._43,      
                1.f };

    _vector position = 0.5f * ((2 * p2) +       
        (-p1 + p3) * _fRatio +  
        (2 * p1 - 5 * p2 + 4 * p3 - p4) * (_fRatio * _fRatio) + 
        (-p1 + 3 * p2 - 3 * p3 + p4) * (_fRatio * _fRatio * _fRatio));  


    position = XMVectorSetW(position, 1.f);

    return position;        

}   

CImgui_Camera* CImgui_Camera::Create()
{
    CImgui_Camera* pInstance = new CImgui_Camera();

    if(FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : Imgui_Camera");
        Safe_Release(pInstance);    
    }

    return pInstance; 
}

void CImgui_Camera::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
   
}
