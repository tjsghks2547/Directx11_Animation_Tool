#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Texcood2 : public CVIBuffer
{
private:
	CVIBuffer_Rect_Texcood2(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Texcood2(const CVIBuffer_Rect_Texcood2& Prototype);
	virtual ~CVIBuffer_Rect_Texcood2() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;


public:
	static CVIBuffer_Rect_Texcood2* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END