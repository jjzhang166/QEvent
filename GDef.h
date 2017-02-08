#pragma once

#include <cstdio>
#include <assert.h>
#define QJ_DEBUG
#define QJCallback
#define QJ_INTERFACE
#define QJ_OVERRIDE override
#define QJ_UNUSE(x) (void)x;
#define QJNEW  new (std::nothrow)
#define QJSignal
#define QJSlot

#define QJ_SHOW_DEFINE( x ) QJ_TO_STRING( x )
#define QJ_TO_STRING( x ) #x

#define QJLOG(argfmt, ...) \
    { \
        printf("QjLog:[%s] " argfmt "\r\n",  __FUNCTION__,##__VA_ARGS__); \
    }
#define QJWarn(argfmt, ...) \
    { \
        printf("***[warn]***\nQJWarn:[%s] " argfmt "\r\n",  __FUNCTION__,##__VA_ARGS__); \
    }
#define QJError(argfmt, ...) \
    { \
printf("\r\nQJError:*****************************\n\
**[Function]:%s\n\
**[File]:%s\n**" argfmt "\
\n************************************\r\n\n",  __FUNCTION__, __FILE__,##__VA_ARGS__); \
    }

#define QJPRINT_BUILD_TIME() \
    {\
        HLog("Build Time -- %s %s.\Version: %s\n", __DATE__, __TIME__, INNER_VERSION);\
    }

#ifdef QJ_DEBUG
#define QJASSERT assert
#else
#define QJASSERT (true)&&
#endif

#ifdef QJ_DEBUG
#define QJDELETE(obj) \
    QJASSERT(obj); \
    delete obj; \
    obj = nullptr;
#else
#define QJDELETE(obj) \
    if( obj ) \
{ \
    delete obj; \
    obj = nullptr; \
    }
#endif

#define VA_ARGS_NUM(...) std::tuple_size<decltype(std::make_tuple(__VA_ARGS__))>::value




