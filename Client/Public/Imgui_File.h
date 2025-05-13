#pragma once
#include "Base.h"
#include "GameInstance.h"
#include "GameObject.h"


class CImgui_File : public CBase	
{
private:
	CImgui_File();
	virtual ~CImgui_File() = default;

public:
	HRESULT Init();
	HRESULT Update();


private:
	HRESULT ModelFile_Read();	
	HRESULT ModelFile_Save();	

private:
	map<const _wstring, class CLayer*>* m_pLayer = { nullptr };	

private:
	bool			m_bHeightMapOpenMenu = { false };
	bool			m_bHeightMapSaveMenu = { false };	

	bool			m_bMapOpenMenu = { false };
	bool			m_bMapSaveMenu = { false };

	bool			m_bNavigationOpenMenu = { false };	
	bool			m_bNavigationSaveMenu = { false };	

	CGameInstance*	m_pGameInstance = { nullptr };
	CGameObject*	m_pGameObject   = { nullptr };
	
	ID3D11Device*			 m_pDevice  = { nullptr };
	ID3D11DeviceContext*	 m_pContext = { nullptr };

	

public:
	static CImgui_File* Create();
	void Free();	 

};

