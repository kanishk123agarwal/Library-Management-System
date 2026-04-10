#!/usr/bin/env bash
# =============================================================================
#  TEST 1 — Book Management
#  Tests: Add book, View all books, Search book, Delete book
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"
LMS="$BUILD_DIR/LMS"
DB="$BUILD_DIR/library.db"

cd "$BUILD_DIR"


# ── Color codes ──────────────────────────────────────────────────────────────
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

pass() { echo -e "  ${GREEN}✔ PASS${RESET}  $1"; }
fail() { echo -e "  ${RED}✘ FAIL${RESET}  $1"; FAILURES=$((FAILURES+1)); }
info() { echo -e "  ${CYAN}ℹ${RESET}  $1"; }

FAILURES=0

echo -e "\n${BOLD}${YELLOW}══════════════════════════════════════════${RESET}"
echo -e "${BOLD}${YELLOW}   TEST SUITE 1 — BOOK MANAGEMENT         ${RESET}"
echo -e "${BOLD}${YELLOW}══════════════════════════════════════════${RESET}\n"

# Fresh DB for each test suite
rm -f "$DB"

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-01: Add a single book
# ─────────────────────────────────────────────────────────────────────────────
echo -e "${BOLD}TC-B-01: Add a book${RESET}"

"$LMS" << 'EOF' > /dev/null 2>&1
1
1
Clean Code
Robert C. Martin
9780132350884
0
0
EOF

COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM books WHERE title='Clean Code';")
if [ "$COUNT" = "1" ]; then
  pass "Book 'Clean Code' inserted into DB"
else
  fail "Book not found in DB (count=$COUNT)"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-02: Add multiple books
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-B-02: Add multiple books${RESET}"

"$LMS" << 'EOF' > /dev/null 2>&1
1
1
The Pragmatic Programmer
David Thomas
9780201616224
1
Design Patterns
Gang of Four
9780201633610
0
0
EOF

COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM books;")
if [ "$COUNT" = "3" ]; then
  pass "3 books total in DB"
else
  fail "Expected 3 books, got $COUNT"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-03: View all books (output check)
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-B-03: View all books shows all entries${RESET}"

OUTPUT=$("$LMS" << 'EOF' 2>&1
1
2
0
0
EOF
)

if echo "$OUTPUT" | grep -q "Clean Code"; then
  pass "Output contains 'Clean Code'"
else
  fail "Output missing 'Clean Code'"
fi

if echo "$OUTPUT" | grep -q "Design Patterns"; then
  pass "Output contains 'Design Patterns'"
else
  fail "Output missing 'Design Patterns'"
fi

if echo "$OUTPUT" | grep -q "Available"; then
  pass "Books show 'Available' status"
else
  fail "No availability status in output"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-04: Search book by title keyword
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-B-04: Search book by keyword${RESET}"

OUTPUT=$("$LMS" << 'EOF' 2>&1
1
3
Pragmatic
0
0
EOF
)

if echo "$OUTPUT" | grep -q "Pragmatic"; then
  pass "Search result contains 'Pragmatic'"
else
  fail "Search did not find 'Pragmatic'"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-05: Search returns no results for unknown keyword
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-B-05: Search with no results${RESET}"

OUTPUT=$("$LMS" << 'EOF' 2>&1
1
3
XYZNONEXISTENT
0
0
EOF
)

if echo "$OUTPUT" | grep -q "No books found"; then
  pass "No results message shown correctly"
else
  fail "Expected 'No books found' message"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-06: Delete a book
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-B-06: Delete a book${RESET}"

BOOK_ID=$(sqlite3 "$DB" "SELECT book_id FROM books WHERE title='Design Patterns';")
info "Deleting book_id=$BOOK_ID (Design Patterns)"

"$LMS" << EOF > /dev/null 2>&1
1
4
$BOOK_ID
0
0
EOF

COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM books WHERE title='Design Patterns';")
if [ "$COUNT" = "0" ]; then
  pass "Book 'Design Patterns' deleted from DB"
else
  fail "Book still exists in DB"
fi

# ─────────────────────────────────────────────────────────────────────────────
# TC-B-07: ISBN uniqueness constraint
# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}TC-B-07: Duplicate ISBN rejected${RESET}"

BEFORE=$(sqlite3 "$DB" "SELECT COUNT(*) FROM books;")

"$LMS" << 'EOF' > /dev/null 2>&1
1
1
Duplicate Book
Someone
9780132350884
0
0
EOF

AFTER=$(sqlite3 "$DB" "SELECT COUNT(*) FROM books;")
if [ "$BEFORE" = "$AFTER" ]; then
  pass "Duplicate ISBN correctly rejected (count unchanged: $AFTER)"
else
  fail "Duplicate ISBN was allowed! Before=$BEFORE After=$AFTER"
fi

# ─────────────────────────────────────────────────────────────────────────────
echo -e "\n${BOLD}${YELLOW}──────────────────────────────────────────${RESET}"
FINAL_BOOK_COUNT=$(sqlite3 "$DB" "SELECT COUNT(*) FROM books;" 2>/dev/null || echo "?")
echo -e "${BOLD}Book Test Results: $FINAL_BOOK_COUNT book(s) remaining in DB${RESET}"
if [ "$FAILURES" -eq 0 ]; then
  echo -e "${GREEN}${BOLD}ALL BOOK TESTS PASSED ✔${RESET}"
else
  echo -e "${RED}${BOLD}$FAILURES TEST(S) FAILED ✘${RESET}"
fi
echo -e "${BOLD}${YELLOW}──────────────────────────────────────────${RESET}\n"

exit $FAILURES
