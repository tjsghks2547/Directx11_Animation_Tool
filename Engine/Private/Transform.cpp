#include "Transform.h"
#include "Shader.h"

CTransform::CTransform(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent(pDevice,pContext)
{
  
}


void CTransform::Set_Scale(_float fScaleX, _float fScaleY, _float fScaleZ)
{
    Set_State(STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * fScaleX);
    Set_State(STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * fScaleY);
    Set_State(STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * fScaleZ);
}

HRESULT CTransform::Initialize_Prototype()
{
    XMStoreFloat4x4(&m_WorldMatrix, XMMatrixIdentity());    

    return S_OK;
}

HRESULT CTransform::Initialize(void* pArg)
{
    /* �׷� ���⼭ fSpeedPerSec�� fRotationPerSec ���� �����Ų��
     ����? -> pArg���� �޾ƿͼ� 
     pArg���� transform ����� ����� ����ü ���� �ƴϸ� 
     Gaemobejct���� �����ҵ� */

    TRANSFORM_DESC* pDesc = static_cast<TRANSFORM_DESC*>(pArg);
   

    if (nullptr == pDesc)
    {
        return S_OK;    
    }
       
    m_fSpeedPerSec    = pDesc->fSpeedPerSec; 
    m_fRotationPerSec = pDesc->fRotationPerSec;
    m_fPosition       = pDesc->fPostion;
 
    
    
    Set_State(STATE_POSITION, XMVectorSetW(XMLoadFloat3(&m_fPosition), 1.f));       
    
    return S_OK;
}

HRESULT CTransform::Bind_ShaderResource(CShader* pShader, const _char* pConstantName)
{
    pShader->Bind_Matrix(pConstantName, &m_WorldMatrix);

    return S_OK;
}

_matrix CTransform::Get_RotationMatrixFromWorld(const _matrix& worldMatrix)
{
   // ���� ����� �� �� ���� ����
   _vector right = worldMatrix.r[0]; // Right ����
   _vector up = worldMatrix.r[1];    // Up ����
   _vector look = worldMatrix.r[2];  // Look ����

   // �� �� ���͸� ����ȭ�Ͽ� ������ ����
   right = XMVector3Normalize(right);
   up = XMVector3Normalize(up);
   look = XMVector3Normalize(look);

   // ȸ�� ��� ����
   _matrix rotationMatrix;
   rotationMatrix.r[0] = right; // ����ȭ�� Right ����
   rotationMatrix.r[1] = up;    // ����ȭ�� Up ����
   rotationMatrix.r[2] = look;  // ����ȭ�� Look ����
   rotationMatrix.r[3] = XMVectorSet(0.f, 0.f, 0.f, 1.f); // ��ġ ����

   return rotationMatrix;
}

void CTransform::Go_Straight(_float fTimeDelta)
{
    /* ���� ��ġ ���ϱ�*/
    _vector vPosition = Get_State(STATE_POSITION);
    _vector vLook = Get_State(STATE_LOOK);

    /* Look vector normalize ���ֱ�*/
    vPosition += XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;   
    
    Set_State(STATE_POSITION, vPosition);

}

void CTransform::Go_BackWard(_float fTimeDelta)
{
    /* ���� ��ġ ���ϱ�*/
    _vector vPosition = Get_State(STATE_POSITION);  
    _vector vLook = Get_State(STATE_LOOK);  


    vPosition -= XMVector3Normalize(vLook) * m_fSpeedPerSec * fTimeDelta;    

    Set_State(STATE_POSITION, vPosition);   
}

void CTransform::Go_Left(_float fTimeDelta)
{
    /* ���� ��ġ ���ϱ�*/
    _vector vPosition = Get_State(STATE_POSITION);
    _vector vRight = Get_State(STATE_RIGHT);


    vPosition -= XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE_POSITION, vPosition);
}

void CTransform::Go_Right(_float fTimeDelta)
{
    /* ���� ��ġ ���ϱ�*/
    _vector vPosition = Get_State(STATE_POSITION);  
    _vector vRight = Get_State(STATE_RIGHT);    


    vPosition += XMVector3Normalize(vRight) * m_fSpeedPerSec * fTimeDelta;

    Set_State(STATE_POSITION, vPosition);   
}

