//#include "../../../EngineSDK/hlsl/Engine_Shader_Defines.hlsli"

//matrix g_WorldMatrix, g_ViewMatrix, g_ProjMatrix;
//matrix g_ViewMatrixInv, g_ProjMatrixInv, g_LightViewMatrix, g_LightProjMatrix;

//vector g_vCamPosition;

//vector g_vLightDir;
//vector g_vLightPos;
//float g_fLightRange;
//vector g_vLightDiffuse;
//vector g_vLightAmbient;
//vector g_vLightSpecular;

//vector g_vMtrlAmbient = vector(1.f, 1.f, 1.f, 1.f);
//vector g_vMtrlSpecular = vector(1.f, 1.f, 1.f, 1.f);

//texture2D g_DiffuseTexture;
//texture2D g_NormalTexture;
//texture2D g_DepthTexture;
//texture2D g_ShadeTexture;
//texture2D g_SpecularTexture;
//texture2D g_ReflectionTexture;
//texture2D g_ShadowDepthTexture;
//texture2D g_BlurTexture;


//sampler LinearSampler = sampler_state
//{
//    filter = min_mag_mip_Linear;
//};

//sampler DepthSampler = sampler_state
//{
//    filter = min_mag_Linear_mip_point;
//    AddressU = clamp;
//    AddressV = clamp;

//};


//struct VS_IN
//{
//    float3 vPosition : POSITION;
//    float2 vTexUV : TEXCOORD0;
//};

//struct VS_OUT
//{
//    float4 vPosition : SV_POSITION;
//    float2 vTexUV : TEXCOORD0;
//};

//VS_OUT VS_MAIN(VS_IN In)
//{
//    VS_OUT Out = (VS_OUT) 0;

//    matrix matWV, matWVP;

//    matWV = mul(g_WorldMatrix, g_ViewMatrix);
//    matWVP = mul(matWV, g_ProjMatrix);

//    Out.vPosition = mul(vector(In.vPosition, 1.f), matWVP);
//    Out.vTexUV = In.vTexUV;

//    return Out;
//}

//struct PS_IN
//{
//    float4 vPosition : SV_POSITION;
//    float2 vTexUV : TEXCOORD0;
//};

//struct PS_OUT
//{
//    float4 vColor : SV_TARGET0;
//};

///* 이렇게 만들어진 픽셀을 PS_MAIN함수의 인자로 던진다. */
///* 리턴하는 색은 Target0 == 장치에 0번째에 바인딩되어있는 렌더타겟(일반적으로 백버퍼)에 그린다. */
///* 그래서 백버퍼에 색이 그려진다. */
//PS_OUT PS_MAIN(PS_IN In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

//    return Out;
//}

//PS_OUT PS_RESULT(PS_IN In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    Out.vColor = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);

//    return Out;
//}

//struct PS_OUT_BLEND
//{
//    float4 vColor : SV_TARGET0;
//};

//struct PS_OUT_LIGHT
//{
//    float4 vShade : SV_TARGET0;
//    float4 vReflection : SV_TARGET1;
//};

//PS_OUT_LIGHT PS_MAIN_LIGHT_DIRECTIONAL(PS_IN In)
//{
//    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

//	/* 0 ~ 1 => -1 ~ 1*/
//    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
//    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
//    vector vSpecularDesc = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
//    vSpecularDesc.yz = vSpecularDesc.x;

//    float fViewZ = vDepthDesc.y * 500.f;

//    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);

//    Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(g_vLightDir) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
//    Out.vShade.a = 1.f;

//    vector vWorldPos = (vector) 0.f;

//	/* 투영 공간상의 위치를 구했다. */
//	/* 투영 공간 == 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
//    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
//    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
//    vWorldPos.z = vDepthDesc.r;
//    vWorldPos.w = 1.0f;

//	/* 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
//    vWorldPos *= fViewZ;

