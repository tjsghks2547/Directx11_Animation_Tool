/* hlsl �� ���� Ÿ�� : float2, float3, float4, vector */ 
#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_Texture;
SamplerState sampler0;

float g_fTimeDelta;  
float g_Count;

  

/* 1. ������ �ε��� ���� + ���̴� ���ε��ؼ� �׸���. */ 
/* 2. �ε������ۿ� ����Ǿ� �ִ� �ε����� �ش��ϴ� ������ �ϳ� ������, */
/* 3. �������̴��Լ��� ȣ���ϸ� ���ڷ� ����(���ý����̽�����)�� �������ش�. */
/* 4. ���� ���̴����� ������ �ʿ��� ������ �����Ѵ�. (ex> ������ ������ġ���ٰ� ����,�� ,��������� ���Ѵ�.)*/


struct VS_IN
{
    float3    vPosition : POSITION;
    float2    vTexcoord : TEXCOORD0;
    
};

struct VS_OUT
{
    float4    vPosition : SV_POSITION;
    float2    vTexcoord : TEXCOORD0;
};


VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT)0; /* 0���� ����ü ��� ��� �ʱ�ȭ */
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    //Out.vPosition = float4(In.vPosition, 1.f);
    
    Out.vTexcoord.x = In.vTexcoord.x - g_fTimeDelta * g_Count;
    Out.vTexcoord.y = In.vTexcoord.y + g_fTimeDelta * g_Count;
    
    return Out; 
}

/* 5. �������̴��� ������ �Ǹ� �׸��� ���¿����� �߰����� ������ ����� ��ٸ���.*/
/* 5-1. D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST ���ٸ� �߰��� �ΰ��� ������ �������̴��� ����Ҷ����� ��ٸ���.*/
/* 6. ���� �׸������ϴ� ������ ���¿� �°Բ� ������ ��� ���ϵȴٸ� �� �������� w������ ����(��������)�� �����Ѵ�. ���������̽��� ���� ���Ŵ�. */
/* 7. ����Ʈ�� ������ ��ġ�� ��ȯ�ϰԵȴ�. ������ ��ǥ�� �������� ��� �� �Űܳ���. */
/* 8. �����Ͷ���� �����ϰ� �ȴ� ( �����Ͷ�����: �������� ������ ������� �Ͽ� �ȼ��� ������ �����Ѵ�) => �ȼ��� �������. */
/* 9. ���� �ȼ��� PS_MAIN �Լ��� ȣ���ϸ鼭 ���ڷ� ������ �ش�.*/
/* 10. �ȼ� ���̴����� ���޹��� �ȼ��� ������ ��������Ͽ� �������� �ȼ��� ���� �����Ͽ� �����ϰԵǰ� */
/* 11. ���ϵ� �������� ������ ����۸� ä��� �ȴ�. */


struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float2 vTexcoord : TEXCOORD0;
};

struct PS_OUT
{    
    float4 vColor : SV_TARGET0;   
};




PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT)0;
    
    //float2 center = float2(0.5f, 0.5f);
    ////Out.vColor = 0.5f; /*RGBA �����̴� */ 
    //float distance = length(In.vTexcoord - center);
    
    //float radius = 0.5f;
    //float borderThickness = 0.001f; // �׵θ� �β� ����

    //// �׵θ� �������� ���� ����
    //if (distance >= radius - borderThickness && distance <= radius + borderThickness)
    //{
    //    Out.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // ������ �׵θ�
    //}
    //else
    //{
    //    Out.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f); // ����
    //}
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 
    
    /* �����׽�Ʈ ���� */ 
    //if( 0.2f >= Out.vColor.a)
    //    discard;
    
    return Out;
}


PS_OUT PS_MAIN_BRUSH(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 center = float2(0.5f, 0.5f);
    //Out.vColor = 0.5f; /*RGBA �����̴� */ 
    float distance = length(In.vTexcoord - center);
    
    float radius = 0.5f;
    float borderThickness = 0.0015f; // �׵θ� �β� ����

    // �׵θ� �������� ���� ����
    if (distance >= radius - borderThickness && distance <= radius + borderThickness)
    {
        Out.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // ������ �׵θ�
    }
    else
    {
        discard; // ������ ������ 
    }
    
    /* �����׽�Ʈ ���� */ 
    //if( 0.2f >= Out.vColor.a)
    //    discard;
    
    return Out;
}

technique11 DefaultTechnique
{ 
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);
    
        VertexShader = compile vs_5_0 VS_MAIN();
        PixelShader = compile ps_5_0 PS_MAIN();
    }

    pass DefaultBrush
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN_BRUSH();
    }
}