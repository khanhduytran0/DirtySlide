<p align="center"><img src="dirtyslide.png" width="55%" alt="DirtySlide"></p>

# DirtySlide

Unprivileged → root macOS LPE. An 8-byte OOB read/write into kernel memory adjacent to a dyld
shared-cache page, from a missing in-page bounds check in the v5 slide walk
(`vm_shared_region_slide_page_v5`), reachable via syscall 536.

**Patched** in macOS 26.5.2 (`25F84`, `xnu-12377.121.10`). Vulnerable through the 26.5 betas
(`xnu-12377.120.72`).

Writeup: https://gracecondition.github.io/posts/dirtyslide/

## Build & run

```sh
make                 # builds the payload into ./dist
./dist/run.sh        # run on the target VM as an unprivileged user → root shell
```

Tested on macOS 26.5 (`25F5042g`) arm64 under Apple Virtualization (VMAPPLE). The physical
scan panics ~50% of runs; the guest reboots, just run again.
