#ifndef CL_EXT_VORTEX_H
#define CL_EXT_VORTEX_H

#include <CL/cl.h>

#ifdef __cplusplus
extern "C"
{
#endif

    extern CL_API_ENTRY cl_mem CL_API_CALL
    clCreateSpatialBuffer(cl_context context,
                          cl_mem_flags flags,
                          size_t size,
                          int Dx, int Dy, int Dz,
                          int Tx, int Ty, int Tz,
                          cl_int *errcode_ret);

#ifdef __cplusplus
}
#endif

#endif /* CL_EXT_VORTEX_H */