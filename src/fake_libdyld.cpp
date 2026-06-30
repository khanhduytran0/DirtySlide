#include <stddef.h>
#include <stdint.h>

extern "C" {
const char* __progname = nullptr;
int NXArgc = 0;
const char** NXArgv = nullptr;
const char** environ = nullptr;
}

struct ProgramVars {
	const void* mh;
	int* NXArgcPtr;
	const char*** NXArgvPtr;
	const char*** environPtr;
	const char** __prognamePtr;
};

struct ErrorResult {
	void* buffer;
};

using FuncLookup = bool (*)(const char*, void**);

struct [[clang::ptrauth_vtable_pointer(process_independent, address_discrimination, type_discrimination)]] FakeLibSystemHelpers {
	virtual uintptr_t version() const { return 7; }
	virtual void* malloc(size_t) const { return nullptr; }
	virtual void free(void*) const {}
	virtual size_t malloc_size(const void*) const { return 0; }
	virtual int vm_allocate(unsigned int, uintptr_t*, uintptr_t, int) const { return 1; }
	virtual int vm_deallocate(unsigned int, uintptr_t, uintptr_t) const { return 0; }
	virtual int pthread_key_create_free(unsigned long*) const { return 0; }
	virtual void* pthread_getspecific(unsigned long) const { return nullptr; }
	virtual int pthread_setspecific(unsigned long, const void*) const { return 0; }
	virtual void __cxa_atexit(void (*)(void*), void*, void*) const {}
	virtual void __cxa_finalize_ranges(const void*, unsigned int) const {}
	virtual bool isLaunchdOwned() const { return false; }
	virtual void os_unfair_recursive_lock_lock_with_options(void*, void*) const {}
	virtual void os_unfair_recursive_lock_unlock(void*) const {}
	[[noreturn]] virtual void exit(int) const
	{
		__builtin_trap();
		for (;;) {}
	}
	virtual const char* getenv(const char*) const { return nullptr; }
	virtual int mkstemp(char*) const { return -1; }
	virtual void os_unfair_recursive_lock_unlock_forked_child(void*) const {}
	virtual void setDyldPatchedObjCClasses() const {}
	virtual void run_async(void* (*)(void*), void*) const {}
	virtual void os_unfair_lock_lock_with_options(void*, void*) const {}
	virtual void os_unfair_lock_unlock(void*) const {}
	virtual void setDefaultProgramVars(ProgramVars& vars) const
	{
		vars.__prognamePtr = &__progname;
		vars.NXArgcPtr = &NXArgc;
		vars.NXArgvPtr = &NXArgv;
		vars.environPtr = &environ;
	}
	virtual FuncLookup legacyDyldFuncLookup() const { return nullptr; }
	virtual ErrorResult setUpThreadLocals(const void*, const void*) const { return { nullptr }; }
	virtual bool os_unfair_recursive_lock_trylock(void*) const { return true; }
};

namespace dyld4 {
__attribute__((visibility("hidden"), section("__TPRO_CONST,__dyld_apis")))
void* gAPIs = nullptr;
}

__attribute__((used, section("__DATA_CONST,__helper")))
static const FakeLibSystemHelpers sHelpers;

extern "C" void
_dyld_initializer(void)
{
}
