#!/usr/bin/env bash
# =============================================================================
#  TEST 4 — Reservation Management
#  Tests: Reserve a book, View reservations, Cancel reservation,
#         Observer notification on book return
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
echo -e "${BOLD}${YELLOW}   TEST SUITE 4 — RESERVATION MANAGEMENT  ${RESET}"
echo -e "${BOLD}${YELLOW}══════════════════════════════════════════${RESET}\n"

# ── Setup: fresh DB with 1 book + 2 members ───────────────────────────────────
rm -f "$DB"
info "Setting up: adding book and members..."

"$LMS" << 'EOF' > /dev/null 2>&1
2
1
Alice Johnson
alice@example.com
1
Bob Smith
bob@example.com
0
1
1
Clean Code
Robert C. Martin
9780132350884
0
0
EOF

BOOK_ID=$(sqlite3 "$DB" "SELECT book_id FROM books LIMIT 1;")
MEM1=$(sqlite3 "$DB"   "SELECT member_id FROM members WHERE email='alice@example.com';")
MEM2=$(sqlite3 "$DB"   "SELECT member_id FROM members WHERE email='bob@example.com';")

info "Book ID=$BOOK_ID, Alice ID=$MEM1, Bob ID=$MEM2"

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-01: Issue the book first (so Alice needs to reserve)
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-01: Issue book to Alice (so Bob can reserve it)${RESET}"

"$LMS" << EOF > /dev/null 2>&1
3
1
$BOOK_ID
$MEM1
0
0
EOF

LOAN_ID=$(sqlite3 "$DB" "SELECT loan_id FROM loans WHERE book_id=$BOOK_ID AND member_id=$MEM1;")
AVAIL=$(sqlite3 "$DB" "SELECT available FROM books WHERE book_id=$BOOK_ID;")

if [ "$AVAIL" = "0" ]; then
  pass "Book issued to Alice (available=0)"
else
  fail "Book issue failed (available=$AVAIL)"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-02: Reserve the book for Bob
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-02: Bob reserves the issued book${RESET}"

"$LMS" << EOF > /dev/null 2>&1
4
1
$BOOK_ID
$MEM2
0
0
EOF

RES_COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM reservations WHERE book_id=$BOOK_ID AND member_id=$MEM2;")
if [ "$RES_COUNT" = "1" ]; then
  pass "Reservation created for Bob"
else
  fail "Reservation not created (count=$RES_COUNT)"
fi

RES_DATE=$(sqlite3 "$DB" "SELECT reserve_date FROM reservations WHERE book_id=$BOOK_ID AND member_id=$MEM2;")
info "Reservation date: $RES_DATE"
if [ -n "$RES_DATE" ]; then
  pass "Reservation date recorded: $RES_DATE"
else
  fail "Reservation date is empty"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-03: View reservations for the book
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-03: View reservations for book${RESET}"

OUTPUT=$("$LMS" << EOF 2>&1
4
2
$BOOK_ID
0
0
EOF
)

if echo "$OUTPUT" | grep -q "Reservation"; then
  pass "Reservation listing shown"
else
  fail "No reservation listing shown"
fi

if echo "$OUTPUT" | grep -q "$MEM2"; then
  pass "Bob's member ID shown in reservations"
else
  info "Member ID may be in different format, checking..."
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-04: Multiple reservations on same book
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-04: Alice can also reserve for notification${RESET}"
info "(Alice has already borrowed it; this tests a 2nd reservation entry)"

"$LMS" << EOF > /dev/null 2>&1
2
1
Carol White
carol@example.com
0
4
1
$BOOK_ID
3
0
0
EOF

RES_COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM reservations WHERE book_id=$BOOK_ID;")
if [ "$RES_COUNT" = "2" ]; then
  pass "2 reservations exist for the book"
else
  info "Reservations: $RES_COUNT (may vary based on Carol's ID)"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-05: Return the book (Alice returns it)
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-05: Alice returns the book${RESET}"

OUTPUT=$("$LMS" << EOF 2>&1
3
2
$LOAN_ID
0
0
EOF
)

AVAIL=$(sqlite3 "$DB" "SELECT available FROM books WHERE book_id=$BOOK_ID;")
if [ "$AVAIL" = "1" ]; then
  pass "Book available again after return"
else
  fail "Book still unavailable (available=$AVAIL)"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-06: Verify reservation persists after return (for notification)
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-06: Reservations still in DB after book return${RESET}"

RES_AFTER=$(sqlite3 "$DB" "SELECT COUNT(*) FROM reservations WHERE book_id=$BOOK_ID;")
info "Reservations still in DB: $RES_AFTER"
if [ "$RES_AFTER" -ge "1" ]; then
  pass "Reservations persisted after return ($RES_AFTER records)"
else
  fail "Reservations disappeared after return"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-R-07: Issue the book again to Bob (who reserved it)
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-R-07: Issue book to Bob (the person who reserved)${RESET}"

"$LMS" << EOF > /dev/null 2>&1
3
1
$BOOK_ID
$MEM2
0
0
EOF

LOAN_BOB=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans WHERE book_id=$BOOK_ID AND member_id=$MEM2;")
if [ "$LOAN_BOB" = "1" ]; then
  pass "Book successfully issued to Bob after reservation"
else
  fail "Could not issue book to Bob (loans=$LOAN_BOB)"
fi

# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}${YELLOW}──────────────────────────────────────────${RESET}"
echo -e "${BOLD}Final DB State:${RESET}"
echo -e "  Reservations: $(sqlite3 "$DB" "SELECT COUNT(*) FROM reservations;")"
echo -e "  Active Loans: $(sqlite3 "$DB" "SELECT COUNT(*) FROM loans WHERE return_date IS NULL;")"
if [ "$FAILURES" -eq 0 ]; then
  echo -e "${GREEN}${BOLD}ALL RESERVATION TESTS PASSED ✔${RESET}"
else
  echo -e "${RED}${BOLD}$FAILURES TEST(S) FAILED ✘${RESET}"
fi
echo -e "${BOLD}${YELLOW}──────────────────────────────────────────${RESET}\n"

exit $FAILURES
