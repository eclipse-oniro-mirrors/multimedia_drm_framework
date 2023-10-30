#ifndef OHOS_DRM_LOG_H
#define OHOS_DRM_LOG_H

#include <stdio.h>

#include "hilog/log.h"
#include "hisysevent.h"
#include "hitrace_meter.h"
#include <cinttypes>

#undef LOG_DOMAIN
#undef LOG_TAG
#define LOG_DOMAIN 0xD002B00
#define LOG_TAG "DRM"
#define MAX_STRING_SIZE 256

#define __FILENAME__ (__builtin_strrchr(__FILE__, '/') ? __builtin_strrchr(__FILE__, '/') + 1 : __FILE__)

#define POINTER_MASK 0x00FFFFFF
#define FAKE_POINTER(addr) (POINTER_MASK & reinterpret_cast<uintptr_t>(addr))

#define DECORATOR_HILOG(op, fmt, args...)                                                                       \
    do{                                                                                                         \
        op(LOG_CORE, "{%{public}s()-%{public}s:%{public}d}" fmt, __FUNCTION__, __FILENAME__, __LINE__, ##args); \
    } while(0)                                                                                                  \

#define DRM_DEBUG_LOG(fmt, ...) DECORATOR_HILOG(HILOG_DEBUG, fmt, ##__VA_ARGS__)
#define DRM_ERR_LOG(fmt, ...) DECORATOR_HILOG(HILOG_ERROR, fmt, ##__VA_ARGS__)
#define DRM_WARNING_LOG(fmt, ...) DECORATOR_HILOG(HILOG_WARN, fmt, ##__VA_ARGS__)
#define DRM_INFO_LOG(fmt, ...) DECORATOR_HILOG(HILOG_INFO, fmt, ##__VA_ARGS__)
#define DRM_FATAL_LOG(fmt, ...) DECORATOR_HILOG(HILOG_FATAL, fmt, ##__VA_ARGS__)

#ifndef CHECK_AND_RETURN_RET_LOG
#define CHECK_AND_RETURN_RET_LOG(cond, ret, fmt, ...) \
    do{                                               \
        if (!(cond)) {                                  \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);          \
            return ret;                               \
        }                                             \
    } while(0)
#endif

#ifndef CHECK_AND_RETURN_LOG
#define CHECK_AND_RETURN_LOG(cond, fmt, ...)          \
    do{                                               \
        if (!(cond)) {                                  \
            DRM_ERR_LOG(fmt, ##__VA_ARGS__);          \
            return;                                   \
        }                                             \
    } while(0)
#endif

#define POINTER_MASK 0x00FFFFFF                                                                   

#endif //OHOS_DRM_LOG_H