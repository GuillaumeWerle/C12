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

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD;
	float4 color : COLOR;
};

cbuffer cb : register(b0, space0)
{
	float4 g_color;
	float4 g_offset;
}

Buffer<float3> g_VertexPositions : register(t0, space2);

Texture2D g_Albedo : register(t0);
SamplerState g_Sampler : register(s0);


PSInput VSMain(float4 _position : POSITION, float4 color : COLOR, float2 uv : TEXCOORD, uint vertexId : SV_VERTEXID)
{
	PSInput result;

	float4 position = float4(g_VertexPositions[vertexId].xyz, 1.0f);

	result.position = position * float4(3,3,1,1) + g_offset;
	result.color = color;
	result.uv = uv;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return g_Albedo.Sample(g_Sampler, input.uv * 16);
}
