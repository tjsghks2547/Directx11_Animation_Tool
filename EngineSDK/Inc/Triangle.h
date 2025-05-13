#pragma once

#include "VIBuffer.h"

BEGIN(Engine)
class ENGINE_DLL CTriangle final : public CVIBuffer
{
private:
	CTriangle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CTriangle(const CTriangle& Prototype);
	~CTriangle() = default;

public:

	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:


public:
	static CTriangle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};
END	
