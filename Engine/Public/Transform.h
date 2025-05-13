#pragma once

#include "Component.h"


BEGIN(Engine)
class CShader;

class ENGINE_DLL CTransform final : public CComponent 
{
public:
	struct TRANSFORM_DESC
	{
		_float fSpeedPerSec = {};
		_float fRotationPerSec = {};
		_float3 fPostion = {};
		

	};
	
	//typedef TRANSFORM_DESC  TRANSFORM_STRUCT;

	enum STATE {STATE_RIGHT, STATE_UP, STATE_LOOK, STATE_POSITION, STATE_END};	
	

private:
	CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CTransform() = default;

public:
	_vector Get_State(STATE eState)
	{
		return XMLoadFloat4x4(&m_WorldMatrix).r[eState];	
	}

	/* set �ϳ����������*/
	void Set_MulWorldMatrix(const _float4x4* _Matrix)
	{
		// _float4x4 -> XMMATRIX ��ȯ �� ���� ����
		XMMATRIX result = XMLoadFloat4x4(_Matrix) * XMLoadFloat4x4(&m_WorldMatrix);

		// ����� _float4x4�� ��ȯ�Ͽ� m_WorldMatrix�� ����
		XMStoreFloat4x4(&m_WorldMatrix, result);
	}

	void Set_State(STATE eState, _fvector vState)
	{
		/* ����� ���Ϳ� �����ϴ°Ŵϱ�.*/

		/* ù ��° ���*/
		/*_matrix  WorldMatrix = XMLoadFloat4x4(&m_WorldMatrix);
		WorldMatrix.r[eState] = vState;
		XMStoreFloat4x4(&m_WorldMatrix, WorldMatrix);*/

		/* �� ��° ���*/
		XMStoreFloat4(reinterpret_cast<_float4*>(&m_WorldMatrix.m[eState]), vState);	
	}

	void Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ);


public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Bind_ShaderResource(CShader* pShader, const _char* pConstantName);

public:
	
	_float3 Get_Scaled()
	{
		return _float3(XMVector3Length(Get_State(STATE_RIGHT)).m128_f32[0]
			, XMVector3Length(Get_State(STATE_UP)).m128_f32[0]
			, XMVector3Length(Get_State(STATE_LOOK)).m128_f32[0]);
	}
	
	_matrix Get_RotationMatrixFromWorld(const _matrix& worldMatrix);


	const _float4x4* Get_WorldFloat4X4()
	{
		return &m_WorldMatrix;
	}

	_matrix Get_WorldMatrix()
	{
		return XMLoadFloat4x4(&m_WorldMatrix);
	}


	_matrix Get_WorldMatrixInverse()
	{
		return XMMatrixInverse(nullptr, Get_WorldMatrix());
	}

	const _float4x4* Get_WorldMatrix_Ptr() const {
		return &m_WorldMatrix;	
	}

	void Set_WorldMatrix(_float4x4 worldmat) { m_WorldMatrix = worldmat; }

public:
	void Go_Straight(_float fTimeDelta);
	void Go_BackWard(_float fTimeDelta);
	void Go_Left(_float fTimeDelta);
	void Go_Right(_float fTimeDelta);
	void Turn(_vector vAxis, _float fTimeDelta);
	void Turn_X(_float _fDegree);		
	void Turn_Y(_float _fDegree);		
	void Turn_Z(_float _fDegree);

	void LookAt(_fvector vAt);	

	void Turn_Degree(_fvector vAxis, _float Degree);	
	void Orbit_Move(_fvector vAxis,  _float fTimeDelta, _fvector vCenter);	
	void Orbit_Move_Once(_fvector vAxis, _float Radian, _fvector vCenter);	
	_float4x4 Orbit_Move_RelativePos(_fmatrix vWorldMatrix, _fvector vRelativePos, _fvector vAxis, _float Radian, _fvector vCenter);

	

	void Rotation(_fvector vAxis, _float fRadians); /* ���� ȸ�� : �׻� �׵���¸� �������� �������� ������ ȸ���س��´�.  */	

public:	
	_float3 Compute_Scaled();
	void  Scaling(const _float3& vScale);

private:
	_float4x4                          m_WorldMatrix = { }; 
	_float							   m_fSpeedPerSec = { 0.f };
	_float							   m_fRotationPerSec = { 0.f };
	_float3							   m_fPosition = {};



public:
	static CTransform* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) { return nullptr;}
	virtual void Free() override; 

};

END