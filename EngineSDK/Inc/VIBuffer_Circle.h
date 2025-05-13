#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Circle final : public CVIBuffer
{
private:
	CVIBuffer_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Circle(const CVIBuffer_Circle& Prototype);
	virtual ~CVIBuffer_Circle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	


public:
	static CVIBuffer_Circle* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override; 
	virtual void Free() override; 
};

END