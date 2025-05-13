#include "pch.h"
#include "Imgui_Model.h"
#include "GameObject.h"
#include "Layer.h"
#include "Model.h"  
#include "Animation.h"

bool g_bRootOnOff = { false };

CImgui_Model::CImgui_Model()
    :m_pGameInstance(CGameInstance::GetInstance())
{
    Safe_AddRef(m_pGameInstance);   
}

HRESULT CImgui_Model::Initialize()
{
      
    return S_OK;    
}

HRESULT CImgui_Model::Update(_float _fTiemDelta)
{
    m_fTimeDetla = m_fTimeDetla; 

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Model"))  
		{
			if (ImGui::MenuItem("Model Select Window", nullptr, &m_bModelWindowOnOff))   
			{   
                int a = 4;
			}   
            
            if (ImGui::MenuItem("Model Weapon Select Window", nullptr, &m_bModelWeaponWindow))
            {
                int a = 4;
            }

            if (ImGui::MenuItem("Model Animation Event Frame", nullptr, &m_bEventFrameWindow))  
            {
                int a = 4;
            }


            if (ImGui::MenuItem("Model Animation Save", nullptr, &m_bModelSaveOnOff))   
            {   
                Model_Save();   
            }

            ImGui::EndMenu();
		}


		ImGui::EndMenuBar();        
	}


    Model_Window();
    
   
    if (m_pModel != nullptr)
    {
        if (m_bAnimationPlay || m_bAnimationStop)
        {
            float fCurrentFramePosition = m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Get_CurrentKeyFramePosition();

            if (m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->isAniMationFinish()
                ||(m_bAniChangeTrackOnOff && (m_iAniChangeTrackPosition == (int)fCurrentFramePosition))) // 특정 트렉포지션 일때 변경 기능
            {
                if (m_iCurSelectAnimationIndex != m_iNextSelectAnimationIndex)
                {
                    m_pModel->SetUp_Animation(m_iNextSelectAnimationIndex, m_bLoop);
                    m_iPreSelectAnimationIndex = m_iCurSelectAnimationIndex;
                }
            }
            else
            {
                m_pModel->SetUp_Animation(m_iCurSelectAnimationIndex, m_bLoop); 
            }
        
            if (m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->isAniMationFinish())
            {
                m_bAnimationPlay = false;   
                /* 이 애니메이션 리셋 시켜야함 */
                //m_pModel->Reset_CurAnimation(); 
                //m_pModel->Reset_Animation(m_iCurSelectAnimationIndex);  

                m_pModel->Reset_Animation(m_iCurSelectAnimationIndex);      
                m_pModel->Reset_Animation(m_iNextSelectAnimationIndex);     
                
            }

            m_pModel->Play_Animation(_fTiemDelta);  
        }

      
        /* 키프레임에 따른 움직임 관련 코드 */
        if (m_bKeyFrameOnOff)
        {
            m_pModel->Play_Animation(_fTiemDelta);
        }
        /* ================================ */
    }
       
    

    return S_OK;    
}

void CImgui_Model::Enroll_Object()  
{
    if (g_bFbxFileDragOnOff)    
    {
       // 삭제 관련 코드 ( 이미 화면에 캐릭터 나와 있는데 드래그 드랍 할 시 ) 
       map<const _wstring, class CLayer*>* pObjectMgrMap = m_pGameInstance->Get_Layers();  

       for(auto& iter : pObjectMgrMap[3])
       {
           if(!wcscmp(TEXT("Layer_Model"),iter.first.c_str()))
           {
               list<CGameObject*>& pLayerGameObject = iter.second->Get_GameObject_List(); 

               for (auto& listiter = pLayerGameObject.begin(); listiter != pLayerGameObject.end(); listiter++)
               {
                   Safe_Release(*listiter);
                   pLayerGameObject.erase(listiter);     
                   
                   m_bRotationValueReset = true;    

                   if (pLayerGameObject.size() == 0)
                       break;
               }
           }
       }
       /* =========================================== */


        wchar_t Title[MAX_PATH] = L"Prototype_GameObject_";   
        
        wcscat_s(Title, g_CurrentFbxFileName);
        

        if (FAILED(m_pGameInstance->Add_GameObject_To_Layer(LEVEL_GAMEPLAY, Title, LEVEL_GAMEPLAY, TEXT("Layer_Model"), nullptr)))
        {
            MSG_BOX("Failed to Improt fbx"); 
        }


        /* 게임 오브젝트 포인터 가져오기 */
        for (auto& iter : pObjectMgrMap[3])
        {
            if (!wcscmp(TEXT("Layer_Model"), iter.first.c_str()))
            {
                list<CGameObject*>& pLayerGameObject = iter.second->Get_GameObject_List();

                for (auto& listiter = pLayerGameObject.begin(); listiter != pLayerGameObject.end(); listiter++)
                {
                    if (pLayerGameObject.size() == 0)
                        break;

                    m_pGameObject = *listiter;  
                    m_pTransformCom = dynamic_cast<CTransform*>(m_pGameObject->Find_Component(TEXT("Com_Transform")));
                
                    if (m_pGameObject != nullptr)
                    {
                        m_pModel = m_pGameObject->Get_Model_Ptr();
                        m_pModel->Play_Animation(m_fTimeDetla);
                    }

                    m_vecAnimationName.clear(); 
                    m_vecAnimationName.reserve(m_pModel->Get_VecAnimation().size());    

                    for(_uint i =0; i < m_pModel->Get_VecAnimation().size(); i++)   
                    {
                        m_vecAnimationName.push_back(m_pModel->Get_VecAnimation().at(i)->Get_AnimationName());  
                    }   
                }
            }
        }
        /*===================================*/

        g_bFbxFileDragOnOff = false;    
    }
}

