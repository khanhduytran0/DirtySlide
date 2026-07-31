__attribute__((visibility("default")))
void
libSystem_initializer(int argc, const char *argv[], const char *envp[], const char *apple[],
    const void *vars)
{
	(void)argc;
	(void)argv;
	(void)envp;
	(void)apple;
	(void)vars;
}

__attribute__((visibility("default")))
void
_libSystem_initializer(int argc, const char *argv[], const char *envp[], const char *apple[],
    const void *vars)
{
	libSystem_initializer(argc, argv, envp, apple, vars);
}

char* getenv(const char *key) {
    return 0;
}

__attribute__((naked))
int open(const char *path, int flags, ...) {
    __asm__ volatile(
        "mov x16, #5\n"     // Syscall number for open on Darwin/iOS
        "svc #0\n"          // Trigger kernel trap
        "ret\n"             // Return to caller (result in x0)
    );
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}
