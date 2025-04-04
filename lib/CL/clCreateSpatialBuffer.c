#include "common.h"
#include "devices/vortex/pocl-vortex.h"
#include "pocl_cl.h"
#include "pocl_shared.h"
#include "pocl_util.h"

CL_API_ENTRY cl_mem CL_API_CALL
clCreateSpatialBuffer(cl_context context,
                      cl_mem_flags flags,
                      size_t size,
                      int Dx, int Dy, int Dz,
                      int Tx, int Ty, int Tz,
                      cl_int *errcode_ret)
{
    cl_int errcode = CL_SUCCESS;
    cl_mem buffer = NULL;

    buffer = pocl_create_memobject(context,
                                   flags,
                                   size,
                                   CL_MEM_OBJECT_BUFFER,
                                   NULL,
                                   NULL,
                                   CL_FALSE,
                                   &errcode);
    fprintf(stderr, "[clCreateSpatialBuffer] Spatial buffer created!\n");
    if (buffer == NULL)
    {
        if (errcode_ret)
            *errcode_ret = errcode;
        return NULL;
    }

    cl_device_id device = context->devices[0];

    if (device->ops->alloc_spatial_mem_obj)
    {
        errcode = device->ops->alloc_spatial_mem_obj(device, buffer, NULL, Dx, Dy, Dz, Tx, Ty, Tz);
    }
    else
    {
        errcode = CL_INVALID_OPERATION;
    }

    if (errcode != CL_SUCCESS)
    {
        // Do manual cleanup matching POCL's style
        if (buffer->device_ptrs)
        {
            for (unsigned i = 0; i < context->num_devices; ++i)
            {
                cl_device_id dev = context->devices[i];
                pocl_mem_identifier *p = &buffer->device_ptrs[dev->global_mem_id];
                if (p->mem_ptr)
                    dev->ops->free(dev, buffer);
            }
            POCL_MEM_FREE(buffer->device_ptrs);
        }

        if (((flags & CL_MEM_USE_HOST_PTR) == 0) && buffer->mem_host_ptr)
            POCL_MEM_FREE(buffer->mem_host_ptr);

        POCL_MEM_FREE(buffer);

        if (errcode_ret)
            *errcode_ret = errcode;
        return NULL;
    }

    if (errcode_ret)
        *errcode_ret = CL_SUCCESS;

    return buffer;
}