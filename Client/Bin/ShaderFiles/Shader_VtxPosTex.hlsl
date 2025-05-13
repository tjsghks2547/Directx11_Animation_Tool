/* hlsl 용 벡터 타입 : float2, float3, float4, vector */ 
#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

Texture2D g_Texture;
SamplerState sampler0;

float g_fTimeDelta;  
float g_Count;

  

/* 1. 정점과 인덱스 버퍼 + 셰이더 바인딩해서 그린다. */ 
/* 2. 인덱스버퍼에 저장되어 있는 인덱스에 해당하는 정점을 하나 꺼내서, */
/* 3. 정점쉐이더함수를 호출하며 인자로 정점(로컬스페이스기준)을 전달해준다. */
/* 4. 정점 쉐이더에서 정점에 필요한 연산을 수행한다. (ex> 정점의 로컬위치에다가 월드,뷰 ,투영행렬을 곱한다.)*/


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
    VS_OUT Out = (VS_OUT)0; /* 0으로 구조체 요소 모두 초기화 */
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);
    //Out.vPosition = float4(In.vPosition, 1.f);
    
    Out.vTexcoord.x = In.vTexcoord.x - g_fTimeDelta * g_Count;
    Out.vTexcoord.y = In.vTexcoord.y + g_fTimeDelta * g_Count;
    
    return Out; 
}

/* 5. 정점쉐이더가 끝나게 되면 그리는 형태에따라 추가적인 정점의 결과를 기다린다.*/
/* 5-1. D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST 였다면 추가로 두개의 정점이 정점쉐이더를 통과할때까지 기다린다.*/
/* 6. 내가 그릴려고하는 도형의 형태에 맞게끔 정점이 모두 리턴된다면 각 정점에게 w나누기 연산(원근투영)을 수행한다. 투영스페이스로 이제 간거다. */
/* 7. 뷰포트로 정점의 위치를 변환하게된다. 윈도우 좌표로 정점들을 모두 다 옮겨낸다. */
/* 8. 래스터라이즈를 수행하게 된다 ( 래스터라이즈: 정점들의 정보를 기반으로 하여 픽셀의 정보를 생성한다) => 픽셀을 만들었다. */
/* 9. 만들어낸 픽셀을 PS_MAIN 함수를 호출하면서 인자로 전달해 준다.*/
/* 10. 픽셀 쉐이더에서 전달받은 픽셀의 정보를 기반으로하여 최종적인 픽셀의 색을 결정하여 리턴하게되고 */
/* 11. 리턴된 최종적인 색으로 백버퍼를 채우게 된다. */


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
    ////Out.vColor = 0.5f; /*RGBA 순서이다 */ 
    //float distance = length(In.vTexcoord - center);
    
    //float radius = 0.5f;
    //float borderThickness = 0.001f; // 테두리 두께 설정

    //// 테두리 영역에만 색상 적용
    //if (distance >= radius - borderThickness && distance <= radius + borderThickness)
    //{
    //    Out.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // 빨간색 테두리
    //}
    //else
    //{
    //    Out.vColor = float4(0.0f, 0.0f, 0.0f, 0.0f); // 투명
    //}
    Out.vColor = g_Texture.Sample(LinearSampler, In.vTexcoord); 
    
    /* 알파테스트 예시 */ 
    //if( 0.2f >= Out.vColor.a)
    //    discard;
    
    return Out;
}


PS_OUT PS_MAIN_BRUSH(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;
    
    float2 center = float2(0.5f, 0.5f);
    //Out.vColor = 0.5f; /*RGBA 순서이다 */ 
    float distance = length(In.vTexcoord - center);
    
    float radius = 0.5f;
    float borderThickness = 0.0015f; // 테두리 두께 설정

    // 테두리 영역에만 색상 적용
    if (distance >= radius - borderThickness && distance <= radius + borderThickness)
    {
        Out.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f); // 빨간색 테두리
    }
    else
    {
        discard; // 나머지 버리기 
    }
    
    /* 알파테스트 예시 */ 
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