void CImgui_Model::Transform_Method()
{
    if (m_pGameObject != nullptr)
    {
        float Scale[3] = { m_pTransformCom->Get_Scaled().x, m_pTransformCom->Get_Scaled().y, m_pTransformCom->Get_Scaled().z };
        ImGui::Text("Scale");
        ImGui::SetNextItemWidth(375);   
        ImGui::SliderFloat3("Scale", Scale, 0.001f, 1.f);        

        //  m_pGameObject 포인터가 변경될 때 이거 초기화 해야함 
        if(m_bRotationValueReset)
        {   
            m_fRotationXYZ = {};    
            m_fPreRotationXYZ = {};
            m_bRotationValueReset = false;  
        }

        float Rotation[3] = { m_fRotationXYZ.x, m_fRotationXYZ.y ,m_fRotationXYZ.z };   
        ImGui::Text("Rotation");
        ImGui::SetNextItemWidth(375);
        ImGui::SliderFloat3("Rotation", Rotation, -180.f, 180.f);      
      

        float position[3] = { m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[0],m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[1],m_pTransformCom->Get_State(CTransform::STATE_POSITION).m128_f32[2] };
        ImGui::Text("Position");
        ImGui::SetNextItemWidth(375);
        ImGui::SliderFloat3("Position", position, -10.f, 10.f);


        if(Scale[0] != 0.f && Scale[1] != 0.f && Scale[2] != 0.f)
            m_pTransformCom->Set_Scale(Scale[0], Scale[1], Scale[2]);


        m_fRotationXYZ = { Rotation[0],Rotation[1],Rotation[2] };   

        /* 쿼터니언으로 수정하기 */
        m_pTransformCom->Turn_X(m_fRotationXYZ.x - m_fPreRotationXYZ.x);           
        m_pTransformCom->Turn_Y(m_fRotationXYZ.y - m_fPreRotationXYZ.y);           
        m_pTransformCom->Turn_Z(m_fRotationXYZ.z - m_fPreRotationXYZ.z);        
        /* ====================== */
        m_fPreRotationXYZ = m_fRotationXYZ;     

        m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ position[0],position[1],position[2],1.f });
    
    
    }

}

