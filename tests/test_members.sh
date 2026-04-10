#!/usr/bin/env bash
# =============================================================================
#  TEST 2 — Member Management
#  Tests: Register member, View all members, Duplicate email rejection
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"
LMS="$BUILD_DIR/LMS"
DB="$BUILD_DIR/library.db"

cd "$BUILD_DIR"


RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

pass() { echo -e "  ${GREEN}✔ PASS${RESET}  $1"; }
fail() { echo -e "  ${RED}✘ FAIL${RESET}  $1"; FAILURES=$((FAILURES+1)); }
info() { echo -e "  ${CYAN}ℹ${RESET}  $1"; }

FAILURES=0

echo -e "\n${BOLD}${YELLOW}══════════════════════════════════════════${RESET}"
echo -e "${BOLD}${YELLOW}   TEST SUITE 2 — MEMBER MANAGEMENT       ${RESET}"
echo -e "${BOLD}${YELLOW}══════════════════════════════════════════${RESET}\n"

rm -f "$DB"

# ─────────────────────────────────────────────────────────────────────────────
# TC-M-01: Register a single member
# ─────────────────────────────────────────────────────────────────────────────
echo -e "${BOLD}TC-M-01: Register a member${RESET}"

"$LMS" << 'EOF' > /dev/null 2>&1
2
1
Alice Johnson
alice@example.com
0
0
EOF

COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM members WHERE name='Alice Johnson';")
if [ "$COUNT" = "1" ]; then
  pass "Member 'Alice Johnson' inserted into DB"
else
  fail "Member not found (count=$COUNT)"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-M-02: Register multiple members
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-M-02: Register multiple members${RESET}"

"$LMS" << 'EOF' > /dev/null 2>&1
2
1
Bob Smith
bob@example.com
1
Carol White
carol@example.com
0
0
EOF

COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM members;")
if [ "$COUNT" = "3" ]; then
  pass "3 members total in DB"
else
  fail "Expected 3 members, got $COUNT"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-M-03: View all members (output check)
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-M-03: View all members shows all entries${RESET}"

OUTPUT=$("$LMS" << 'EOF' 2>&1
2
2
0
0
EOF
)

if echo "$OUTPUT" | grep -q "Alice Johnson"; then
  pass "Output contains 'Alice Johnson'"
else
  fail "Output missing 'Alice Johnson'"
fi

if echo "$OUTPUT" | grep -q "Bob Smith"; then
  pass "Output contains 'Bob Smith'"
else
  fail "Output missing 'Bob Smith'"
fi

if echo "$OUTPUT" | grep -q "carol@example.com"; then
  pass "Output contains Carol's email"
else
  fail "Output missing Carol's email"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-M-04: Duplicate email rejected
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-M-04: Duplicate email rejected${RESET}"

BEFORE=$(sqlite3 "$DB" "SELECT COUNT(*) FROM members;")

"$LMS" << 'EOF' > /dev/null 2>&1
2
1
Alice Duplicate
alice@example.com
0
0
EOF

AFTER=$(sqlite3 "$DB" "SELECT COUNT(*) FROM members;")
if [ "$BEFORE" = "$AFTER" ]; then
  pass "Duplicate email correctly rejected (count unchanged: $AFTER)"
else
  fail "Duplicate email was allowed! Before=$BEFORE After=$AFTER"
fi

# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}${YELLOW}──────────────────────────────────────────${RESET}"
echo -e "${BOLD}Members in DB: $(sqlite3 "$DB" "SELECT COUNT(*) FROM members;")${RESET}"
if [ "$FAILURES" -eq 0 ]; then
  echo -e "${GREEN}${BOLD}ALL MEMBER TESTS PASSED ✔${RESET}"
else
  echo -e "${RED}${BOLD}$FAILURES TEST(S) FAILED ✘${RESET}"
fi
echo -e "${BOLD}${YELLOW}──────────────────────────────────────────${RESET}\n"

exit $FAILURES
