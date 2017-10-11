// 25.09.2017

#ifndef __SHADER_COMMONH_H
#define __SHADER_COMMONH_H

#ifdef __cplusplus

#define __CPU

#define SPACE_INDEX(val)	val
#define float4      XMFLOAT4
#define float4x4    XMMATRIX
#define ALIGN16     __declspec(align(16))

#define B_REGISTER(v) v

#else
#define __GPU

#define SPACE_INDEX(val)	space##val
#define ALIGN16

SamplerState g_Sampler : register(s0);

#define B_REGISTER(v) b##v

#endif

// Register space declaration
#define SPACE_USER_CB			SPACE_INDEX(0)
#define SPACE_ENGINE_CB			SPACE_INDEX(1)
#define ENGINE_CB_REGISTER_OBJ  B_REGISTER(0)
#define ENGINE_CB_REGISTER_PASS B_REGISTER(1)

#endif // __SHADER_COMMONH_H