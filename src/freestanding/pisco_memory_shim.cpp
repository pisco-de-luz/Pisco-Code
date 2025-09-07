// src/platform/freestanding/pisco_memory_shim.cpp
#if defined(PISCO_FREESTANDING)
#include <stddef.h>

extern "C" __attribute__((weak)) void*
memcpy(void* dst, const void* src, size_t n)
{
    unsigned char*       d = static_cast<unsigned char*>(dst);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    for (size_t i = 0; i < n; ++i)
        d[i] = s[i];
    return dst;
}

extern "C" __attribute__((weak)) void*
memset(void* s, int c, size_t n)
{
    unsigned char*      p = static_cast<unsigned char*>(s);
    const unsigned char v = static_cast<unsigned char>(c);
    for (size_t i = 0; i < n; ++i)
        p[i] = v;
    return s;
}

extern "C" __attribute__((weak)) void*
memmove(void* dst, const void* src, size_t n)
{
    unsigned char*       d = static_cast<unsigned char*>(dst);
    const unsigned char* s = static_cast<const unsigned char*>(src);
    if (d == s || n == 0)
        return dst;
    if (d < s)
    {
        for (size_t i = 0; i < n; ++i)
            d[i] = s[i];
    }
    else
    {
        for (size_t i = n; i; --i)
            d[i - 1] = s[i - 1];
    }
    return dst;
}

extern "C" __attribute__((weak)) int
memcmp(const void* a, const void* b, size_t n)
{
    const unsigned char* x = static_cast<const unsigned char*>(a);
    const unsigned char* y = static_cast<const unsigned char*>(b);
    for (size_t i = 0; i < n; ++i)
    {
        if (x[i] != y[i])
            return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}
#endif
