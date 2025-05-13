
sampler LinearSampler = sampler_state
{
    Filter = MIN_MAG_MIP_LINEAR;
    AddressU = wrap;
    AddressV = wrap;
};

RasterizerState RS_Default
{
    CullMode = BACK;
	
};
RasterizerState RS_Wireframe
{
    FillMode = WIREFRAME;

};

RasterizerState RS_Cull_CW
{
    FillMode = SOLID;
    CullMode = FRONT;
};

DepthStencilState DSS_Default
{
    DepthEnable = TRUE;
    DepthWriteMask = ALL;
    DepthFunc = LESS_EQUAL;

};

DepthStencilState DSS_Skip_Z
{
    DepthEnable = FALSE;
    DepthWriteMask = ZERO;
};

DepthStencilState DSS_GreaterDepth  
{
    DepthEnable = TRUE; 
    DepthWriteMask = ALL; // 깊이값을 기록    
    DepthFunc = GREATER; // 깊이값이 클수록 먼저 렌더링 
};

BlendState BS_Default
{
    BlendEnable[0] = FALSE;
};

BlendState BS_AlphaBlend
{
    BlendEnable[0] = TRUE;

    BlendOp = Add;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;

	/*
	SrcColor(rgba), DestColor(rgba)

	SrcColor.rgb * SrcColor.a + DestColor.rgb * (1.f - SrcColor.a)
	*/		
};

BlendState BS_Blend
{
    BlendEnable[0] = TRUE;
    BlendOp[0] = Add;
    SrcBlend[0] = ONE;
    DestBlend[0] = ZERO;    

	/*
	SrcColor(rgba), DestColor(rgba)

	SrcColor.rgb * 1 + DestColor.rgb * 1


	*/
};

BlendState BS_CustomBlend
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA; // 기존 값에 새 값을 덮어씀
    BlendOp = ADD;
};
