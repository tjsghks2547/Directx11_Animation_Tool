#include "pch.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Rect.h"
#include "FreeCamera.h"
#include "Terrain.h"
#include "Monster.h"


#pragma region 캐릭터 관련 요소 
#include "Player.h"
#include "Body_Player.h"
#include "LeftWeapon.h"
#include "RightWeapon.h"
#pragma endregion 


#pragma region 보스 바그 관련 
#include "Boss_Varg.h"
#include "Body_Varg.h"

#pragma region 

#pragma region 카메라 관련 요소 
#include "FreeCamera.h"
#include "CharacterCamera.h"
#pragma endregion 


#pragma region 카메라 모델
#include "CameraModel.h"	
#pragma endregion 

#pragma region 큐브 모델 
#include "CubeObject.h"	
#pragma endregion 



#include "Terrain.h"




CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pGameInstance);
}

_uint APIENTRY Thread_Main(void* pArg)
{
	CLoader*		pLoader = static_cast<CLoader*>(pArg);

	pLoader->Start_Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevelID)
{


	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);

	// m_eNextLevelID가 필요로하는 자원을 로드해주기위한 스레드를 만들어낸다. 
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, Thread_Main, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT CLoader::Start_Loading()
{
	EnterCriticalSection(&m_CriticalSection);

	CoInitializeEx(nullptr, 0);

	HRESULT			hr = {};

	switch (m_eNextLevelID)
	{
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_For_Level_GamePlay();
		break;
	}

	if (FAILED(hr))
		return E_FAIL;

	LeaveCriticalSection(&m_CriticalSection);

	return S_OK;
}

void CLoader::SetUp_WindowText()
{
	SetWindowText(g_hWnd, m_szLoadingText);
}

HRESULT CLoader::Loading_For_Level_Logo()
{
#pragma region 게임오브젝트 원형들 

	/* LOGO LEVEL 을 위한 자원을 로드합니다.  */

	lstrcpyW(m_szLoadingText, TEXT("로고레벨을 위한 자원을 로딩 중입니다."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
#pragma endregion
#pragma region 컴포넌트 원형들

	/* LOGO LEVEL 을 위한 자원을 로드합니다.  */

	lstrcpyW(m_szLoadingText, TEXT("사각형 원형을 생성한다"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
#pragma endregion
#pragma region 쉐이더 

	lstrcpyW(m_szLoadingText, TEXT("사각형의 셰이더를 생성한다"));

	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO,TEXT("Prototype_Component_VIBuffer_Rect_Shader")
		,CShader::Create(m_pDevice,m_pContext,TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"),VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	//여기서부터 다시 작성하기. 

#pragma endregion
#pragma region 텍스처 

	lstrcpyW(m_szLoadingText, TEXT("텍스처를 생성한다"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Splash.bmp"), 1))))
		return E_FAIL;


#pragma endregion
	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));
	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* 게임플레이레벨을 위한 자원을 로드합니다.  */
	lstrcpyW(m_szLoadingText, TEXT("게임플레이레벨을 위한 자원을 로딩 중입니다."));		
	_matrix PreTransformMatrix = XMMatrixIdentity();
	


#pragma region 보스 바그

	lstrcpyW(m_szLoadingText, TEXT("보스 바그 모델을 생성한다."));
	/* For.Prototype_Component_Model_Kaku*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Boss_Varg"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Boss/Boss_Varg/Boss_Varg.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;
	
	///* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Varg"),	
		CBody_Varg::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Boss_Varg"),	
		CBoss_Varg::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion 




#pragma region 티메시아 캐릭터 
	lstrcpyW(m_szLoadingText, TEXT("티메시아 모델을 생성한다."));
	/* For.Prototype_Component_Model_Kaku*/
	PreTransformMatrix = /*XMMatrixScaling(0.015f, 0.015f, 0.015f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corner"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus/Corvus.fbx", CModel::MODEL_ANIM, PreTransformMatrix))))
		return E_FAIL;
	
	///* For.Prototype_GameObject_Body_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Body_Player"),
		CBody_Player::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
	
	///* For.Prototype_GameObject_Player */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Corvus"),	
		CPlayer::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion	
#pragma region 플레이어 자벨린 무기			
	lstrcpyW(m_szLoadingText, TEXT("자벨린 무기 모델을 생성한다."));			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Javelin_Sword"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Javelin_Sword/Weapon_Javelin_Sword.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


#pragma region 티메시아 캐릭터 오른손 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("주인공 오른손 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Right_Weapon/Corvus_Right_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Weapon"),
		CRightWeapon::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 


#pragma region 티메시아 캐릭터 왼손 무기 모델 
	lstrcpyW(m_szLoadingText, TEXT("주인공 왼손 무기 모델을 생성한다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Left_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Left_Weapon/Corvus_Left_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Left_Weapon"),
		CLeftWeapon::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
#pragma endregion 


#pragma region 지형

	lstrcpyW(m_szLoadingText, TEXT("지형 생성"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

#pragma endregion 


#pragma region 카메라 
	lstrcpyW(m_szLoadingText, TEXT("카메라 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FreeCamera")	
		, CFreeCamera::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	


	lstrcpyW(m_szLoadingText, TEXT("카메라 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CharacterCamera")
		, CCharacterCamera::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 

#pragma region 카메라 모델 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CameraModel"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Camera/activities_Camera.fbx", CModel::MODEL_NONANIM))))	
		return E_FAIL;	

	lstrcpyW(m_szLoadingText, TEXT("카메라 게임 오브젝트 생성"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CameraModel")		
		, CCameraModel::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 

#pragma region 큐브 모델 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CubeObject"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Cube/cube.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("큐브 오브젝트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeObject")				
		,CCubeObject::Create(m_pDevice, m_pContext))))			
		return E_FAIL;		
#pragma endregion	



#pragma region 컴포넌트 원형들 

	lstrcpyW(m_szLoadingText, TEXT("사각형 원형을 생성한다"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("터레인 컴포넌트 생성"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 50, 50, 1, nullptr))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("사각형 텍스쿠드2개 원형을 생성한다"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Texcood2")
		, CVIBuffer_Rect_Texcood2::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion
#pragma region 셰이더

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Shader")
		, CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosNorTex.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Shader_Brush")
		, CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::Elements, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMMESH::Elements, VTXANIMMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Water */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_Water"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Water.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements))))
		return E_FAIL;
#pragma endregion
#pragma region 텍스처
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Water_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Water/Water.dds"), 1))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Altar_Base_Texture"),	
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Temple/AltarBase.dds"), 1))))	
	//	return E_FAIL;	



#pragma region Collider (충돌체)

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

#pragma endregion 


#pragma region 큐브 컴포넌트
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))	
		return E_FAIL;	

	
#pragma endregion 


#pragma endregion

	/* 로딩이 완료되었습ㄴ미다 */
	lstrcpyW(m_szLoadingText, TEXT("로딩끝."));	
	m_isFinished = true;
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevelID)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);

	DeleteObject(m_hThread);

	CloseHandle(m_hThread);

	DeleteCriticalSection(&m_CriticalSection);

	Safe_Release(m_pGameInstance);	
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
