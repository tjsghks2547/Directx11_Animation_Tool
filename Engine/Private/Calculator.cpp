#include "Calculator.h"
#include "Transform.h"

CCalculator::CCalculator()
{
}



void CCalculator::Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, CTransform* pTransform)
{
	_matrix	 WorldMatrixInv = pTransform->Get_WorldMatrixInverse();

	XMStoreFloat3(pRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldMatrixInv));	
	XMStoreFloat3(pRayDir, XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldMatrixInv));	

	
}

void CCalculator::Compute_LocalRayInfo(_float3* pRayDir, _float3* pRayPos, XMMATRIX WorldInverse)
{
	XMStoreFloat3(pRayPos, XMVector3TransformCoord(XMLoadFloat3(&m_vRayPos), WorldInverse));	
	XMStoreFloat3(pRayDir, XMVector3TransformNormal(XMLoadFloat3(&m_vRayDir), WorldInverse));	
}

_float3 CCalculator::Compute_PickingPoint(_float3 _point1, _float3 _point2, _float3 _point3)
{

	_float3 test;
	_float3 test2;

	XMStoreFloat3(&test,  (XMLoadFloat3(&_point2) - XMLoadFloat3(&_point1)));
	XMStoreFloat3(&test2, (XMLoadFloat3(&_point3) - XMLoadFloat3(&_point1)));

	XMVECTOR test3 = XMVector3Normalize(XMVector3Cross(XMLoadFloat3(&test), XMLoadFloat3(&test2)));
	
	_float3 normal; /* ��������*/	
	XMStoreFloat3(&normal, test3);
	
	/* ����� ��� ��� */
	float D = -(normal.x * _point1.x +
		normal.y * _point1.y +
		normal.z * _point1.z);

	/* ����� ������ */

	/* normal.x * x  + normal.y * y + normal.z* z + d = 0 */

	/* ������ ������ ���ؾ��� */
	//m_vRayDir /* ���� */
	//m_vRayPos /* ��ġ */ 


	// ������ ���� ���Ϳ� ��ġ
	_float3 rayDir = m_vRayDir; // ��: �Է¹ްų� ������ ��	
	_float3 rayPos = m_vRayPos; // ��: �Է¹ްų� ������ ��	

	// �и� ���
	float denominator = normal.x * rayDir.x + normal.y * rayDir.y + normal.z * rayDir.z;

	// �и� 0�̸� ������ ����� ����
	if (fabs(denominator) < 1e-6f)
	{
		// ������ ���� ó�� (���⼭�� �ʱ�ȭ�� ���� ��ȯ)
		return _float3(0.0f, 0.0f, 0.0f);
	}

	// �Ű����� t ���
	float numerator = -(normal.x * rayPos.x + normal.y * rayPos.y + normal.z * rayPos.z + D);
	float t = numerator / denominator;

	// ������ ���
	_float3 intersection;
	intersection.x = rayPos.x + t * rayDir.x;
	intersection.y = rayPos.y + t * rayDir.y;
	intersection.z = rayPos.z + t * rayDir.z;
	/*_float3 test4; 
	XMStoreFloat3(&test4, test3);*/

	return intersection;	
}

HRESULT CCalculator::Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	ZeroMemory(&m_viewport, sizeof(D3D11_VIEWPORT));
	UINT numViewports = 1; 
	pContext->RSGetViewports(&numViewports, &m_viewport);
	m_hWnd = hWnd;

	return S_OK;
}

void CCalculator::Store_MouseRay(XMMATRIX _mat_ProjInverse, XMMATRIX _mat_viewInverse)
{
	GetCursorPos(&m_MousePoint);
	ScreenToClient(m_hWnd, &m_MousePoint);


	_float3 vMousePos;

	/* ��ũ����ǥ���� -> NDC ��ǥ��� ��ȯ����*/
	vMousePos.x = m_MousePoint.x / (m_viewport.Width  * 0.5f) - 1.f; 
	vMousePos.y = m_MousePoint.y / -(m_viewport.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	/* z���� 0.f�� �δ� ���� 
	�׷� ray�� �� z=0�� �������� ��� �� ȭ�� ��ü�� �ش��ϴ� ������ ���� �����Ŵϱ� ��� ��ü�� ���̸� �ƿ﷯�� ����ִٴ� �ǹ̳�?
	ChatGPT�� ��:
	ChatGPT
	��, �½��ϴ�! z = 0�� �������� ���̸� ��ٴ� ���� ȭ���� ���� �տ� �ִ� ��ü(near-plane)�� �ش��ϴ� ���̸� �������� ���̸� ��� �Ͱ� �����ϴ�.*/


	/* NDC ��ǥ�谡 ���������̽��ϱ� (���������̽� -> �佺���̽� ��ȯ)*/
	/* �� w(=�佺���̽� z��) �� ��������Ѵٴ°� (����������ϱ�)*/
	/* ������ �츮�� ���콺 ���̸� ��� �������� �߿��� ���� ������ ���� ����(vecRayDir)�� ������(vecRayPos)�Դϴ�. ���⼭ z ���� �߰������� �������� �ʴ� ������:

	vecRayPos�� ����(0, 0, 0)���� �����ϰ�, vecRayDir�� ����ȭ�� ���� ���ͷ� �����մϴ�.
	vecMousePos�� ȭ���� NDC ��ǥ���� 3D �������� ��ȯ�� ��ǥ�Դϴ�. �� �� z ���� ���̸� ��Ÿ���� ��������, ���콺 ���̸� �� ���� �밳 ���̸� 0���� �ΰų� Ư�� ���� ���� �����Ͽ� ���̸� ���ϴ�. */

	_vector vecMousePos = XMLoadFloat3(&vMousePos);
	vecMousePos = XMVectorSetW(vecMousePos, 1.f);

	vecMousePos = XMVector3TransformCoord(vecMousePos, _mat_ProjInverse);
	_vector  vecRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector  vecRayDir = vecMousePos - vecRayPos; 
	
	/* ���� �����̽� ���� ��ǥ�� ( �佺���̽� -> ���彺���̽�)*/
	vecRayPos = XMVector3TransformCoord(vecRayPos, _mat_viewInverse);	
	vecRayDir = XMVector3TransformNormal(vecRayDir, _mat_viewInverse);	
	vecRayDir = XMVector3Normalize(vecRayDir);	

	XMStoreFloat3(&m_vRayPos, vecRayPos);	
	XMStoreFloat3(&m_vRayDir, vecRayDir);	

}


CCalculator* CCalculator::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CCalculator* pInstance = new CCalculator();

	if(FAILED(pInstance->Initialize(pDevice,pContext,hWnd)))
	{
		MSG_BOX("Failed to Created : Calculator");
		Safe_Release(pInstance);
	}
	
	return pInstance; 
}

void CCalculator::Free()
{
	__super::Free();
}
