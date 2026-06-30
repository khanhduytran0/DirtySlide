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
