#include "pch.h"
#include "Player.h"
#include "Body_Player.h" 
#include "GameInstance.h"
#include "Animation.h"


CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    : CPartObject(Prototype)
{
}

HRESULT CBody_Player::Initialize_Prototype()
{
    if (FAILED(__super::Initialize_Prototype()))
        return E_FAIL;

    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    CBody_Player::BODY_PLAYER_DESC* pDesc = static_cast<CBody_Player::BODY_PLAYER_DESC*>(pArg);

    m_pParentState = pDesc->pParentState;
    m_pParentPhsaeState = pDesc->pParentPhaseState;

    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_pModelCom->SetUp_Animation(0, false);

    // m_pTransformCom->Set_State(CTransform::STATE_POSITION, XMVectorSet(1.f, 0.f, 0.f, 1.f));
    //m_pTransformCom->Scaling(_float3(0.1f, 0.1f, 0.1f));
    

    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{

}

void CBody_Player::Update(_float fTimeDelta)
{
//#pragma region  각 상황에 따른 함수 
//    switch (*m_pParentState)        
//    {
//    case STATE_IDLE:    
//        STATE_IDLE_Method();    
//        break;
//    case STATE_RUN: 
//        STATE_RUN_Method(); 
//        break;
//    case STATE_WALK:    
//        STATE_WALK_Method();    
//        break;
//    case STATE_ATTACK:  
//        STATE_ATTACK_Method();  
//        break;
//    case STATE_ATTACK_L1:
//        STATE_ATTACK_L1_Method();
//        break;
//    case STATE_ATTACK_L2:
//        STATE_ATTACK_L2_Method();
//        break;
//    case STATE_ATTACK_L3:
//        STATE_ATTACK_L3_Method();
//        break;
//    case STATE_ATTACK_LONG_CLAW_01:
//        STATE_ATTACK_LONG_CLAW_01_Method();
//        break;
//    case STATE_ATTACK_LONG_CLAW_02: 
//        STATE_ATTACK_LONG_CLAW_02_Method(); 
//        break;  
//    default:
//        break;
//    }
//#pragma endregion 
//
//    m_pModelCom->Play_Animation(fTimeDelta * m_fAnimSpeed);
//
//    if(m_pModelCom->Get_VecAnimation().at(0)->Get_CurrentEventFrame() == 10)        
//    {
//        int a = 4;  
//    }   



    /* 3월 6일 추가 작업 및  이 방향으로 아이디어 나가기 */
    for(auto& iter : *m_pModelCom->Get_VecAnimation().at(m_pModelCom->Get_CurrentAnimIndex())->Get_vecEvent())
    {
        if(iter.isPlay == false)    
        {
            if(iter.eType == EVENT_COLLIDER && iter.isEventActivate == true) // EVENT_COLLIDER 부분 
            {
                // 그 구간에서는 계속 진행 
            }

            else if(iter.isEventActivate == true && iter.isPlay == false)  // 여기가 EVENT_EFFECT, EVENT_SOUND, EVENT_STATE 부분 
            {
                iter.isPlay = true;      // 한 번만 재생 되어야 하므로 
            }
        }
    }
    
    XMStoreFloat4x4(&m_CombinedWorldMatrix, XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentWorldMatrix));

  
}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(CRenderer::RG_NONBLEND, this);
}

HRESULT CBody_Player::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    _uint			iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {


        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_DIFFUSE, "g_DiffuseTexture", 0)))
            return E_FAIL;
        /*if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, i, aiTextureType_NORMALS, "g_NormalTexture", 0)))
            return E_FAIL;*/

        if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, i, "g_BoneMatrices")))
            return E_FAIL;

        m_pShaderCom->Begin(0);
        m_pModelCom->Render(i);
    }

    return S_OK;
}

void CBody_Player::STATE_IDLE_Method()
{
    m_pModelCom->SetUp_Animation(2, true);
}

void CBody_Player::STATE_RUN_Method()
{
    m_pModelCom->SetUp_Animation(6, true);
}

void CBody_Player::STATE_WALK_Method()
{
}

void CBody_Player::STATE_ATTACK_Method()
{
}

void CBody_Player::STATE_ATTACK_L1_Method()
{
    m_pModelCom->SetUp_Animation(3, false);

    if (*m_pParentState == STATE_ATTACK_L1 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 60.f)
    {
        *m_pParentPhsaeState = CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L1 && m_pModelCom->Get_VecAnimation().at(3)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_ATTACK_L2_Method() 
{
    m_pModelCom->SetUp_Animation(4, false);

    if (*m_pParentState == STATE_ATTACK_L2 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 90.f)
    {
        *m_pParentPhsaeState = CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L2 && m_pModelCom->Get_VecAnimation().at(4)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_ATTACK_L3_Method() 
{
    m_pModelCom->SetUp_Animation(5, false);

    if (*m_pParentState == STATE_ATTACK_L3 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 125.f)
    {
        *m_pParentPhsaeState = CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_L3 && m_pModelCom->Get_VecAnimation().at(5)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_ATTACK_LONG_CLAW_01_Method()   
{
    m_pModelCom->SetUp_Animation(0, false);

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)
    {
        *m_pParentPhsaeState = CPlayer::PHASE_IDLE;
    }

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_01 && m_pModelCom->Get_VecAnimation().at(0)->isAniMationFinish())
    {
        *m_pParentState = STATE_IDLE;
    }
}

void CBody_Player::STATE_ATTACK_LONG_CLAW_02_Method()   
{
    m_pModelCom->SetUp_Animation(1, false); 

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_CurrentAnmationTrackPosition() > 140.f)    
    {
        *m_pParentPhsaeState = CPlayer::PHASE_IDLE; 
    }

    if (*m_pParentState == STATE_ATTACK_LONG_CLAW_02 && m_pModelCom->Get_VecAnimation().at(1)->isAniMationFinish()) 
    {
        *m_pParentState = STATE_IDLE;   
    }

}

HRESULT CBody_Player::Ready_Components()
{
    /* Com_Shader */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom))))
        return E_FAIL;

    /* Com_Model */
    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corner"),
        TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom))))
        return E_FAIL;


    return S_OK;
}

HRESULT CBody_Player::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_VIEW))))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_float4x4(CPipeLine::D3DTS_PROJ))))
        return E_FAIL;

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    if (nullptr == pLightDesc)
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", &m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
        return E_FAIL;

    return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : CBody_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);

}
