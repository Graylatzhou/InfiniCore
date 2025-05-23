#ifndef __INFINIOPTEST_UTILS_HPP__
#define __INFINIOPTEST_UTILS_HPP__
#include "../../utils.h"
#include "gguf.hpp"
#include <cstring>
#include <iostream>

#define CHECK_OR(cmd, action) CHECK_API_OR(cmd, INFINI_STATUS_SUCCESS, action)

inline double getVal(void *ptr, GGML_TYPE ggml_type) {
    switch (ggml_type) {
    case GGML_TYPE_F16:
        return utils::cast<double>(*(fp16_t *)ptr);
    case GGML_TYPE_F32:
        return *(float *)ptr;
    case GGML_TYPE_F64:
        return *(double *)ptr;
    case GGML_TYPE_I8:
        return *(int8_t *)ptr;
    case GGML_TYPE_I16:
        return *(int16_t *)ptr;
    case GGML_TYPE_I32:
        return *(int32_t *)ptr;
    case GGML_TYPE_I64:
        return (double)(*(int64_t *)ptr);
    default:
        throw std::runtime_error("Unsupported data type");
    }
}

#endif
