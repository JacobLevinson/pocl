#ifndef POCL_VORTEX_H
#define POCL_VORTEX_H

#include "pocl_cl.h"

#include "prototypes.inc"
GEN_PROTOTYPES (vortex)

/* Manually-added custom prototypes: */

cl_int pocl_vortex_alloc_spatial_mem_obj(cl_device_id dev, cl_mem mem_obj, void *host_ptr,
                                         int Dx, int Dy, int Dz, int Tx, int Ty, int Tz);
                                         
#endif /* POCL_VORTEX_H */
