#include "Material.h"
#include "Shader.h"

CMaterial::CMaterial(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:m_pDevice(pDevice)
	,m_pContext(pContext)
{
	Safe_AddRef(pDevice);
	Safe_AddRef(pContext);
}

HRESULT CMaterial::Initialize(const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	for (size_t i =0; i< AI_TEXTURE_TYPE_MAX; i++)
	{
		/* 특정 속성을 표현하는 텍스처가 몇장인가? (ex) Diffuse 텍스처가 몇장인지 */
		_uint iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(i));
		/* i = 0 이면 non , i =1 이면 Diffuse , i = 2 이면 Specluar  등 총 18가지의 종류*/

		
		m_MtrlTextures[i].reserve(iNumTextures);	
		

		for(size_t j=0; j< iNumTextures; j++)
		{
			/* ShaderResoureView 자료형으로 만들어주기*/
			aiString                    strTextureFilePath; 
			ID3D11ShaderResourceView*   pSRV = { nullptr };

			/* strTextureFilePath는 내가 추출 당시에  사용했던 텍스처 경로라 다름! 따라서 이걸 
			   내 클라이언트 파일 안에있는 resource로 경로를 바꿔주기 위해 전처리 작업을 진행*/
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(i), j, &strTextureFilePath)))
				return E_FAIL;

			_char  szDrive[MAX_PATH] = "";
			_char  szDir[MAX_PATH] = "";
			_char  szFileName[MAX_PATH] = "";
			_char  szEXT[MAX_PATH] = "";

			/* 내가 넣은 모델 경로의 드라이브 경로와 디렉토리 경로까지 분리해서 넣어줌*/
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);
			
			_char szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar szLastPath[MAX_PATH] = {};

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szLastPath, MAX_PATH);
	
			HRESULT hr = {};

			if (false == strcmp(szEXT, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);

			else if (false == strcmp(szEXT, ".tga"))
				return E_FAIL; 

			else
			{
				hr = CreateWICTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);
			}

			if (FAILED(hr))
				return E_FAIL; 


			m_MtrlTextures[i].push_back(pSRV);
		}
	}

	return S_OK; 
}

HRESULT CMaterial::Bind_Material(CShader* pShader, aiTextureType eType, const _char* pConstantName, _uint iTextureIndex)
{
	if (iTextureIndex >= m_MtrlTextures[eType].size())
		return E_FAIL;

	return pShader->Bind_SRV(pConstantName, m_MtrlTextures[eType][iTextureIndex]);
}

