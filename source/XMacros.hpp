//
// Created by 林炳河 on 2019/3/4.
//

#ifndef MMMacros_h
#define MMMacros_h

/***** 安全释放资源相关宏定义 *****/
#define SAFE_DELETE(p)     \
    do {                   \
        if (p) {           \
            delete (p);    \
            (p) = nullptr; \
        }                  \
    } while (0)
#define SAFE_DELETE_ARRAY(p) \
    do {                     \
        if (p) {             \
            delete[](p);     \
            (p) = nullptr;   \
        }                    \
    } while (0)
#define SAFE_FREE(p)       \
    do {                   \
        if (p) {           \
            free(p);       \
            (p) = nullptr; \
        }                  \
    } while (0)
#define SAFE_RELEASE(p)     \
    do {                    \
        if (p) {            \
            (p)->release(); \
        }                   \
    } while (0)
#define SAFE_RELEASE_NULL(p) \
    do {                     \
        if (p) {             \
            (p)->release();  \
            (p) = nullptr;   \
        }                    \
    } while (0)
#define SAFE_RETAIN(p)     \
    do {                   \
        if (p) {           \
            (p)->retain(); \
        }                  \
    } while (0)

#define BREAK_IF(cond) \
    if (cond) break

/***** 命名空间相关宏定义 *****/
#ifdef __cplusplus
#define NS_X_IMAGE_BEGIN namespace XImageNS {
#define NS_X_IMAGE_END }
#define USING_NS_X_IMAGE using namespace XImageNS;
#define NS_X_IMAGE XImageNS
#else
#define NS_X_IMAGE_BEGIN
#define NS_X_IMAGE_END
#define USING_NS_X_IMAGE
#define NS_X_IMAGE
#endif

#endif //MMMacros_h
