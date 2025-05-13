#ifndef Engine_Struct_h__
#define Engine_Struct_h__

namespace Engine
{
	typedef struct
	{
		HWND			hWnd;
		unsigned int	iNumLevels;
		bool			isWindowed;
		unsigned int	iViewportWidth;
		unsigned int	iViewportHeight;

	}ENGINE_DESC;

	struct ENGINE_DLL VTXPOS	
	{
		XMFLOAT3		vPosition;

		const static unsigned int					iNumElements = 1;	
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];		
	};

	struct ENGINE_DLL VTXPOSTEX
	{
		XMFLOAT3      vPosition;
		XMFLOAT2      vTexcoord;

		static const unsigned int				iNumElements = { 2 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[2];

	};

	struct ENGINE_DLL VTXPOSTEX2			
	{
		XMFLOAT3      vPosition;	
		XMFLOAT2      vTexcoord;
		XMFLOAT2      vTexcoord2;

		static const unsigned int				iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC	Elements[3];

	};	

	
	struct ENGINE_DLL VTXCUBE	
	{
		XMFLOAT3		vPosition;	

		const static unsigned int					iNumElements = 1;		
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];	
	};	


	struct ENGINE_DLL VTXNORTEX
	{
		XMFLOAT3    vPosition;
		XMFLOAT3    vNormal; 
		XMFLOAT2    vTexcoord;

		/* 이 밑에 static 변수는 이 구조체 크기에 포함안됨 왜냐하면
		정적 변수는 컴파일때 메모리가 미리 할당되므로 이 구조체 크기와 관련 x*/
		static const unsigned int iNumElements = { 3 };
		static const D3D11_INPUT_ELEMENT_DESC  Elements[3];
	};

	struct ENGINE_DLL LIGHT_DESC
	{
		enum TYPE {TYPE_DIRECTIONAL, TYPE_POINT, TYPE_END };

		TYPE eType = { TYPE_END };

		XMFLOAT4 vDirection;
		XMFLOAT4 vPosition;
		float    fRange;

		/* 빛의 기본적인 색상*/
		XMFLOAT4 vDiffuse;
		XMFLOAT4 vAmbient; 
		XMFLOAT4 vSpecular;
	};

	struct ENGINE_DLL VTXTEX_DECLARATION
	{
		/* static 변수는 구조체 메모리에 포함 x (컴파일이시 메모리가 할당되므로)*/
		static const unsigned int iNumElements = 2;
		static const D3D11_INPUT_ELEMENT_DESC Elements[iNumElements];
	};


	struct ENGINE_DLL VTXMESH
	{
		XMFLOAT3		vPosition;
		XMFLOAT3		vNormal;
		XMFLOAT2		vTexcoord;
		XMFLOAT3		vTangent;

		const static unsigned int					iNumElements = 4;
		const static D3D11_INPUT_ELEMENT_DESC		Elements[iNumElements];
	};


	struct ENGINE_DLL VTXANIMMESH
	{
		XMFLOAT3        vPosition; 
		XMFLOAT3        vNormal;
		XMFLOAT2        vTexcoord;
		XMFLOAT3        vTangent;

		/* 이 정점이 어떤 뼈를 따라가야하는지에 대한 뼈 인덱스 (최대 4개) */
		XMUINT4			vBlendIndex;
		
		/* 각 뼈의 상태를 얼마나 적용해야 하는가? */
		/* x + y + z + w  = 1 */
		XMFLOAT4		vBlendWeight;

		const static unsigned int				iNumElements = 6; 
		const static D3D11_INPUT_ELEMENT_DESC   Elements[iNumElements];

	};

	struct KEYFRAME
	{
		XMFLOAT3  vScale;
		XMFLOAT4  vRotation;
		XMFLOAT3  vTranslation;

		float     fTrackPosition;
	};


	struct ENGINE_DLL ANIMEVENT	
	{
		_bool isEventActivate = false;
		_bool isPlay = false;
		_float fStartTime;
		_float fEndTime;
		EVENT_FRAME_TYPE eType;
		char szName[MAX_PATH];	
	};

	
	struct ENGINE_DLL Camera_Event
	{
		char szName[MAX_PATH];
		_float    LerpTime;	
		_float    LerpTimeSpeed; 
		_float4x4 CamWorldMatrix;	
		_uint     LerpMethod;
	};


}


#endif // Engine_Struct_h__
