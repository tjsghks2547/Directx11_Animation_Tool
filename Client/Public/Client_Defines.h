#pragma once

#include <process.h>

namespace Client 
{
	const unsigned int		g_iWinSizeX = 1600;
	const unsigned int		g_iWinSizeY = 900;

	enum LEVELID { LEVEL_STATIC, LEVEL_LOADING, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_END };

}



using namespace Client;

extern HINSTANCE g_hInst;
extern HWND g_hWnd;
extern WCHAR g_CurrentFbxFileName[MAX_PATH];	
extern bool  g_bFbxFileDragOnOff;	
extern bool  g_bRootOnOff;	