//	/* 뷰 공간상의 위치르 ㄹ구한다. */
//	/* 로컬점의위치 * 월드행렬 * 뷰행렬 */
//    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

//	/* 로컬점의위치 * 월드행렬 */
//    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

//    vector vReflect = reflect(normalize(g_vLightDir), normalize(vNormal));
//    vector vLook = vWorldPos - g_vCamPosition;

//    Out.vReflection = (g_vLightSpecular * vSpecularDesc) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f);
//    Out.vReflection.a = 0.f;
//    return Out;
//}

//PS_OUT_LIGHT PS_MAIN_LIGHT_POINT(PS_IN In)
//{
//    PS_OUT_LIGHT Out = (PS_OUT_LIGHT) 0;

//	/* 0 ~ 1 => -1 ~ 1*/
//    vector vNormalDesc = g_NormalTexture.Sample(LinearSampler, In.vTexUV);
//    vector vDepthDesc = g_DepthTexture.Sample(LinearSampler, In.vTexUV);
//    vector vSpecularDesc = g_SpecularTexture.Sample(LinearSampler, In.vTexUV);
//    vSpecularDesc.yz = vSpecularDesc.x;

//    float fViewZ = vDepthDesc.y * 500.f;

//    vector vWorldPos = (vector) 0.f;

//	/* 투영 공간상의 위치를 구했다. */
//	/* 투영 공간 == 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 / w */
//    vWorldPos.x = In.vTexUV.x * 2.f - 1.f;
//    vWorldPos.y = In.vTexUV.y * -2.f + 1.f;
//    vWorldPos.z = vDepthDesc.r;
//    vWorldPos.w = 1.0f;

//	/* 로컬점의위치 * 월드행렬 * 뷰행렬 * 투영행렬 */
//    vWorldPos *= fViewZ;

//	/* 뷰 공간상의 위치르 ㄹ구한다. */
//	/* 로컬점의위치 * 월드행렬 * 뷰행렬 */
//    vWorldPos = mul(vWorldPos, g_ProjMatrixInv);

//	/* 로컬점의위치 * 월드행렬 */
//    vWorldPos = mul(vWorldPos, g_ViewMatrixInv);

//    vector vNormal = vector(vNormalDesc.xyz * 2.f - 1.f, 0.f);
//    vector vLightDir = vWorldPos - g_vLightPos;
//    float fDistance = length(vLightDir);

//    float fAtt = saturate((g_fLightRange - fDistance) / g_fLightRange);

//    Out.vShade = g_vLightDiffuse * (saturate(dot(normalize(vLightDir) * -1.f, normalize(vNormal))) + (g_vLightAmbient * g_vMtrlAmbient));
//    Out.vShade *= fAtt;
//    Out.vShade.a = 1.f;

//    vector vReflect = reflect(normalize(vLightDir), normalize(vNormal));
//    vector vLook = vWorldPos - g_vCamPosition;

//    Out.vReflection = (g_vLightSpecular * vSpecularDesc) * pow(saturate(dot(normalize(vReflect) * -1.f, normalize(vLook))), 30.f);
//    Out.vReflection *= fAtt;
//    Out.vReflection.a = 0.f;

//    return Out;
//}



//PS_OUT_BLEND PS_MAIN_BLEND(PS_IN In)
//{
//    PS_OUT Out = (PS_OUT) 0;

//    vector vDiffuse = g_DiffuseTexture.Sample(LinearSampler, In.vTexUV);
//    vector vShade = g_ShadeTexture.Sample(LinearSampler, In.vTexUV);
//    vector vReflection = g_ReflectionTexture.Sample(LinearSampler, In.vTexUV);

//    Out.vColor = vDiffuse * vShade + vReflection;

//    vector vDepthInfo = g_DepthTexture.Sample(DepthSampler, In.vTexUV);
//    float fViewZ = vDepthInfo.y * 500.f;

//    vector vPosition;

