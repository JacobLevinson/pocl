#include <vx_spawn.h>
#include <vx_print.h>
#include "kernel_args.h"

int g_work_dim;
dim3_t g_global_offset;

void* vx_local_alloc(uint32_t size) {
  return __local_mem(size);
}

void* __vx_get_kernel_callback(int kernel_id);

uint32_t alignOffset2(uint32_t offset, uint32_t alignment)
{
  return (offset + alignment - 1) & ~(alignment - 1);
}

int main(void) {
  kernel_args_t* kargs = (kernel_args_t*)csr_read(VX_CSR_MSCRATCH);

  g_work_dim = kargs->work_dim;
  for (int i = 0, n = kargs->work_dim; i < 3; i++) {
    g_global_offset.m[i] = (i < n) ? kargs->global_offset[i] : 0;
  }
  uint32_t aligned_kernel_args_size = alignOffset2(sizeof(kernel_args_t), sizeof(size_t));
  void* arg = (void*)((uint8_t*)kargs + aligned_kernel_args_size);
  vx_kernel_func_cb kernel_func = (vx_kernel_func_cb)__vx_get_kernel_callback(kargs->kernel_id);
  return vx_spawn_threads_spatial(kargs->work_dim, kargs->num_groups, kargs->local_size, kernel_func, arg);
}