void CImgui_Model::Animaiton_Function()
{
   if(m_pModel != nullptr)
   {
       //// 선택된 항목의 인덱스를 저장할 변수 (초기값: 0)
       //static int current_item = 0;

       // Combo 박스를 그리기 (label, 현재 선택된 항목의 텍스트)
       ImGui::Text(u8"모델 애니메이션 종류");

       ImGui::NewLine();
       /* 현재 진행 시킬 애니메이션 */
       ImGui::Text(u8"처음 애니메이션");
       if (ImGui::BeginCombo(u8"처음 애니메이션", m_vecAnimationName[m_iCurSelectAnimationIndex]))    
       {
           // 벡터의 모든 항목을 순회하며 선택지로 표현
           for (int n = 0; n < m_vecAnimationName.size(); n++)
           {
               // 현재 항목이 선택되었는지 여부
               bool isSelected = (m_iCurSelectAnimationIndex == n);      
               if (ImGui::Selectable(m_vecAnimationName[n], isSelected))    
               {
                   // 사용자가 해당 항목을 선택하면 currentItem 값을 갱신합니다.
                   //if(m_pGameInstance->isMouseEnter(DIM_LB))
                   m_iCurSelectAnimationIndex = n;  
                   m_pModel->SetUp_Animation(m_iCurSelectAnimationIndex, m_bLoop);   
               }
               // 선택된 항목에 기본 포커스를 맞춥니다.
               if (isSelected)
               {
                   ImGui::SetItemDefaultFocus();
               }

           }
           ImGui::EndCombo();       
       }
       /* ========================================== */


       /* 다음 진행 시킬 애니메이션 */

       ImGui::Text(u8"다음 애니메이션");   
       if (ImGui::BeginCombo(u8"다음 애니메이션", m_vecAnimationName[m_iNextSelectAnimationIndex]))     
       {
           // 벡터의 모든 항목을 순회하며 선택지로 표현   
           for (int n = 0; n < m_vecAnimationName.size(); n++)      
           {
               // 현재 항목이 선택되었는지 여부  
               bool isSelected = (m_iNextSelectAnimationIndex == n);        
               if (ImGui::Selectable(m_vecAnimationName[n], isSelected))     
               {
                   // 사용자가 해당 항목을 선택하면 currentItem 값을 갱신합니다.    
                   m_iNextSelectAnimationIndex = n;     
                   //m_pModel->SetUp_Animation(m_iNextSelectAnimationIndex, m_bLoop);   
               }
               // 선택된 항목에 기본 포커스를 맞춥니다. 
               if (isSelected)  
               {
                   ImGui::SetItemDefaultFocus();    
               }

           }
           ImGui::EndCombo();   
       }
       /* ========================================== */

       ImGui::NewLine();
       ImGui::Checkbox("Loop", &m_bLoop);

       ImGui::SameLine(); 
       ImGui::Dummy(ImVec2(150.0f, 0.0f)); // 10픽셀 넓이의 빈 영역 생성   
       ImGui::SameLine();   
       if(ImGui::Button(u8"애니메이션 실행", ImVec2(120.f, 30.f)))
       {
           m_bAnimationPlay = !m_bAnimationPlay;
           /* 이 애니메이션 리셋 시켜야함 */
           //m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)

           //m_pModel->Reset_CurAnimation();
           m_pModel->Reset_Animation(m_iCurSelectAnimationIndex);
           m_pModel->Reset_Animation(m_iNextSelectAnimationIndex);  
           //m_pModel->Reset_Animation(m_iCurSelectAnimationIndex);    
       }

       /* SPACE BAR  기능 애니메이션 멈춤 및 재생 기능 */
       if (m_pGameInstance->isKeyEnter(DIK_SPACE))
       {
           m_bAnimationStop = !m_bAnimationStop;
           //g_bRootOnOff = true;
           
       }
       if (m_pGameInstance->isKeyEnter(DIK_LCONTROL))   
           m_bAnimationStop = !m_bAnimationStop;    

       if(m_pGameInstance->isKeyEnter(DIK_R))
       {
           m_pModel->Reset_Animation(m_iCurSelectAnimationIndex);
           m_pModel->Reset_Animation(m_iNextSelectAnimationIndex);
       }
   
       
            
     


       /* 키프레임 기능 */ 
       m_iCurTrackPosition = m_pModel->Get_CurrentAnmationTrackPosition();      
     

       if (m_iCurTrackPosition >= m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Get_Duration())
           m_iCurTrackPosition = m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Get_Duration();
     

     
       if (ImGui::SliderInt("TrackPosition", &m_iCurTrackPosition, 0, m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Get_Duration()))   
       {
           m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Set_TrackPosition(m_iCurTrackPosition, m_pModel->Get_CurrentKeyFrameIndices()[m_pModel->Get_CurrentAnimIndex()], m_pModel->Get_Bones());
           m_bKeyFrameOnOff = true;
       }

       else
           m_bKeyFrameOnOff = false;
 
       /* ======================================= */


       /* 애니메이션 속도 기능 */
       ImGui::NewLine(); 
       ImGui::Text(u8"애니메이션 속도 "); 
       ImGui::NewLine();    

       ImGui::Text("Animation 1");
    
       m_fFirstAnimationSpeed = m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Get_AnimationSpeed();
       ImGui::InputFloat(u8"애니메이션 1", &m_fFirstAnimationSpeed, 0.1f);
       m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Set_AnimationSpeed(m_fFirstAnimationSpeed); 

       ImGui::NewLine();
       ImGui::Checkbox("Change Animation", &m_bAniChangeTrackOnOff);
       ImGui::SameLine(); 
       ImGui::Dummy(ImVec2(15.f, 0.f));
       ImGui::SameLine();
       ImGui::PushItemWidth(100.0f);  // 입력 박스의 너비를 200 픽셀로 설정
       ImGui::InputInt("Change FramePosition", &m_iAniChangeTrackPosition, 1);  
       ImGui::PopItemWidth();
       ImGui::Text("Animation 2");


       m_fSecondAnimationSpeed = m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Get_AnimationSpeed();
       ImGui::InputFloat(u8"애니메이션 2", &m_fSecondAnimationSpeed, 0.1f);     
       m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Set_AnimationSpeed(m_fSecondAnimationSpeed);   

       ImGui::NewLine();
       ImGui::PushItemWidth(100.0f);  // 입력 박스의 너비를 200 픽셀로 설정
       ImGui::InputFloat("start Frame", &m_fStartFramePosition, 0.1f);
       ImGui::PopItemWidth();   
       ImGui::SameLine();
       if (ImGui::Button(u8"시작 설정", ImVec2(100.f, 30.f)))
       {
           m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Set_StartOffSetTrackPosition(m_fStartFramePosition);
           MSG_BOX("시작 프레임 설정완료");  
       }
       /* ========================== */



       /* 애니메이션 속도 기능 */
       ImGui::NewLine();
       ImGui::Text(u8"선형 보간 시간");

       ImGui::Text("Animation 1");

       m_fFirstAnimationLerpSpeed = m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Get_AnimationLerpTime();
       ImGui::InputFloat(u8"보간 속도_1", &m_fFirstAnimationLerpSpeed, 0.1f);
       m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Set_LerpTime(m_fFirstAnimationLerpSpeed);

       ImGui::Text("Animation 2");


       m_fSecondAnimationLerpSpeed = m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Get_AnimationLerpTime(); 
       ImGui::InputFloat(u8"보간 속도_2", &m_fSecondAnimationLerpSpeed, 0.1f);
       m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Set_LerpTime(m_fSecondAnimationLerpSpeed); 

       /* ========================== */

       /* 루트 기능 */
       ImGui::NewLine();
       ImGui::Checkbox(u8"루트 애니메이션", &g_bRootOnOff);

       /* 캐릭터 위치 리셋 기능 */
       ImGui::SameLine();   
       ImGui::Dummy(ImVec2(120.0f, 0.0f)); // 10픽셀 넓이의 빈 영역 생성   
       ImGui::SameLine();
       if (ImGui::Button(u8"모델 위치 리셋", ImVec2(100.f, 30.f)))    
       {
           m_pTransformCom->Set_State(CTransform::STATE_POSITION, _fvector{ 0.f,0.f,0.f,1.f }); 
       }
   
   }

}

