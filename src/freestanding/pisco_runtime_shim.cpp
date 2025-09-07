#if defined(PISCO_FREESTANDING)
extern "C" void
__cxa_pure_virtual() noexcept
{
    for (;;)
    {
    }
}
extern "C" void
__cxa_deleted_virtual() noexcept
{
    for (;;)
    {
    }
}
#endif