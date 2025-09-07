// src/runtime/abort_handlers.cpp
extern "C" __attribute__((weak)) void
__cxa_pure_virtual() noexcept
{
    for (;;)
    {
    }
}

extern "C" __attribute__((weak)) void
__cxa_deleted_virtual() noexcept
{
    for (;;)
    {
    }
}