void CImgui_Model::Weapon_Select_Function()
{
    ImGui::Text("====================================================");
    if (ImGui::RadioButton("Hand", m_iSelect_option == 1))  
        m_iSelect_option = 1;

    ImGui::SameLine();

    if (ImGui::RadioButton("Knife", m_iSelect_option == 2)) 
        m_iSelect_option = 2;   
}

void CImgui_Model::Model_EventFrame_Function()  
{
    ImGui::NewLine();   

    ImGui::Text("====================================================");
    if (ImGui::RadioButton("EVENT_SOUND", m_iSelect_option == 1))  
        m_iSelect_option = 1;       
   
    ImGui::SameLine();  

    if (ImGui::RadioButton("EVENT_EFFECT", m_iSelect_option == 2)) 
        m_iSelect_option = 2;  

    ImGui::NewLine();

    if (ImGui::RadioButton("EVENT_COLLIDER", m_iSelect_option == 3))
        m_iSelect_option = 3;

    ImGui::NewLine();

    if (ImGui::RadioButton("EVENT_STATE", m_iSelect_option == 4))   
        m_iSelect_option = 4;   

    ImGui::NewLine();

    ImGui::Text("====================================================");    


    ImGui::Text(u8"이벤트 프레임 등록하기 : ");
    ImGui::NewLine(); 
    ImGui::InputText(u8"이벤트 이름", m_szEventName, MAX_PATH);
    ImGui::NewLine();   
    ImGui::InputFloat(u8"시작 프레임", &m_fStartEventFrame, 1);  
    ImGui::SameLine();  
    ImGui::Dummy(ImVec2(20.0f, 0.0f)); // 10픽셀 넓이의 빈 영역 생성    

    ImGui::InputFloat(u8"끝 프레임", &m_fEndEventFrame, 1);      
  
    if (ImGui::Button(u8"이벤트 설정", ImVec2(100.f, 30.f)))
    {
        ANIMEVENT Event;
        
        switch (m_iSelect_option)
        {
        case 0 :
            break;
        case 1:
            Event.eType = EVENT_SOUND;
            break;
        case 2:
            Event.eType = EVENT_EFFECT;
            break;
        case 3:
            Event.eType = EVENT_COLLIDER;   
            break;
        case 4:
            Event.eType = EVENT_STATE;  
            break;  
        default:
            break;
        }
        
        Event.fStartTime = m_fStartEventFrame;
        Event.fEndTime   = m_fEndEventFrame;
        strcpy_s(Event.szName, m_szEventName);  

        if (FAILED(m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Add_EventFrame(Event)))
        {
            MSG_BOX("추가 실패");   
        }
        else
            MSG_BOX("추가 완료");   
    }
   
    ///*빼는 기능 */
    ImGui::Text("====================================================");        
    ImGui::NewLine();       

    ImGui::Text(u8"이벤트 제거하기");  
    ImGui::InputText(u8"제거할 이벤트 이름", m_szDeleteEventName, MAX_PATH);    
    ImGui::SameLine();  
    ImGui::Dummy(ImVec2(20.0f, 0.0f)); // 10픽셀 넓이의 빈 영역 생성        
    if (ImGui::Button(u8"제거하기", ImVec2(100.f, 30.f)))
    {
        m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Sub_EventFrame(m_szDeleteEventName);   
        
        MSG_BOX("제거 완료");
    }


    /* 현재 추가된 이벤트 프레임 현황 확인 기능 */
    ImGui::Text("====================================================");    
 
    vector<ANIMEVENT>* pVecEventFrame = m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Get_vecEvent();

    for (auto& iter : *pVecEventFrame)  
    {  
        ImGui::Text("%.2f", iter.fStartTime);
        ImGui::SameLine(); 
        ImGui::Text("%.2f", iter.fEndTime);   
        ImGui::SameLine();  
        ImGui::Text("%s", iter.szName);
    }
}

