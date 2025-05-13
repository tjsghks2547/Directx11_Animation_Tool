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

float g_fTimeDelta;

/* 파도 */
float g_fTime; // 시간 (CPU에서 전달) 
float g_fAmplitude; // 파동의 높이   
float g_fWavelength; // 파동의 주기
float g_fSpeed; // 파동의 속도   



struct VS_IN
{
    float3 vPosition : POSITION;
    float3 vNormal   : NORMAL;
    float2 vTexcoord : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
    float2 vTexcoord : TEXCOORD0;
    float4 vWorldPos : TEXCOORD1;

};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out = (VS_OUT) 0;
    
 //   float waveX = g_fAmplitude * sin((In.vPosition.x + g_fTime * 10.f) / g_fWavelength);
 //   float waveZ = g_fAmplitude * cos((In.vPosition.z + g_fTime * 10.f) / g_fWavelength);
	////pVertices[i].vPosition.y = waveX  + waveZ;	
    
 //   In.vPosition.y = waveX + waveZ; 

    //float wave1 = g_fAmplitude * sin((In.vPosition.x + g_fTime * g_fSpeed) / g_fWavelength);
    //float wave2 = g_fAmplitude * 0.5f * sin((In.vPosition.z - g_fTime * g_fSpeed * 0.5f) / (g_fWavelength * 2.0f));
    //float wave3 = g_fAmplitude * 0.3f * sin((In.vPosition.x + In.vPosition.z + g_fTime * g_fSpeed * 0.8f) / (g_fWavelength * 1.5f));
    //In.vPosition.y = wave1 + wave2 + wave3;
    
    float waveValueA = sin(g_fTime * g_fSpeed + In.vPosition.x * g_fWavelength) * g_fAmplitude;
    float waveValueB = cos(g_fTime * g_fSpeed + In.vPosition.y * g_fWavelength) * g_fAmplitude;
    
    float waveValueC = sin(g_fTime * 1.5 + (In.vPosition.z + In.vPosition.y) * g_fWavelength) * g_fAmplitude;
    float waveValueD = cos(g_fTime * 1.5 + (In.vPosition.z + In.vPosition.y) * g_fWavelength) * g_fAmplitude;

    float waveValueE = sin(g_fTime * 0.5 + (In.vPosition.z + In.vPosition.x) * g_fWavelength * 0.3) * g_fAmplitude;
    float waveValueF = cos(g_fTime * 0.5 + (In.vPosition.z + In.vPosition.x) * g_fWavelength * 0.3) * g_fAmplitude;

    
    float3 VMove = float3(In.vPosition.x + waveValueA, In.vPosition.y + waveValueB, In.vPosition.z);

    VMove = VMove + float3(In.vPosition.x + waveValueC, In.vPosition.y + waveValueD, In.vPosition.z);

    VMove = VMove + float3(In.vPosition.x + waveValueF, In.vPosition.y + waveValueE, In.vPosition.z);

    In.vPosition.xyz = VMove;
    
    Out.vPosition = mul(float4(In.vPosition, 1.f), g_WorldMatrix);
    Out.vPosition = mul(Out.vPosition, g_ViewMatrix);
    Out.vPosition = mul(Out.vPosition, g_ProjMatrix);

    
    //Out.vNormal = mul(float4(In.vNormal, 0.f), g_WorldMatrix);
    
    In.vTexcoord.x = In.vTexcoord.x - g_fTimeDelta * 0.4f;
    In.vTexcoord.y = In.vTexcoord.y - g_fTimeDelta * 0.2f;
    

    Out.vTexcoord = In.vTexcoord;
    //Out.vTexcoord.x = In.vTexcoord.x + g_fTimeDelta;
    //Out.vTexcoord.y = In.vTexcoord.y;
    
    Out.vWorldPos = mul(float4(In.vPosition, 1.f), g_WorldMatrix);

    return Out;
}

//struct GS_OUT
//{
//    float4 vPosition : SV_POSITION;
//    float4 vNormal   : NORMAL;
//    float2 vTexcoord : TEXCOORD0;
//    float4 vWorldPos : TEXCOORD1;
//
//};
//
//[maxvertexcount(3)] // 최대 출력 정점 수   
//void GS_MAIN(triangle VS_OUT input[3], inout TriangleStream<GS_OUT> OutputStream)
//{
//  
//    // 삼각형의 정점 가져오기
//    float3 v0 = input[0].vPosition.xyz;
//    float3 v1 = input[1].vPosition.xyz;
//    float3 v2 = input[2].vPosition.xyz;
//
//    // 삼각형의 노멀 계산
//    float3 edge1 = v1 - v0;
//    float3 edge2 = v2 - v0;
//    float4 normal = float4(normalize(cross(edge1, edge2)), 0.f);
//
//    // 각 정점에 노멀을 기록
//    for (int i = 0; i < 3; ++i)
//    {
//        GS_OUT output = (GS_OUT) 0;
//        output.vPosition = input[i].vPosition;
//        output.vNormal = normal;
//        //output.vNormal = mul(output.vNormal, g_WorldMatrix);
//        output.vTexcoord = input[i].vTexcoord;
//        output.vWorldPos = input[i].vWorldPos;
//        OutputStream.Append(output);
//    }
//}

struct PS_IN
{
    float4 vPosition : SV_POSITION;
    float4 vNormal   : NORMAL;
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

    vector vMtrlDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexcoord);
    vector purpleTint = vector(1.0f, 0.0f, 1.0f, 1.0f); 
    
    Out.vColor = vMtrlDiffuse * purpleTint;
    
    //vector fShade = max(dot(normalize(g_vLightDir) * -1.f,
	//	normalize(In.vNormal)), 0.f) + (g_vLightAmbient * g_vMtrlAmbient);
    //
    //vector vReflect = reflect(normalize(g_vLightDir), normalize(In.vNormal)); /* 로컬끼리해준거 */
    //vector vLook = In.vWorldPos - g_vCamPosition;
    //
    //float fSpecular = pow(max(dot(normalize(vReflect) * -1.f, normalize(vLook)), 0.f), 50.f);
    //
    ////Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) + fSpecular * (g_vLightSpecular * g_vMtrlSpecular); //* saturate(fShade);
    //////+ fSpecular * (g_vLightSpecular * g_vMtrlSpecular);
    //Out.vColor = (g_vLightDiffuse * vMtrlDiffuse) * saturate(fShade);
    //+ fSpecular * (g_vLightSpecular * g_vMtrlSpecular);
    
    
    
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
        //GeometryShader = compile gs_5_0 GS_MAIN();
        GeometryShader = NULL;
        PixelShader = compile ps_5_0 PS_MAIN();
    }
}
