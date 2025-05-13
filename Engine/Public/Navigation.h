#pragma once

#include "Component.h"

BEGIN(Engine)


class ENGINE_DLL CNavigation final : public CComponent
{

private:
	CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CNavigation() = default; 

	

public:
	virtual HRESULT Initialize(); 

public:
	virtual HRESULT Add_Cell(_float3* pPoints);
	virtual _bool Find_Cube(_float3* pPoints);		
	HRESULT Render();
	vector<class CCell*>&  Get_VecCell()  { return m_Cells; }	 /* 벡터는 메모리 크기가 커지면 다시 만들기 때문에 참조로 가져온다.*/
	vector<class CCube*>&  Get_VecCube() { return m_vecCubes; }	 /* 벡터는 메모리 크기가 커지면 다시 만들기 때문에 참조로 가져온다.*/


private:
				     
	vector<class CCell*>			m_Cells; 
	vector<class CCube*>			m_vecCubes;			

	_float4x4					m_WorldMatrix; 

	class CShader*				m_pShader = { nullptr };

	_uint						m_iPass = { 0 };

public:
	void Setting_Shader_Pass(_uint _PassNumber) { m_iPass = _PassNumber; }


public:
	static CNavigation* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg) override; 
	virtual void Free() override; 
};

END