HRESULT CImgui_Model::Model_Save()
{
    m_pModel->Save_Model_File();    

    return S_OK;    
}

void CImgui_Model::Model_Window()   
{
    Enroll_Object(); 

#pragma region Model Tool Window 
    if (m_bModelWindowOnOff)
    {
       /* if (!m_bSystemOnOff)
            Enroll_Object();*/

        ImGui::Text(u8"모델 실시간 정보");

        if (ImGui::Begin("Model Tool Window",&m_bModelWindowOnOff))        
        {
            /* Transform 관련된 기능 */
            Transform_Method();
            /* ===================  */

            ImGui::NewLine();   

            /* 애니메이션 관련된 기능 */
            Animaiton_Function();
            /* ===================  */
        
        }

        ImGui::End(); // 창 종료를 위해 반드시 호출해야 합니다. \

        /* 모델 실시간 정보 */
        ImGui::NewLine();

        if (m_pModel != nullptr)
        {
            ImGui::Text(u8"현재 재생중인 애니메이션 이름");
            ImGui::Text(m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Get_AnimationName());

            ImGui::NewLine();

            ImGui::Text(u8"현재 재생중인 애니메이션의 속도");
            ImGui::Text("%.2f",m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Get_AnimationSpeed());
            
            ImGui::NewLine();   

            ImGui::Text(u8"현재 재생중인 애니메이션의 보간 속도");
            ImGui::Text("%.2f", m_pModel->Get_VecAnimation().at(m_pModel->Get_CurrentAnimIndex())->Get_AnimationLerpTime());   
            

            /* 애니메이션 키프레임에 따른 속도 기능 */
            ImGui::NewLine();
            ImGui::Text(u8"KeyFrame속도 ");

            ImGui::Text("Animation 1");
            ImGui::SameLine();
            ImGui::Text(u8"애니메이션 총 길이 :");
            ImGui::SameLine();
            ImGui::Text("%.2f",m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Get_Duration());       

            m_fFirstAnimation_KeyFrameSpeed = m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Get_CurrentKeySpeed();
            ImGui::Text("%.2f", m_fFirstAnimation_KeyFrameSpeed);   
            ImGui::InputFloat(u8"키프레임 속도_1", &m_fFirstAnimation_KeySpeed, 0.1f);     
            /*범위를 제공 해야함 */
            ImGui::NewLine();
            ImGui::PushItemWidth(120.0f);  // 입력 박스의 너비를 200 픽셀로 설정
            ImGui::InputInt("start1", &m_iFirstAnimationStartKeyIndex, 1);
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(20.0f, 0.0f)); // 10픽셀 넓이의 빈 영역 생성  
            ImGui::SameLine();
            ImGui::InputInt("end1", &m_iFirstAnimationSecondKeyIndex, 1);

            ImGui::NewLine();   
            ImGui::Dummy(ImVec2(225.0f, 0.0f)); 
            ImGui::SameLine();      
            if(ImGui::Button(u8"설정1", ImVec2(100.f, 25.f)))
            {
                m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Set_TrackKeySpeed(m_iFirstAnimationStartKeyIndex, m_iFirstAnimationSecondKeyIndex, m_fFirstAnimation_KeySpeed);
                MSG_BOX("설정완료");
            }
      
            ImGui::PopItemWidth();
            //m_pModel->Get_VecAnimation().at(m_iCurSelectAnimationIndex)->Set_LerpTime(m_fFirstAnimationLerpSpeed);   

            ImGui::NewLine();       
            ImGui::Text("Animation 2"); 
            ImGui::SameLine();  
            ImGui::Text(u8"애니메이션 총 길이 :");      
            ImGui::SameLine();  
            ImGui::Text("%.2f", m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Get_Duration());       


            m_fSecondAnimation_KeyFrameSpeed = m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Get_CurrentKeySpeed();
            ImGui::Text("%.2f", m_fSecondAnimation_KeyFrameSpeed);  
            ImGui::InputFloat(u8"키프레임 속도_2", &m_fSecondAnimation_KeySpeed, 0.1f);   
            //m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Set_LerpTime(m_fSecondAnimation_KeySpeed);

            ImGui::NewLine();       
            ImGui::PushItemWidth(120.0f);  // 입력 박스의 너비를 200 픽셀로 설정 
            ImGui::InputInt("start2", &m_iSecondAnimationStartKeyIndex, 1);   
            ImGui::SameLine();  
            ImGui::Dummy(ImVec2(20.0f, 0.0f)); // 10픽셀 넓이의 빈 영역 생성      
            ImGui::SameLine();  
            ImGui::InputInt("end2", &m_iSecondAnimationSecondKeyIndex, 1);   

            ImGui::NewLine();   
            ImGui::Dummy(ImVec2(225.0f, 0.0f)); 
            ImGui::SameLine();  
            if (ImGui::Button(u8"설정2", ImVec2(100.f, 25.f))) 
            {
                m_pModel->Get_VecAnimation().at(m_iNextSelectAnimationIndex)->Set_TrackKeySpeed(m_iSecondAnimationStartKeyIndex, m_iSecondAnimationSecondKeyIndex, m_fSecondAnimation_KeySpeed);
                MSG_BOX("설정완료");    
            }   


            ImGui::PopItemWidth();

            /* =================================  */

        }
    }
#pragma endregion 
#pragma region Model Weapon Window 
    if (m_bModelWeaponWindow)
    {
        if (ImGui::Begin("Model Weapon Window", &m_bModelWeaponWindow))
        {
            ImGui::Text("Weapon");  
            Weapon_Select_Function(); 
        }

        ImGui::End();
    }
#pragma endregion 



#pragma region Model Event Frame  Window 
    if (m_bEventFrameWindow)
    {
        if (ImGui::Begin("Model Weapon Window", &m_bEventFrameWindow))  
        {
            ImGui::Text("Event Frame");   
            Model_EventFrame_Function();
        }

        ImGui::End();
    }
#pragma endregion 
}

CImgui_Model* CImgui_Model::Create()
{
    CImgui_Model* pInstance = new CImgui_Model();

    if(FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : Imugi_Model"); 
        Safe_Release(pInstance);
    }

    return pInstance;   
}

void CImgui_Model::Free()
{
    __super::Free(); 


    Safe_Release(m_pGameInstance);  
}
