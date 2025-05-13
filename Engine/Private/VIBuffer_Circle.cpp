#include "VIBuffer_Circle.h"

CVIBuffer_Circle::CVIBuffer_Circle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer(pDevice,pContext)
{

}

CVIBuffer_Circle::CVIBuffer_Circle(const CVIBuffer_Circle& Prototype)
	:CVIBuffer(Prototype)
{

}

HRESULT CVIBuffer_Circle::Initialize_Prototype()
{
	
	return S_OK;
}

HRESULT CVIBuffer_Circle::Initialize(void* pArg)
{
	return S_OK;
}


CVIBuffer_Circle* CVIBuffer_Circle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CVIBuffer_Circle* pInstance = new CVIBuffer_Circle(pDevice, pContext);
	
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : VIBuffer_Circle");
		Safe_Release(pInstance);
	}
	
	return pInstance;
}

CComponent* CVIBuffer_Circle::Clone(void* pArg)
{
	CVIBuffer_Circle* pInstance = new CVIBuffer_Circle(*this);

	if(FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : VIBuffer_CirCle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Circle::Free()
{
	__super::Free();

}
