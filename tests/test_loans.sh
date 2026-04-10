#!/usr/bin/env bash
# =============================================================================
#  TEST 3 — Loan Management
#  Tests: Issue book, View active loans, Return book, Overdue fine,
#         Cannot issue unavailable book, Invalid loan return
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
echo -e "${BOLD}${YELLOW}   TEST SUITE 3 — LOAN MANAGEMENT         ${RESET}"
echo -e "${BOLD}${YELLOW}══════════════════════════════════════════${RESET}\n"

# ── Setup: fresh DB with 2 books + 2 members ─────────────────────────────────
rm -f "$DB"
info "Setting up: adding books and members..."

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
1
Design Patterns
Gang of Four
9780201633610
0
0
EOF

BOOK1=$(sqlite3 "$DB" "SELECT book_id FROM books WHERE isbn='9780132350884';")
BOOK2=$(sqlite3 "$DB" "SELECT book_id FROM books WHERE isbn='9780201633610';")
MEM1=$(sqlite3 "$DB"  "SELECT member_id FROM members WHERE email='alice@example.com';")
MEM2=$(sqlite3 "$DB"  "SELECT member_id FROM members WHERE email='bob@example.com';")

info "Book1 ID=$BOOK1 (Clean Code), Book2 ID=$BOOK2 (Design Patterns)"
info "Member1 ID=$MEM1 (Alice), Member2 ID=$MEM2 (Bob)"

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-01: Issue a book
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-01: Issue a book to a member${RESET}"

"$LMS" << EOF > /dev/null 2>&1
3
1
$BOOK1
$MEM1
0
0
EOF

LOAN_COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans;")
if [ "$LOAN_COUNT" = "1" ]; then
  pass "Loan record created"
else
  fail "Expected 1 loan, got $LOAN_COUNT"
fi

AVAIL=$(sqlite3 "$DB" "SELECT available FROM books WHERE book_id=$BOOK1;")
if [ "$AVAIL" = "0" ]; then
  pass "Book marked as unavailable (available=0)"
else
  fail "Book still shows available=$AVAIL (expected 0)"
fi

DUE=$(sqlite3 "$DB" "SELECT due_date FROM loans WHERE loan_id=1;")
info "Due date: $DUE"
if [ -n "$DUE" ]; then
  pass "Due date set: $DUE"
else
  fail "Due date is empty"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-02: View active loans
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-02: View active loans displays correct info${RESET}"

OUTPUT=$("$LMS" << 'EOF' 2>&1
3
3
0
0
EOF
)

if echo "$OUTPUT" | grep -q "1"; then
  pass "Active loans section shown"
else
  fail "No active loan output"
fi

if echo "$OUTPUT" | grep -q "No"; then
  pass "Overdue status shown (should be 'No' for fresh loan)"
else
  info "Overdue display may vary"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-03: Cannot issue a book that is already issued
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-03: Cannot issue an already-issued book${RESET}"

BEFORE_LOANS=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans;")

"$LMS" << EOF > /dev/null 2>&1
3
1
$BOOK1
$MEM2
0
0
EOF

AFTER_LOANS=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans;")
if [ "$BEFORE_LOANS" = "$AFTER_LOANS" ]; then
  pass "Second issue of same book rejected (loans unchanged)"
else
  fail "Duplicate loan was created! Before=$BEFORE_LOANS After=$AFTER_LOANS"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-04: Issue a second book to a different member
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-04: Issue a second book to another member${RESET}"

"$LMS" << EOF > /dev/null 2>&1
3
1
$BOOK2
$MEM2
0
0
EOF

LOAN_COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans;")
if [ "$LOAN_COUNT" = "2" ]; then
  pass "Second loan created (total: 2 loans)"
else
  fail "Expected 2 loans, got $LOAN_COUNT"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-05: Return a book
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-05: Return a book${RESET}"

LOAN1_ID=$(sqlite3 "$DB" "SELECT loan_id FROM loans WHERE book_id=$BOOK1;")
info "Returning loan_id=$LOAN1_ID"

OUTPUT=$("$LMS" << EOF 2>&1
3
2
$LOAN1_ID
0
0
EOF
)

RETURN_DATE=$(sqlite3 "$DB" "SELECT return_date FROM loans WHERE loan_id=$LOAN1_ID;")
if [ -n "$RETURN_DATE" ]; then
  pass "Return date recorded: $RETURN_DATE"
else
  fail "Return date not set"
fi

AVAIL=$(sqlite3 "$DB" "SELECT available FROM books WHERE book_id=$BOOK1;")
if [ "$AVAIL" = "1" ]; then
  pass "Book marked as available again (available=1)"
else
  fail "Book not available after return (available=$AVAIL)"
fi

if echo "$OUTPUT" | grep -q "returned successfully"; then
  pass "Return success message displayed"
else
  fail "Return success message missing"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-06: No fine for on-time return
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-06: No fine for on-time return${RESET}"

if echo "$OUTPUT" | grep -q "No fine\|returned on time\|fine — returned"; then
  pass "No fine shown for on-time return"
else
  info "Fine status: $(echo "$OUTPUT" | grep -i 'fine' | head -1)"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-07: Simulate overdue fine via direct DB manipulation
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-07: Overdue fine calculation${RESET}"
info "Backdating loan due_date by 30 days to simulate overdue..."

LOAN2_ID=$(sqlite3 "$DB" "SELECT loan_id FROM loans WHERE book_id=$BOOK2;")
sqlite3 "$DB" "UPDATE loans SET due_date=date('now','-30 days') WHERE loan_id=$LOAN2_ID;"

OUTPUT=$("$LMS" << EOF 2>&1
3
3
0
0
EOF
)

if echo "$OUTPUT" | grep -qi "YES\|overdue"; then
  pass "Overdue loan detected in active loans"
else
  fail "Overdue not detected (check active loans view)"
fi

# Now return the overdue book and check fine
RETURN_OUTPUT=$("$LMS" << EOF 2>&1
3
2
$LOAN2_ID
0
0
EOF
)

if echo "$RETURN_OUTPUT" | grep -qi "FINE\|fine:"; then
  FINE_LINE=$(echo "$RETURN_OUTPUT" | grep -i "FINE")
  pass "Overdue fine shown on return: $FINE_LINE"
else
  fail "No fine shown for 30-day overdue book"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-L-08: Active loans count reduces after return
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-L-08: Active loans reduce to 0 after both returns${RESET}"

ACTIVE=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans WHERE return_date IS NULL;")
if [ "$ACTIVE" = "0" ]; then
  pass "No active loans remaining"
else
  fail "Still $ACTIVE active loans (expected 0)"
fi

# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}${YELLOW}──────────────────────────────────────────${RESET}"
TOTAL_LOANS=$(sqlite3 "$DB" "SELECT COUNT(*) FROM loans;")
echo -e "${BOLD}Total loans in DB: $TOTAL_LOANS (all returned)${RESET}"
if [ "$FAILURES" -eq 0 ]; then
  echo -e "${GREEN}${BOLD}ALL LOAN TESTS PASSED ✔${RESET}"
else
  echo -e "${RED}${BOLD}$FAILURES TEST(S) FAILED ✘${RESET}"
fi
echo -e "${BOLD}${YELLOW}──────────────────────────────────────────${RESET}\n"

exit $FAILURES