void CTransform::Turn(_vector vAxis, _float fTimeDelta)
{
    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp    = Get_State(STATE_UP);
    _vector vLook  = Get_State(STATE_LOOK);

    /*�̰� �ϳ��� ������ ȸ���ϴ°Ŵϱ� ������ ���� ���ص�?*/

    if(m_fRotationPerSec == 0.0f)
    {
        m_fRotationPerSec = 5.f;
    }

    _matrix RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);

    
    
    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp    = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook  = XMVector3TransformNormal(vLook, RotationMatrix);
    
    Set_State(STATE_RIGHT,vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);

}

void CTransform::Turn_X(_float _fDegree)
{

    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    /*�̰� �ϳ��� ������ ȸ���ϴ°Ŵϱ� ������ ���� ���ص�?*/

    _matrix RotationMatrix = XMMatrixRotationX(XMConvertToRadians(_fDegree));    


    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(STATE_RIGHT, vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);
}

void CTransform::Turn_Y(_float _fDegree)
{

    _vector vRight = Get_State(STATE_RIGHT);
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    /*�̰� �ϳ��� ������ ȸ���ϴ°Ŵϱ� ������ ���� ���ص�?*/

    _matrix RotationMatrix = XMMatrixRotationY(XMConvertToRadians(_fDegree));


    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(STATE_RIGHT, vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);
}

void CTransform::Turn_Z(_float _fDegree)
{

    _vector vRight = Get_State(STATE_RIGHT);    
    _vector vUp = Get_State(STATE_UP);
    _vector vLook = Get_State(STATE_LOOK);

    /*�̰� �ϳ��� ������ ȸ���ϴ°Ŵϱ� ������ ���� ���ص�?*/

    _matrix RotationMatrix = XMMatrixRotationZ(XMConvertToRadians(_fDegree));   


    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    Set_State(STATE_RIGHT, vRight);
    Set_State(STATE_UP, vUp);
    Set_State(STATE_LOOK, vLook);
}



void CTransform::LookAt(_fvector vAt)
{
    _float3 vScale = Compute_Scaled();  

    _vector vLook  = vAt - Get_State(STATE_POSITION);
    _vector vRight = XMVector3Cross(XMVectorSet(0.f, 1.f, 0.f, 0.f), vLook);
    _vector vUp = XMVector3Cross(vLook, vRight);


    Set_State(STATE_RIGHT, XMVector3Normalize(vRight) * vScale.x);
    Set_State(STATE_UP, XMVector3Normalize(vUp) * vScale.y);
    Set_State(STATE_LOOK, XMVector3Normalize(vLook) * vScale.z);
    
   
}

void CTransform::Turn_Degree(_fvector vAxis, _float Degree)
{
    _vector			vRight = Get_State(CTransform::STATE_RIGHT);
    _vector			vUp = Get_State(CTransform::STATE_UP);
    _vector			vLook = Get_State(CTransform::STATE_LOOK);

    _matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Degree);

    Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
    Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
    Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

void CTransform::Orbit_Move(_fvector vAxis, _float fTimeDelta, _fvector vCenter)
{
    _vector			vPosition = Get_State(CTransform::STATE_POSITION);  

    _vector         vRelativePos = vPosition - vCenter; 

    _matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, m_fRotationPerSec * fTimeDelta);   


    _vector         vNewCamPos = XMVectorSetW(vCenter + XMVector3TransformCoord(vRelativePos, RotationMatrix), 1.f);

    Set_State(STATE_POSITION, vNewCamPos);  
}

void CTransform::Orbit_Move_Once(_fvector vAxis, _float Radian, _fvector vCenter)
{
    _vector			vPosition = Get_State(CTransform::STATE_POSITION);
    _vector         vRelativePos = vPosition - vCenter;
    _matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);


    _vector         vNewCamPos = XMVectorSetW(vCenter + XMVector3TransformNormal(vRelativePos, RotationMatrix), 1.f);

    Set_State(STATE_POSITION, vNewCamPos);
    //_vector			vPosition = Get_State(CTransform::STATE_POSITION);
    //_vector         vRelativeLength = XMVector3Length(XMVectorSubtract(vPosition,vCenter));//vPosition - vCenter;  
    //_vector         vRelativeDir = XMVector3Normalize(vPosition - vCenter); 
    //_matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);   

    //// �ùٸ��� ȸ�� ����
    ////_vector vRotatedDir = XMVector3TransformNormal(vRelativeDir, RotationMatrix);   

    ////// ȸ���� �� �ٽ� �߽��� ����
    ////_vector vNewCamPos = vCenter + vRotatedDir * XMVectorGetX(vRelativeLength);  

    ////Set_State(STATE_POSITION, XMVectorSetW(vNewCamPos, 1.f));   
    //_vector         vNewCamPos = XMVectorSetW(vCenter + XMVector3TransformNormal(vRelativePos, RotationMatrix), 1.f);
    //
    //Set_State(STATE_POSITION, vNewCamPos);
}

