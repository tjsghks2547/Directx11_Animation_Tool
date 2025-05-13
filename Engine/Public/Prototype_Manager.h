#pragma once

/* 11�� 5�� ����*/
#include "Model.h"
#include "Shader.h"
/* 10�� 31�� ����*/
#include "Triangle.h" // �ﰢ������ 
#include "Texture.h"
#include "Transform.h"

#include "VIBuffer_Rect.h"
#include "VIBuffer_Terrain.h"
#include "VIBuffer_Cube.h"	
#include "VIBuffer_Rect_Texcood2.h"

#include "Collider.h"
#include "Bounding_AABB.h"
#include "Bounding_Sphere.h"


/*
0. ��ü�� ���� + ������Ʈ�� ����. 
1. ���ӳ��� �������� ������ �� �ִ� ��ü���� ������ü�� �����Ѵ�. 
2. �������� �������� �и��Ͽ� �����Ѵ�. 
2-1. ��� �������� �����ϴ� ������ ���� ��Ƽ� �����Ѵ�. 
3. �������� ���ÿ� ���� ������ü�� �����Ͽ� �纻�� ����� �������ִ� ���. 
*/

BEGIN(Engine)

class CPrototype_Manager final : public CBase
{
private:
	CPrototype_Manager();
	virtual ~CPrototype_Manager() = default;	

public:
	HRESULT			Initialize(_uint iNumLevels);	
	HRESULT			Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype);	
	CBase*			Clone_Prototype(PROTOTYPE eType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg);	
	void			Clear(_uint iLevelIndex);

	map<const _wstring, CBase*>* Get_Prototypes_Map() { return m_pPrototypes; }

private:
	_uint								m_iNumLevels = {};

	map<const _wstring, CBase*>*		m_pPrototypes = { nullptr };
	typedef map<const _wstring, CBase*>	PROTOTYPES;

private:
	CBase* Find_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag);

public:
	static CPrototype_Manager* Create(_uint iNumLevels);
	virtual void Free() override;
};

END