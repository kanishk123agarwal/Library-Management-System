-- ===========================================================================
-- Library Management System — SQLite Schema
-- Day 6: Database Design
-- ===========================================================================

PRAGMA foreign_keys = ON;

-- ---------------------------------------------------------------------------
-- Books Table
-- ---------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS books (
    book_id   INTEGER PRIMARY KEY AUTOINCREMENT,
    title     TEXT    NOT NULL,
    author    TEXT    NOT NULL,
    isbn      TEXT    NOT NULL UNIQUE,
    available INTEGER NOT NULL DEFAULT 1   -- 1 = available, 0 = issued
);

-- ---------------------------------------------------------------------------
-- Members Table
-- ---------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS members (
    member_id INTEGER PRIMARY KEY AUTOINCREMENT,
    name      TEXT NOT NULL,
    email     TEXT NOT NULL UNIQUE
);

-- ---------------------------------------------------------------------------
-- Loans Table
-- ---------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS loans (
    loan_id    INTEGER PRIMARY KEY AUTOINCREMENT,
    book_id    INTEGER NOT NULL,
    member_id  INTEGER NOT NULL,
    issue_date TEXT    NOT NULL,          -- ISO-8601: YYYY-MM-DD
    due_date   TEXT    NOT NULL,          -- ISO-8601: YYYY-MM-DD
    return_date TEXT   DEFAULT NULL,      -- NULL means not yet returned
    FOREIGN KEY (book_id)   REFERENCES books(book_id),
    FOREIGN KEY (member_id) REFERENCES members(member_id)
);

-- ---------------------------------------------------------------------------
-- Reservations Table
-- ---------------------------------------------------------------------------
CREATE TABLE IF NOT EXISTS reservations (
    reservation_id INTEGER PRIMARY KEY AUTOINCREMENT,
    book_id        INTEGER NOT NULL,
    member_id      INTEGER NOT NULL,
    reserve_date   TEXT    NOT NULL,      -- ISO-8601: YYYY-MM-DD
    FOREIGN KEY (book_id)   REFERENCES books(book_id),
    FOREIGN KEY (member_id) REFERENCES members(member_id)
);