HRESULT CMaterial::Save_Material(ostream& os, const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	for(size_t i= 1; i< AI_TEXTURE_TYPE_MAX; i++)
	{
		m_iNumTextures = pAIMaterial->GetTextureCount(aiTextureType(i));

		//os.write((char*)&m_iNumTextures, sizeof(_uint));	
		os.write((char*)&m_iNumTextures, sizeof(_uint));	

		m_ForSaveMtrlTextures[i].reserve(m_iNumTextures);		
		
		for(size_t j =0; j< m_iNumTextures; j++)
		{
			/* ShaderResoureView 자료형으로 만들어주기*/
			aiString                    strTextureFilePath;
			ID3D11ShaderResourceView* pSRV = { nullptr };

			/* strTextureFilePath는 내가 추출 당시에  사용했던 텍스처 경로라 다름! 따라서 이걸
			   내 클라이언트 파일 안에있는 resource로 경로를 바꿔주기 위해 전처리 작업을 진행*/
			if (FAILED(pAIMaterial->GetTexture(aiTextureType(i), j, &strTextureFilePath)))
				return E_FAIL;

			_char  szDrive[MAX_PATH] = "";
			_char  szDir[MAX_PATH] = "";
			_char  szFileName[MAX_PATH] = "";
			_char  szEXT[MAX_PATH] = "";

			/* 내가 넣은 모델 경로의 드라이브 경로와 디렉토리 경로까지 분리해서 넣어줌*/
			_splitpath_s(pModelFilePath, szDrive, MAX_PATH, szDir, MAX_PATH, nullptr, 0, nullptr, 0);
			_splitpath_s(strTextureFilePath.data, nullptr, 0, nullptr, 0, szFileName, MAX_PATH, szEXT, MAX_PATH);

			_char szFullPath[MAX_PATH] = "";
			strcpy_s(szFullPath, szDrive);
			strcat_s(szFullPath, szDir);
			strcat_s(szFullPath, szFileName);
			strcat_s(szFullPath, szEXT);

			_tchar szLastPath[MAX_PATH] = {};

			MultiByteToWideChar(CP_ACP, 0, szFullPath, strlen(szFullPath), szLastPath, MAX_PATH);

			HRESULT hr = {};

			if (false == strcmp(szEXT, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);

			else if (false == strcmp(szEXT, ".tga"))
				return E_FAIL;

			else
			{
				hr = CreateWICTextureFromFile(m_pDevice, szLastPath, nullptr, &pSRV);
			}

			if (FAILED(hr))
				return E_FAIL;

			
			m_ForSaveMtrlTextures[i].push_back(pSRV);	

			//os.write((char*)&m_iNumTextures, sizeof(_uint));	
			os.write((char*)&szLastPath, sizeof(_tchar) * MAX_PATH);	

		}
		
		
	}

	return S_OK;
}

HRESULT CMaterial::Load_Material(istream& os)
{
	for(size_t i=1; i< AI_TEXTURE_TYPE_MAX; i++)
	{
		os.read((char*)&m_iNumTextures, sizeof(_uint));
		/*그럼 저게 지금 m_iNumTextures 가 최종값으로 결국 변하니깐 그 과정에 값들을 저장해줘야겠다.*/
		m_vecSaveNumTexture.push_back(m_iNumTextures);	
	

		m_MtrlTextures[i].reserve(m_iNumTextures);	

		for(size_t j =0; j< m_iNumTextures; j++)	
		{
			os.read((char*)&m_szLastPath, sizeof(_tchar) * MAX_PATH);	

			ID3D11ShaderResourceView* pSRV = { nullptr };

			HRESULT hr = {};	
			_char  szEXT[MAX_PATH] = "";
			_char  szFullPath[MAX_PATH] = "";			


			WideCharToMultiByte(CP_ACP, 0, m_szLastPath, wcslen(m_szLastPath), szFullPath, MAX_PATH,nullptr,nullptr);

			_splitpath_s(szFullPath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);			


			if (false == strcmp(szEXT, ".dds"))
				hr = CreateDDSTextureFromFile(m_pDevice, m_szLastPath, nullptr, &pSRV);		

			else if (false == strcmp(szEXT, ".tga"))	
				return E_FAIL;	

			else
			{
				hr = CreateWICTextureFromFile(m_pDevice, m_szLastPath, nullptr, &pSRV);	
			}

			if (FAILED(hr))
				return E_FAIL;


			m_MtrlTextures[i].push_back(pSRV);	


		}
		
	}

	return S_OK;	
}

HRESULT CMaterial::Save_Pass_Material(ostream& os)
{
	_uint index = 0; 

	for (size_t i = 1; i < AI_TEXTURE_TYPE_MAX; i++)
	{
		
		os.write((char*)&m_vecSaveNumTexture.at(i-1), sizeof(_uint));		

		for (size_t j = 0; j < m_vecSaveNumTexture.at(i - 1); j++)	
		{
			os.write((char*)&m_szLastPath, sizeof(_tchar) * MAX_PATH);		
		}

	}

	return S_OK;
}

CMaterial* CMaterial::LoadCreate(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, istream& os)	
{
	CMaterial* pInstance = new CMaterial(pDevice,pContext);

	if(FAILED(pInstance->Load_Material(os)))
	{
		MSG_BOX("Failed to LoadCreate : Material");
		Safe_Release(pInstance);	
	}

	return pInstance;	
}

CMaterial* CMaterial::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMaterial* pAIMaterial, const _char* pModelFilePath)
{
	CMaterial* pInstance = new CMaterial(pDevice, pContext);

	if(FAILED(pInstance->Initialize(pAIMaterial,pModelFilePath)))
	{
		MSG_BOX("Failed to Created : Material");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMaterial::Free()
{
	__super::Free(); 

	for(auto& vectorSRV : m_MtrlTextures)
	{
		for (auto& SRV : vectorSRV)
			Safe_Release(SRV);
		
		vectorSRV.clear();
	}

	for (auto& vectorSRV2 : m_ForSaveMtrlTextures)
	{
		for (auto& SRV : vectorSRV2)
			Safe_Release(SRV);

		vectorSRV2.clear();	
	}


	Safe_Release(m_pDevice);	
	Safe_Release(m_pContext);	

}
