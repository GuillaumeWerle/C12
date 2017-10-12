//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "ShaderCommon.h"

struct CBPass
{
    float4x4 ViewProj;
};

struct CBObj
{
    float4x4 World;
};
    
struct CBStruct
{
	float4 Color;
	float4 Offset;
};

#ifdef __GPU

struct VSInput
{
    float4 position : POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

cbuffer cbpass : register(ENGINE_CB_REGISTER_PASS, SPACE_ENGINE_CB)
{
    CBPass g_Pass;
}

cbuffer cbobj : register(ENGINE_CB_REGISTER_OBJ, SPACE_ENGINE_CB)
{
    CBObj g_Obj;
}

cbuffer cb : register(b0, SPACE_USER_CB)
{
	CBStruct g_CB;
}

Texture2D g_Albedo : register(t0);

PSInput VSMain(VSInput IN)
{
    PSInput result;

	result.position		= IN.position * float4(3,3,1,1) + g_CB.Offset;
	result.color		= IN.color * 2.0f;
	result.uv			= IN.uv * 4.0f;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return g_Albedo.Sample(g_Sampler, input.uv) * input.color;
}

#endif // __GPU