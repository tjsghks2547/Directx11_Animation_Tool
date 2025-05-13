#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

float4x4 g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;

vector g_vLightDir;
vector g_vLightDiffuse;
Texture2D g_DiffuseTexture;


/*11월 14일 부터 추가된 코드 */ 
vector g_vLightAmbient;
vector g_vLightSpecular;
vector g_vCamPosition;

vector g_vMtrlAmbient = vector(0.3f, 0.3f, 0.3f, 1.0f); /* 빛이 아예 안들어오는 부분이라도 검은색으로 칠해지지 않게 하기 위해 설정*/
vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);


//sampler LinearSampler = sampler_state
//{
//    Filter = MIN_MAG_MIP_LINEAR;
//    AddressU = wrap;
//    AddressV = wrap;

//};

struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;

};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;

    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);

    
    
    Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    
    Out.vTexcoord = In.vTexcoord;
    
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;

};

struct PS_OUT
{
    float4 vColor : SV_TARGET0;
    //float vWorldPos : SV_TARGET1;
};

PS_OUT PS_MAIN(PS_IN In)
{
    PS_OUT Out = (PS_OUT) 0;

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord * 30.f);

    //Out.vColor = vMtrlDiffuse;
    
    vector fShade = max(dot(normalize(g_vLightDir) * -1.f,
		normalize(In.vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient);
    
    vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal)); /* 로컬끼리해준거 */
    vector vLook = In.vWorldPos - g_vCamPosition;
    
    float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
    
    Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade)
    + fSpecular * (g_vLightSpecular * g_vMtrlSpecular);
    
    //Out.vWorldPos.x = In.vWorldPos; 
    
    return Out;
}


technique11 DefaultTechnique
{
    pass DefaultPass
    {
        SetRasterizerState(RS_Default);
        SetDepthStencilState(DSS_Default, 0);
        SetBlendState(BS_Blend, float4(0.f, 0.f, 0.f, 0.f), 0xffffffff);

        //SetSemplerState(LinearSampler);
        VertexShader = compile vs_5_0 VS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
