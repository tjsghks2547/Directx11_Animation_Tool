#include "pch.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "BackGround.h"
#include "Rect.h"
#include "FreeCamera.h"
#include "Terrain.h"
#include "Monster.h"


#pragma region ĳ���� ���� ��� 
#include "Player.h"
#include "Body_Player.h"
#include "LeftWeapon.h"
#include "RightWeapon.h"
#pragma endregion 


#pragma region ���� �ٱ� ���� 
#include "Boss_Varg.h"
#include "Body_Varg.h"

#pragma region 

#pragma region ī�޶� ���� ��� 
#include "FreeCamera.h"
#include "CharacterCamera.h"
#pragma endregion 


#pragma region ī�޶� ��
#include "CameraModel.h"	
#pragma endregion 

#pragma region ť�� �� 
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

	// m_eNextLevelID�� �ʿ���ϴ� �ڿ��� �ε����ֱ����� �����带 ������. 
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
#pragma region ���ӿ�����Ʈ ������ 

	/* LOGO LEVEL �� ���� �ڿ��� �ε��մϴ�.  */

	lstrcpyW(m_szLoadingText, TEXT("�ΰ����� ���� �ڿ��� �ε� ���Դϴ�."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		CBackGround::Create(m_pDevice, m_pContext))))
		return E_FAIL;	
#pragma endregion
#pragma region ������Ʈ ������

	/* LOGO LEVEL �� ���� �ڿ��� �ε��մϴ�.  */

	lstrcpyW(m_szLoadingText, TEXT("�簢�� ������ �����Ѵ�"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
#pragma endregion
#pragma region ���̴� 

	lstrcpyW(m_szLoadingText, TEXT("�簢���� ���̴��� �����Ѵ�"));

	if(FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO,TEXT("Prototype_Component_VIBuffer_Rect_Shader")
		,CShader::Create(m_pDevice,m_pContext,TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"),VTXPOSTEX::Elements, VTXPOSTEX::iNumElements))))
		return E_FAIL;
	//���⼭���� �ٽ� �ۼ��ϱ�. 

#pragma endregion
#pragma region �ؽ�ó 

	lstrcpyW(m_szLoadingText, TEXT("�ؽ�ó�� �����Ѵ�"));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Splash.bmp"), 1))))
		return E_FAIL;


#pragma endregion
	/* �ε��� �Ϸ�Ǿ������̴� */
	lstrcpyW(m_szLoadingText, TEXT("�ε���."));
	m_isFinished = true;

	return S_OK;
}


HRESULT CLoader::Loading_For_Level_GamePlay()
{
	/* �����÷��̷����� ���� �ڿ��� �ε��մϴ�.  */
	lstrcpyW(m_szLoadingText, TEXT("�����÷��̷����� ���� �ڿ��� �ε� ���Դϴ�."));		
	_matrix PreTransformMatrix = XMMatrixIdentity();
	


#pragma region ���� �ٱ�

	lstrcpyW(m_szLoadingText, TEXT("���� �ٱ� ���� �����Ѵ�."));
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




#pragma region Ƽ�޽þ� ĳ���� 
	lstrcpyW(m_szLoadingText, TEXT("Ƽ�޽þ� ���� �����Ѵ�."));
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
#pragma region �÷��̾� �ں��� ����			
	lstrcpyW(m_szLoadingText, TEXT("�ں��� ���� ���� �����Ѵ�."));			
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Javelin_Sword"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Javelin_Sword/Weapon_Javelin_Sword.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


#pragma region Ƽ�޽þ� ĳ���� ������ ���� �� 
	lstrcpyW(m_szLoadingText, TEXT("���ΰ� ������ ���� ���� �����Ѵ�."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Right_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Right_Weapon/Corvus_Right_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Right_Weapon"),
		CRightWeapon::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 


#pragma region Ƽ�޽þ� ĳ���� �޼� ���� �� 
	lstrcpyW(m_szLoadingText, TEXT("���ΰ� �޼� ���� ���� �����Ѵ�."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Corvus_Left_Weapon"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Corvus_Left_Weapon/Corvus_Left_Weapon.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;


	///* For.Prototype_GameObject_Weapon */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Left_Weapon"),
		CLeftWeapon::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	
#pragma endregion 


#pragma region ����

	lstrcpyW(m_szLoadingText, TEXT("���� ����"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),	
		CTerrain::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	

#pragma endregion 


#pragma region ī�޶� 
	lstrcpyW(m_szLoadingText, TEXT("ī�޶� ����"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_FreeCamera")	
		, CFreeCamera::Create(m_pDevice, m_pContext))))	
		return E_FAIL;	


	lstrcpyW(m_szLoadingText, TEXT("ī�޶� ����"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CharacterCamera")
		, CCharacterCamera::Create(m_pDevice, m_pContext))))	
		return E_FAIL;
#pragma endregion 

#pragma region ī�޶� �� 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CameraModel"),	
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Camera/activities_Camera.fbx", CModel::MODEL_NONANIM))))	
		return E_FAIL;	

	lstrcpyW(m_szLoadingText, TEXT("ī�޶� ���� ������Ʈ ����"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CameraModel")		
		, CCameraModel::Create(m_pDevice, m_pContext))))		
		return E_FAIL;	

#pragma endregion 

#pragma region ť�� �� 
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_CubeObject"),
		CModel::Create(m_pDevice, m_pContext, "../Bin/Resources/Models/Cube/cube.fbx", CModel::MODEL_NONANIM))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("ť�� ������Ʈ ����"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_CubeObject")				
		,CCubeObject::Create(m_pDevice, m_pContext))))			
		return E_FAIL;		
#pragma endregion	



#pragma region ������Ʈ ������ 

	lstrcpyW(m_szLoadingText, TEXT("�簢�� ������ �����Ѵ�"));	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect")
		, CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpyW(m_szLoadingText, TEXT("�ͷ��� ������Ʈ ����"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain")
		, CVIBuffer_Terrain::Create(m_pDevice, m_pContext, 50, 50, 1, nullptr))))
		return E_FAIL;


	lstrcpyW(m_szLoadingText, TEXT("�簢�� �ؽ����2�� ������ �����Ѵ�"));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Rect_Texcood2")
		, CVIBuffer_Rect_Texcood2::Create(m_pDevice, m_pContext))))	
		return E_FAIL;

#pragma endregion
#pragma region ���̴�

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
#pragma region �ؽ�ó
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds"), 1))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Water_Texture"),
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Water/Water.dds"), 1))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Altar_Base_Texture"),	
	//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Temple/AltarBase.dds"), 1))))	
	//	return E_FAIL;	



#pragma region Collider (�浹ü)

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Collider_Sphere"),
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;

#pragma endregion 


#pragma region ť�� ������Ʈ
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxCube"),	
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::Elements, VTXCUBE::iNumElements))))	
		return E_FAIL;	

	
#pragma endregion 


#pragma endregion

	/* �ε��� �Ϸ�Ǿ������̴� */
	lstrcpyW(m_szLoadingText, TEXT("�ε���."));	
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
