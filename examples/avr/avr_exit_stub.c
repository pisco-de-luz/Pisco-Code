__attribute__((noreturn)) void
exit(int code)
{
    (void) code;
    for (;;)
        ;
}

/* Some toolchains call _exit, keep a weak alias */
__attribute__((noreturn, weak)) void
_exit(int code)
{
    (void) code;
    for (;;)
        ;
}
