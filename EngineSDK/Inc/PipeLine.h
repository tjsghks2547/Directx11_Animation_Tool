#pragma once

#include "Base.h"

/* 해당 클래스를 만든 이유*/
/* 1. 렌더링 파이프라인에 범용 (모든 객체들에게 공통적으로 적용될 수 있는, 월드(x)) 적으로 사용할 수 있는 필수적으로 필요한 상태변환 행렬을 가진다.(뷰,투영)*/
/* 2. 뷰, 투영을 가지고 있는 상황에 따라 리턴해주는 기능을 가진다.*/
/* 3. 뷰, 투영의 역행렬을 계산하여 저장한다.*/

BEGIN(Engine)

class CPipeLine final : public CBase
{
public:
	enum D3DTRANSFORMSTATE { D3DTS_VIEW, D3DTS_PROJ, D3DTS_END };

private:
	CPipeLine();
	virtual ~CPipeLine() = default; 

public:
	const _float4x4* Get_Transform_float4x4(D3DTRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix(D3DTRANSFORMSTATE eState);
	const _float4x4* Get_Transform_float4x4_Inverse(D3DTRANSFORMSTATE eState);
	_matrix Get_Transform_Matrix_Inverse(D3DTRANSFORMSTATE eState);
	const _float4* Get_CamPosition_float4();
	_vector Get_CamPosition();

public:
	void Set_Transform(D3DTRANSFORMSTATE eState, _fmatrix TransformMatrix) 
	{
		XMStoreFloat4x4(&m_TransformStateMatrix[eState], TransformMatrix);
	}

public:
	HRESULT Initialize();
	void Update();

public:
	_float4x4           m_TransformStateMatrix[D3DTS_END];
	_float4x4           m_TransformStateMatrixInverse[D3DTS_END];
	_float4				m_vCamPosition; 


public:
	static CPipeLine* Create();
	virtual void Free() override;

};

END