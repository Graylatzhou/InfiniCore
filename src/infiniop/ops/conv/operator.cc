#include "../../operator.h"
#include "../../handle.h"
#include "infiniop/ops/conv.h"

#ifdef ENABLE_CPU_API
#include "cpu/conv_cpu.h"
#endif
#ifdef ENABLE_CUDA_API
#include "cuda/conv_cuda.cuh"
#endif

__C __export infiniStatus_t infiniopCreateConvDescriptor(infiniopHandle_t handle,
                                                         infiniopConvDescriptor_t *desc_ptr,
                                                         infiniopTensorDescriptor_t y_desc,
                                                         infiniopTensorDescriptor_t x_desc,
                                                         infiniopTensorDescriptor_t w_desc,
                                                         void *pads,
                                                         void *strides,
                                                         void *dilations,
                                                         size_t n) {
#define CREATE(CASE, NAMESPACE)                                             \
    case CASE:                                                              \
        return op::conv::NAMESPACE::Descriptor::create(                     \
            handle,                                                         \
            reinterpret_cast<op::conv::NAMESPACE::Descriptor **>(desc_ptr), \
            y_desc,                                                         \
            x_desc,                                                         \
            w_desc,                                                         \
            pads,                                                           \
            strides,                                                        \
            dilations,                                                      \
            n)
    switch (handle->device) {
#ifdef ENABLE_CPU_API
        CREATE(INFINI_DEVICE_CPU, cpu);
#endif
#ifdef ENABLE_CUDA_API
        CREATE(INFINI_DEVICE_NVIDIA, cuda);
#endif
    default:
        return INFINI_STATUS_DEVICE_TYPE_NOT_SUPPORTED;
    }
#undef CREATE
}

__C infiniStatus_t
infiniopGetConvWorkspaceSize(
    infiniopConvDescriptor_t desc,
    size_t *size) {

#define GET(CASE, NAMESPACE)                                                                      \
    case CASE:                                                                                    \
        *size = reinterpret_cast<const op::conv::NAMESPACE::Descriptor *>(desc)->workspaceSize(); \
        return INFINI_STATUS_SUCCESS

    switch (desc->device_type) {

#ifdef ENABLE_CPU_API
        GET(INFINI_DEVICE_CPU, cpu);
#endif
#ifdef ENABLE_CUDA_API
        GET(INFINI_DEVICE_NVIDIA, cuda);
#endif
    default:
        return INFINI_STATUS_DEVICE_TYPE_NOT_SUPPORTED;
    }

#undef GET
}

__C infiniStatus_t infiniopConv(
    infiniopConvDescriptor_t desc,
    void *workspace,
    size_t workspace_size,
    void *y,
    const void *x,
    const void *w,
    void *stream) {
#define CALCULATE(CASE, NAMESPACE)                                             \
    case CASE:                                                                 \
        return reinterpret_cast<const op::conv::NAMESPACE::Descriptor *>(desc) \
            ->calculate(workspace, workspace_size,                             \
                        y,                                                     \
                        x,                                                     \
                        w,                                                     \
                        stream)
    switch (desc->device_type) {
#ifdef ENABLE_CPU_API
        CALCULATE(INFINI_DEVICE_CPU, cpu);
#endif
#ifdef ENABLE_CUDA_API
        CALCULATE(INFINI_DEVICE_NVIDIA, cuda);
#endif

    default:
        return INFINI_STATUS_DEVICE_TYPE_NOT_SUPPORTED;
    }
#undef CALCULATE
}

__C infiniStatus_t
infiniopDestroyConvDescriptor(infiniopConvDescriptor_t desc) {
#define DELETE(CASE, NAMESPACE)                                                 \
    case CASE:                                                                  \
        delete reinterpret_cast<const op::conv::NAMESPACE::Descriptor *>(desc); \
        return INFINI_STATUS_SUCCESS;

    switch (desc->device_type) {
#ifdef ENABLE_CPU_API
        DELETE(INFINI_DEVICE_CPU, cpu);
#endif
#ifdef ENABLE_CUDA_API
        DELETE(INFINI_DEVICE_NVIDIA, cuda);
#endif
    default:
        return INFINI_STATUS_DEVICE_TYPE_NOT_SUPPORTED;
    }
#undef DELETE
}