//    vPosition.x = (In.vTexUV.x * 2.f - 1.f) * fViewZ;
//    vPosition.y = (In.vTexUV.y * -2.f + 1.f) * fViewZ;
//    vPosition.z = vDepthInfo.x * fViewZ;
//    vPosition.w = fViewZ;

//	// 뷰 상
//    vPosition = mul(vPosition, g_ProjMatrixInv);

//	// 월드 상
//    vPosition = mul(vPosition, g_ViewMatrixInv);

//    vPosition = mul(vPosition, g_LightViewMatrix);

//    vector vUVPos = mul(vPosition, g_LightProjMatrix);
//    float2 vNewUV;

//    vNewUV.x = (vUVPos.x / vUVPos.w) * 0.5f + 0.5f;
//    vNewUV.y = (vUVPos.y / vUVPos.w) * -0.5f + 0.5f;

//    vector vShadowDepthInfo = g_ShadowDepthTexture.Sample(DepthSampler, vNewUV);
///*
//	if (vPosition.z - 0.1f > vShadowDepthInfo.r * 500.0f)
//		Out.vColor = vector(0.f, 0.f, 0.f, 1.f);*/

//    if (vPosition.z - 0.2f > vShadowDepthInfo.r * 500.f)
//        Out.vColor -= vector(0.1f, 0.1f, 0.1f, 0.f);

//    if (Out.vColor.a == 0.f)
//        discard;

//    return Out;
//}



//RasterizerState RS_Default
//{
//    FillMode = solid;
//    CullMode = back;
//    FrontCounterClockwise = false;
//};
//BlendState BS_Default
//{
//    BlendEnable[0] = false;
//};

//BlendState BS_LightBlending
//{
//    BlendEnable[0] = true;
//    BlendEnable[1] = true;

//    SrcBlend = one;
//    DestBlend = one;
//    BlendOp = add;
//};

//DepthStencilState DSS_Default
//{
//    DepthEnable = true;
//    DepthWriteMask = all;
//    DepthFunc = less_equal;
//};

//DepthStencilState DSS_ZEnable_Disable_ZWrite_Disable
//{
//    DepthEnable = false;
//    DepthWriteMask = zero;
//};



//technique11 DefaultTechnique
//{
//    pass DebugTarget //0
//    {
//        SetRasterizerState(RS_Default);
//        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
//        SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

//        VertexShader = compile vs_5_0 VS_MAIN();
//        GeometryShader = NULL;
//        PixelShader = compile ps_5_0 PS_MAIN();
//    }

//    pass Light_Directional //1
//    {
//        SetRasterizerState(RS_Default);
//        SetBlendState(BS_LightBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
//        SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

//        VertexShader = compile vs_5_0 VS_MAIN();
//        GeometryShader = NULL;
//        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_DIRECTIONAL();
//    }

//    pass Light_Point //2
//    {
//        SetRasterizerState(RS_Default);
//        SetBlendState(BS_LightBlending, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
//        SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

//        VertexShader = compile vs_5_0 VS_MAIN();
//        GeometryShader = NULL;
//        PixelShader = compile ps_5_0 PS_MAIN_LIGHT_POINT();
//    }

//    pass Blend //3
//    {
//        SetRasterizerState(RS_Default);
//        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
//        SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

//        VertexShader = compile vs_5_0 VS_MAIN();
//        GeometryShader = NULL;
//        PixelShader = compile ps_5_0 PS_MAIN_BLEND();
//    }

//    pass ResultOut //4
//    {
//        SetRasterizerState(RS_Default);
//        SetBlendState(BS_Default, float4(0.f, 0.f, 0.f, 1.f), 0xffffffff);
//        SetDepthStencilState(DSS_ZEnable_Disable_ZWrite_Disable, 0);

//        VertexShader = compile vs_5_0 VS_MAIN();
//        GeometryShader = NULL;
//        PixelShader = compile ps_5_0 PS_RESULT();
//    }

//}