_float4x4 CTransform::Orbit_Move_RelativePos(_fmatrix vWorldMatrix, _fvector vRelativePos, _fvector vAxis, _float Radian, _fvector vCenter)
{
    _vector			vRight = vWorldMatrix.r[0];
    _vector			vUp = vWorldMatrix.r[1];
    _vector			vLook = vWorldMatrix.r[2];

    _matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, Radian);

    vRight = XMVector3TransformNormal(vRight, RotationMatrix);
    vUp = XMVector3TransformNormal(vUp, RotationMatrix);
    vLook = XMVector3TransformNormal(vLook, RotationMatrix);

    _vector         vNewCamPos = XMVectorSetW(vCenter + XMVector3TransformCoord(vRelativePos, RotationMatrix), 1.f);

    _matrix  ConvertedMatrix = XMMatrixIdentity();

    ConvertedMatrix.r[0] = vRight;
    ConvertedMatrix.r[1] = vUp;
    ConvertedMatrix.r[2] = vLook;
    ConvertedMatrix.r[3] = vNewCamPos;

    _float4x4  finalConvertedMatrix = {};
    XMStoreFloat4x4(&finalConvertedMatrix, ConvertedMatrix);

    return  finalConvertedMatrix;
}

void CTransform::Rotation(_fvector vAxis, _float fRadians)
{
    _float3			vScaled = Compute_Scaled();

    _vector			vRight = XMVectorSet(1.f, 0.f, 0.f, 0.f) * vScaled.x;
    _vector			vUp = XMVectorSet(0.f, 1.f, 0.f, 0.f) * vScaled.y;
    _vector			vLook = XMVectorSet(0.f, 0.f, 1.f, 0.f) * vScaled.z;

    _matrix			RotationMatrix = XMMatrixRotationAxis(vAxis, fRadians);

    Set_State(STATE_RIGHT, XMVector3TransformNormal(vRight, RotationMatrix));
    Set_State(STATE_UP, XMVector3TransformNormal(vUp, RotationMatrix));
    Set_State(STATE_LOOK, XMVector3TransformNormal(vLook, RotationMatrix));
}

_float3 CTransform::Compute_Scaled()
{
    /* ���⼭ �� X��Ҹ� Get�ϳĸ� XMVector3Length�� ��ȯŸ����
    XMVector�̴� �׷��� ���� ���̰� 2�̸� (2,2,2,2)�� �������ֱ� ������
    �� �� �ϳ��� ��Ҹ� ���� ���� XMVectorGetX�� ����ߴ�.*/
    return _float3(
        XMVectorGetX(XMVector3Length(Get_State(STATE_RIGHT))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_UP))),
        XMVectorGetX(XMVector3Length(Get_State(STATE_LOOK)))
    );
}

void CTransform::Scaling(const _float3& vScale) 
{
    Set_State(STATE::STATE_RIGHT, XMVector3Normalize(Get_State(STATE_RIGHT)) * vScale.x);
    Set_State(STATE::STATE_UP, XMVector3Normalize(Get_State(STATE_UP)) * vScale.y);
    Set_State(STATE::STATE_LOOK, XMVector3Normalize(Get_State(STATE_LOOK)) * vScale.z);
}

CTransform* CTransform::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CTransform* pInstance = new CTransform(pDevice, pContext);

    if(FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : CTransform");
        Safe_Release(pInstance);
    }

    return pInstance;
}

//CComponent* CTransform::Clone(void* pArg)
//{
//    CTransform* pInstance = new CTransform(*this);
//    
//    if(FAILED(pInstance->Initialize(pArg)))
//    {
//        MSG_BOX("Failed to Cloned : CTransform");
//        Safe_Release(pInstance);    
//    }
//
//    return pInstance;
//}

void CTransform::Free()
{
    __super::Free();    
    

}
