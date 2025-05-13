#pragma once

#include "Renderer.h"
#include "Prototype_Manager.h"
#include "PipeLine.h"


/* 0. 엔진에서 클라이언트 프로젝트에 직접 보여주는 클래스 : CGameInstance, 부모클래스(CBase, CLevel, CGameObject, CComponent..) */
/* 1. 클라이언트 프로젝트에서 엔진프로젝트에 있는 기능을 호출하고자한다라면 무조건 GameInstance를 통해서 호출할 수 있도록 하겠다.  */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	_uint2 Get_ViewportSize() const {
		return _uint2(m_iViewportWidth, m_iViewportHeight);
	}


public:
	/* 클라에서 호출할 만한 엔진의 기능을 위해 필요한 초기화를 수행한다. */
	HRESULT Initialize_Engine(HINSTANCE hInst,const ENGINE_DESC& EngineDesc, _Inout_ ID3D11Device** ppDevice, _Inout_ ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	HRESULT Render_Begin(const _float4 & vClearColor);	
	HRESULT Draw();
	HRESULT Render_End();
	void Clear(_uint iLevelIndex);

#pragma region Graphic_Device
public:
	ID3D11RenderTargetView* Get_RTV_Textrue();

#pragma region TIMER_MANAGER
public: /* For.Timer_Manager */
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
	void			Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Open_Level(_uint iNewLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype);
	CBase* Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	map<const _wstring, CBase*>* Get_Prototypes_Map();
#pragma endregion

#pragma region OBJECT_MANAGER
	HRESULT Add_GameObject_To_Layer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	HRESULT Sub_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, const _char* ObjectName);
	CComponent* Find_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentName);
	map<const _wstring, class CLayer*>* Get_Layers();
	_uint	Get_NumLevel();
	list<class CGameObject*>* Get_Layer_List(_uint iLevelIndex, const _wstring& strLayerTag);	
	CGameObject* Get_GameObject_To_Layer(_uint iLevelIndex, const _wstring& strLayerTag, _char* ObjectName);


#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(CRenderer::RENDERGROUP eRenderGroupID, class CGameObject* pGameObject);
#pragma endregion

#pragma region PipeLine
	const _float4x4* Get_Transform_float4x4(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix(CPipeLine::D3DTRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4_Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix_Inverse(CPipeLine::D3DTRANSFORMSTATE eState);
	const _float4* Get_CamPosition_float4();
	_vector Get_CamPosition();
	void Set_Transform(CPipeLine::D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix);

#pragma endregion

#pragma region Input_Device 

	_byte	Get_DIKeyState(_ubyte byKeyID);
	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

	_bool   isKeyEnter(_int _iKey);
	_bool	isKeyReleased(_int _iKey);
	_bool   isKeyPressed(_int _iKey);

	_bool   isMouseEnter(_uint _iKey);
	_bool   isMouseRelease();

#pragma endregion
#pragma region Calculator
	_vector Get_RayPos();
	_vector Get_RayDir();
	void Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, class CTransform* pTransform);
	void Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, XMMATRIX WorldInvers);
	POINT Get_Mouse_Point();	

	_float3 Get_WorldRayPos();
	_float3 Get_WorldRayDir();

	_float3 Compute_PickingPoint(_float3 _point1, _float3 _point2, _float3 _point3);
#pragma endregion

#pragma region Light_Manager
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#pragma endregion


#pragma region Font_Manager
	HRESULT Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);	
	HRESULT Render_Font(const wstring& strFontTag, const _tchar* pText, const _float2& vPosition,
		_fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRotation = 0.f,
		const _float2& vOrigin = _float2(0.f, 0.f), const _float fScale = 1.f);
#pragma endregion



#pragma region Target_Manager
	HRESULT Add_RenderTarget(const wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Begin_MRT(const wstring& strMRTTag, _bool isClear = true);
	HRESULT Begin_MRT(const wstring& strMRTTag, ID3D11DepthStencilView* pDSView = nullptr);
	HRESULT End_MRT();
	HRESULT Bind_RenderTargetSRV(const wstring& strTargetTag, class CShader* pShader, const _char* pConstantName);
	HRESULT Copy_Resource(const wstring& strTargetTag, ID3D11Texture2D* pDesc);
#ifdef _DEBUG
	HRESULT Ready_RTDebug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render_RTDebug(const wstring& strMRTTag, class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif
#pragma endregion

private:
	_uint								m_iViewportWidth{}, m_iViewportHeight{};

private:
	class CTimer_Manager*				m_pTimer_Manager = { nullptr };
	class CGraphic_Device*				m_pGraphic_Device = { nullptr };
	class CLevel_Manager*				m_pLevel_Manager = { nullptr };
	class CObject_Manager*				m_pObject_Manager = { nullptr };
	class CPrototype_Manager*			m_pPrototype_Manager = { nullptr };
	class CRenderer*					m_pRenderer = { nullptr };
	class CPipeLine*					m_pPipeLine = { nullptr };
	class CInput_Device*				m_pInput_Device = { nullptr };
	class CCalculator*					m_pCalculator = { nullptr };
	class CLight_Manager*				m_pLight_Manager = { nullptr };
	class CTarget_Manager*				m_pTarget_Manager = { nullptr };
	class CFont_Manager*				m_pFont_Manager = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
	
};

END