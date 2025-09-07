#if defined(PISCO_FREESTANDING)
extern "C" __attribute__((noreturn)) void
exit(int code)
{
    (void) code;
    for (;;)
        ;
}

/* Some toolchains call _exit, keep a weak alias */
extern "C" __attribute__((noreturn, weak)) void
_exit(int code)
{
    (void) code;
    for (;;)
        ;
}
#endif