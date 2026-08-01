#include <mach-o/dyld.h>
#include <mach-o/getsect.h>
#include <mach-o/loader.h>
#include <mach-o/nlist.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/syscall.h>
#include <time.h>
#include <unistd.h>

#define TO_STR_HELPER(x) #x
#define TO_STR(x) TO_STR_HELPER(x)
#define SYSCALL(func) \
    __asm__( \
        ".global _" #func "\n\t" \
        "_" #func ":\n\t" \
        "mov x16, " TO_STR(SYS_##func) "\n\t" \
        "svc #0x80\n\t" \
        "b.lo Lret\n\t" \
        "pacibsp\n\t" \
        "stp x29, x30, [sp, #-0x10]!\n\t" \
        "mov x29, sp\n\t" \
        "bl _cerror_nocancel\n\t" \
        "mov sp, x29\n\t" \
        "ldp x29, x30, [sp], #0x10\n\t" \
        "retab\n\t" \
        "Lret:\n\t" \
        "ret\n\t" \
    );
#define WRAP_DYLD(func) \
    static void* dyld_##func; \
    __asm__( \
        ".global _" #func "\n\t" \
        "_" #func ":\n\t" \
        "adrp x16, _dyld_" #func "@PAGE\n\t" \
        "add x16, x16, _dyld_" #func "@PAGEOFF\n\t" \
        "ldr x16, [x16]\n\t" \
        "br x16\n" \
    );

SYSCALL(fileport_makeport)
WRAP_DYLD(__error)
WRAP_DYLD(__shared_region_check_np)
WRAP_DYLD(__shared_region_map_and_slide_2_np)
WRAP_DYLD(_exit)
WRAP_DYLD(_simple_dprintf)
WRAP_DYLD(cerror_nocancel)
WRAP_DYLD(close)
WRAP_DYLD(getpid)
WRAP_DYLD(open)
WRAP_DYLD(sleep_ns)
WRAP_DYLD(socket)
WRAP_DYLD(syscall)
#undef WRAP_DYLD

extern void _simple_dprintf(int __fd, const char *__fmt, ...);
extern void sleep_ns(uint64_t ns);
const char **environ;

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
    environ = envp;
}

__attribute__((visibility("default")))
void
_libSystem_initializer(int argc, const char *argv[], const char *envp[], const char *apple[],
    const void *vars)
{
	libSystem_initializer(argc, argv, envp, apple, vars);
}

int usleep(useconds_t usec) {
    sleep_ns(usec * 1000ull);
    return 0;
}

unsigned int sleep(unsigned int s) {
    return usleep(s * 1000000ull);
}

char* getenv(const char *key) {
    return 0;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (n--) {
        *d++ = *s++;
    }
    return dest;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++; s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    while (n > 0 && *s1 && (*s1 == *s2)) {
        s1++; s2++; n--;
    }
    if (n == 0) {
        return 0;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

// from litehook
void *litehook_find_symbol(const struct mach_header_64 *header, const char *symbolName) {
    struct symtab_command *symtabCommand = NULL;
    struct segment_command_64 *linkeditSegCommand = NULL;
    uint64_t slide = -1;
    uint32_t off = 0;
    for (uint32_t i = 0; i < header->ncmds && off < header->sizeofcmds; i++) {
        struct load_command *lc = (struct load_command *)((uintptr_t)header + sizeof(struct mach_header_64) + off);
        if (lc->cmd == LC_SYMTAB) {
            symtabCommand = (struct symtab_command *)lc;
        }
        else if (lc->cmd == LC_SEGMENT_64) {
            struct segment_command_64 *segCmd = (struct segment_command_64 *)lc;
            if (slide == -1) {
                slide = (uintptr_t)header - segCmd->vmaddr;
            }
            if (!strncmp(segCmd->segname, "__LINKEDIT", sizeof(segCmd->segname))) {
                linkeditSegCommand = segCmd;
            }
        }
        if (symtabCommand && linkeditSegCommand) break;
        off += lc->cmdsize;
    }
    if (!symtabCommand || !linkeditSegCommand) return NULL;
    uint8_t *linkedit = (uint8_t *)((uintptr_t)header + linkeditSegCommand->vmaddr);
    struct nlist_64 *syms = (struct nlist_64 *)(linkedit + (symtabCommand->symoff - linkeditSegCommand->fileoff));
    char *strtbl = (char *)(linkedit + (symtabCommand->stroff - linkeditSegCommand->fileoff));
    size_t strtblSize = symtabCommand->strsize;
    for (uint32_t i = 0; i < symtabCommand->nsyms; i++) {
        struct nlist_64 *symEntry = &syms[i];
        uint32_t stroff = symEntry->n_un.n_strx;
        if (stroff >= strtblSize || off == 0) {
            continue;
        }
        if ((symEntry->n_type & N_TYPE) != N_SECT) {
            continue;
        }
        const char* curSymbolName = &strtbl[stroff];
        if (curSymbolName[0] == '\x00') {
            continue;
        }
        if (!strcmp(curSymbolName, symbolName)) {
            return (void *)((uintptr_t)header + symEntry->n_value);
        }
    }
    return NULL;
}

__attribute__((constructor))
void libSystem_bindFromDyld(void) {
    uint64_t lr = ((uint64_t)__builtin_return_address(0)) & ~0x3fff;
    while (*(uint32_t*)lr != MH_MAGIC_64) {
        lr -= 0x4000;
    }
    struct mach_header_64 *mh = (void *)lr;
#define WRAP_DYLD(func) dyld_##func = litehook_find_symbol(mh, "_" #func);
    WRAP_DYLD(__error)
    WRAP_DYLD(__shared_region_check_np)
    WRAP_DYLD(__shared_region_map_and_slide_2_np)
    WRAP_DYLD(_exit)
    WRAP_DYLD(_simple_dprintf)
    WRAP_DYLD(cerror_nocancel)
    WRAP_DYLD(close)
    WRAP_DYLD(getpid)
    WRAP_DYLD(open)
    WRAP_DYLD(sleep_ns)
    WRAP_DYLD(socket)
    WRAP_DYLD(syscall)
#undef WRAP_DYLD
}
