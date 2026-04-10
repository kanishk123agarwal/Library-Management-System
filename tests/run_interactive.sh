#!/usr/bin/env bash
# =============================================================================
#  Interactive Demo Mode
#  Sets up a clean, seeded database and launches the interactive LMS CLI
# =============================================================================

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$SCRIPT_DIR/../build"
LMS="$BUILD_DIR/LMS"
DB="$BUILD_DIR/library.db"

# Colors for nice console experience
RED='\033[0;31m'; GREEN='\033[0;32m'; YELLOW='\033[1;33m'
CYAN='\033[0;36m'; BOLD='\033[1m'; RESET='\033[0m'

echo -e "\n${BOLD}${CYAN}╔══════════════════════════════════════════════╗${RESET}"
echo -e "${BOLD}${CYAN}║    LMS — Interactive Manual Demo Mode        ║${RESET}"
echo -e "${BOLD}${CYAN}╚══════════════════════════════════════════════╝${RESET}\n"

# Verify binary exists
if [ ! -f "$LMS" ]; then
  echo -e "${RED}ERROR: LMS executable not found at $LMS${RESET}"
  echo -e "${YELLOW}Building the project first...${RESET}"
  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"
  cmake .. -DCMAKE_BUILD_TYPE=Debug
  make -j$(nproc)
fi

# Reset and seed database
echo -e "${YELLOW}Resetting and seeding the database...${RESET}"
rm -f "$DB"

if [ -f "$SCRIPT_DIR/../database/schema.sql" ] && [ -f "$SCRIPT_DIR/../database/seed.sql" ]; then
  sqlite3 "$DB" < "$SCRIPT_DIR/../database/schema.sql"
  sqlite3 "$DB" < "$SCRIPT_DIR/../database/seed.sql"
  echo -e "${GREEN}✔ Database initialized with schema and seed data!${RESET}\n"
else
  echo -e "${RED}Warning: schema.sql or seed.sql not found. App will start with empty database.${RESET}\n"
fi

echo -e "${BOLD}${YELLOW}Launching LMS in interactive mode.${RESET}"
echo -e "You can type options to add books, members, issues, and returns yourself!\n"
sleep 1.5

# Run LMS interactively
cd "$BUILD_DIR"
./LMS
