#pragma once

#include "Base.h"

/* �ش� Ŭ������ ���� ����*/
/* 1. ������ ���������ο� ���� (��� ��ü�鿡�� ���������� ����� �� �ִ�, ����(x)) ������ ����� �� �ִ� �ʼ������� �ʿ��� ���º�ȯ ����� ������.(��,����)*/
/* 2. ��, ������ ������ �ִ� ��Ȳ�� ���� �������ִ� ����� ������.*/
/* 3. ��, ������ ������� ����Ͽ� �����Ѵ�.*/

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