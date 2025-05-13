#pragma once

#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)
class CGameInstance;

class CEventMgr : public CBase
{
private:
	CEventMgr(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CEventMgr() = default;

private:
	HRESULT  Initialize();

public:
	/* 객체의 삭제와 생성과 관련된 함수들 */
	HRESULT Delete_All_In_Layer(_uint iLevelIndex,const _wstring& strLayerTag);	

private:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };
	CGameInstance* m_pGameInstance = { nullptr };

private:
	map<_wstring, CGameObject*>    m_mapDeadObejct;

public:
	static CEventMgr* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;

};

END