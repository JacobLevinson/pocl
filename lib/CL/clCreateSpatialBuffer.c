#include "pocl_cl.h"
#include "pocl_util.h"
#include "devices/pocl-vortex.h"

CL_API_ENTRY cl_mem CL_API_CALL
clCreateSpatialBuffer(cl_context context,
                      cl_mem_flags flags,
                      size_t size,
                      int Dx, int Dy, int Dz,
                      int Tx, int Ty, int Tz,
                      cl_int *errcode_ret)
{

    cl_int err;
    cl_mem buffer = pocl_create_memobject(context, flags, size, NULL, &err);
    if (err != CL_SUCCESS)
    {
        if (errcode_ret)
            *errcode_ret = err;
        return NULL;
    }

    cl_device_id device = context->devices[0]; // assuming single-device context

    err = pocl_vortex_alloc_spatial_mem_obj(device, buffer, NULL, Dx, Dy, Dz, Tx, Ty, Tz);
    if (err != CL_SUCCESS)
    {
        pocl_memobj_destructor(buffer);
        if (errcode_ret)
            *errcode_ret = err;
        return NULL;
    }

    if (errcode_ret)
        *errcode_ret = CL_SUCCESS;
    return buffer;
}
