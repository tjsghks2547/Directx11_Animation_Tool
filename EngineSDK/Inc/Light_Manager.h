#pragma once

#include "Base.h"

BEGIN(Engine)
class CShader;
class CVIBuffer_Rect;
class CLight;

class CLight_Manager : public CBase
{
private:
	CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const LIGHT_DESC* Get_LightDesc(_uint iIndex) const;


public:
	HRESULT Initialize();
	HRESULT Add_Light(const LIGHT_DESC& LightDesc);
	HRESULT Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer);

private:
	list<CLight*> m_Lights;

public:
	static CLight_Manager* Create();
	virtual void Free() override;	

};

END
