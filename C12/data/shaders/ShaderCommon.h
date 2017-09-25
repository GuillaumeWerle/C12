// 25.09.2017

#ifndef __SHADER_COMMONH_H
#define __SHADER_COMMONH_H

#ifdef __cplusplus

#define __CPU

#define SPACE_INDEX(val)	val

#else
#define __GPU

#define SPACE_INDEX(val)	space##val

#endif

// Register space declaration
#define SPACE_ENGINE_CB			SPACE_INDEX(1)
#define SPACE_VERTEX_SRV		SPACE_INDEX(2)


#endif // __SHADER_COMMONH_H