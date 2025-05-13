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
	
	_float3 normal; /* 법선벡터*/	
	XMStoreFloat3(&normal, test3);
	
	/* 평면의 상수 계산 */
	float D = -(normal.x * _point1.x +
		normal.y * _point1.y +
		normal.z * _point1.z);

	/* 평면의 방정식 */

	/* normal.x * x  + normal.y * y + normal.z* z + d = 0 */

	/* 직선의 방정식 구해야함 */
	//m_vRayDir /* 방향 */
	//m_vRayPos /* 위치 */ 


	// 직선의 방향 벡터와 위치
	_float3 rayDir = m_vRayDir; // 예: 입력받거나 설정된 값	
	_float3 rayPos = m_vRayPos; // 예: 입력받거나 설정된 값	

	// 분모 계산
	float denominator = normal.x * rayDir.x + normal.y * rayDir.y + normal.z * rayDir.z;

	// 분모가 0이면 직선과 평면이 평행
	if (fabs(denominator) < 1e-6f)
	{
		// 교차점 없음 처리 (여기서는 초기화된 값을 반환)
		return _float3(0.0f, 0.0f, 0.0f);
	}

	// 매개변수 t 계산
	float numerator = -(normal.x * rayPos.x + normal.y * rayPos.y + normal.z * rayPos.z + D);
	float t = numerator / denominator;

	// 교차점 계산
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

	/* 스크린좌표에서 -> NDC 좌표계로 변환해줌*/
	vMousePos.x = m_MousePoint.x / (m_viewport.Width  * 0.5f) - 1.f; 
	vMousePos.y = m_MousePoint.y / -(m_viewport.Height * 0.5f) + 1.f;
	vMousePos.z = 0.f;

	/* z값을 0.f로 두는 이유 
	그럼 ray를 쏠때 z=0을 기점으로 쏘면 아 화면 전체에 해당하는 깊이중 가장 작은거니깐 모든 물체의 깊이를 아울러서 쏠수있다는 의미네?
	ChatGPT의 말:
	ChatGPT
	네, 맞습니다! z = 0을 기점으로 레이를 쏜다는 것은 화면의 가장 앞에 있는 물체(near-plane)에 해당하는 깊이를 기준으로 레이를 쏘는 것과 같습니다.*/


	/* NDC 좌표계가 투영스페이스니깐 (투영스페이스 -> 뷰스페이스 변환)*/
	/* 즉 w(=뷰스페이스 z값) 을 곱해줘야한다는거 (나눠줬었으니깐)*/
	/* 하지만 우리는 마우스 레이를 쏘는 과정에서 중요한 것은 레이의 방향 벡터(vecRayDir)와 시작점(vecRayPos)입니다. 여기서 z 값이 추가적으로 곱해지지 않는 이유는:

	vecRayPos는 원점(0, 0, 0)으로 설정하고, vecRayDir는 정규화된 방향 벡터로 설정합니다.
	vecMousePos는 화면의 NDC 좌표에서 3D 공간으로 변환된 좌표입니다. 이 때 z 값은 깊이를 나타내는 값이지만, 마우스 레이를 쏠 때는 대개 깊이를 0으로 두거나 특정 깊이 값을 지정하여 레이를 쏩니다. */

	_vector vecMousePos = XMLoadFloat3(&vMousePos);
	vecMousePos = XMVectorSetW(vecMousePos, 1.f);

	vecMousePos = XMVector3TransformCoord(vecMousePos, _mat_ProjInverse);
	_vector  vecRayPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	_vector  vecRayDir = vecMousePos - vecRayPos; 
	
	/* 월드 스페이스 상의 좌표들 ( 뷰스페이스 -> 월드스페이스)*/
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
