#!/usr/bin/env bash
# =============================================================================
#  MASTER TEST RUNNER — Library Management System
#  Runs all test suites and prints a final summary
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"
LMS="$BUILD_DIR/LMS"

RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

# ─── Pre-flight checks ────────────────────────────────────────────────────────
echo -e "\n${BOLD}${CYAN}╔══════════════════════════════════════════════╗${RESET}"
echo -e "${BOLD}${CYAN}║    LMS — Automated Test Suite Runner         ║${RESET}"
echo -e "${BOLD}${CYAN}╚══════════════════════════════════════════════╝${RESET}\n"

if [ ! -f "$LMS" ]; then
  echo -e "${RED}ERROR: LMS executable not found at $LMS${RESET}"
  echo -e "${YELLOW}Build the project first:${RESET}"
  echo -e "  cd $(dirname "$SCRIPT_DIR") && mkdir -p build && cd build && cmake .. && make"
  exit 1
fi

if ! command -v sqlite3 &>/dev/null; then
  echo -e "${RED}ERROR: sqlite3 not found. Install with:${RESET}"
  echo -e "  sudo apt install sqlite3"
  exit 1
fi

echo -e "${GREEN}✔ LMS binary found: $LMS${RESET}"
echo -e "${GREEN}✔ sqlite3 found: $(which sqlite3)${RESET}\n"

# Make all test scripts executable
chmod +x "$SCRIPT_DIR"/test_*.sh

# ─── Run all suites ───────────────────────────────────────────────────────────
SUITES=(
  "test_books.sh:Book Management"
  "test_members.sh:Member Management"
  "test_loans.sh:Loan Management"
  "test_reservations.sh:Reservation Management"
  "test_patterns.sh:Design Patterns"
)

TOTAL_PASS=0
TOTAL_FAIL=0
FAILED_SUITES=()

for entry in "${SUITES[@]}"; do
  SCRIPT="${entry%%:*}"
  NAME="${entry##*:}"

  echo -e "${BOLD}Running: $NAME${RESET}"
  echo -e "${CYAN}$SCRIPT_DIR/$SCRIPT${RESET}"

  if bash "$SCRIPT_DIR/$SCRIPT"; then
    echo -e "${GREEN}✔ $NAME — PASSED${RESET}\n"
    TOTAL_PASS=$((TOTAL_PASS+1))
  else
    EC=$?
    echo -e "${RED}✘ $NAME — FAILED ($EC test(s) failed)${RESET}\n"
    TOTAL_FAIL=$((TOTAL_FAIL+1))
    FAILED_SUITES+=("$NAME")
  fi
done

# ─── Final Summary ────────────────────────────────────────────────────────────
echo -e "${BOLD}${CYAN}╔══════════════════════════════════════════════╗${RESET}"
echo -e "${BOLD}${CYAN}║              FINAL TEST RESULTS              ║${RESET}"
echo -e "${BOLD}${CYAN}╚══════════════════════════════════════════════╝${RESET}"
echo -e "  ${GREEN}Suites passed : $TOTAL_PASS${RESET}"
echo -e "  ${RED}Suites failed : $TOTAL_FAIL${RESET}"

if [ ${#FAILED_SUITES[@]} -gt 0 ]; then
  echo -e "\n  ${RED}Failed suites:${RESET}"
  for s in "${FAILED_SUITES[@]}"; do
    echo -e "    ${RED}✘ $s${RESET}"
  done
fi

echo ""
if [ "$TOTAL_FAIL" -eq 0 ]; then
  echo -e "${GREEN}${BOLD}  ✔ ALL TEST SUITES PASSED!${RESET}\n"
  exit 0
else
  echo -e "${RED}${BOLD}  ✘ SOME TESTS FAILED — see output above${RESET}\n"
  exit 1
fi
