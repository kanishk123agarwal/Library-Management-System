-- ===========================================================================
-- Library Management System — Seed Data
-- Day 6: Sample data for testing
-- ===========================================================================

-- Sample Books
INSERT OR IGNORE INTO books (title, author, isbn, available) VALUES
('Clean Code',             'Robert C. Martin', '9780132350884', 1),
('The Pragmatic Programmer','David Thomas',    '9780201616224', 1),
('Design Patterns',        'Gang of Four',     '9780201633610', 1),
('Effective C++',          'Scott Meyers',     '9780321334879', 1),
('Introduction to Algorithms','CLRS',          '9780262033848', 1);

-- Sample Members
INSERT OR IGNORE INTO members (name, email) VALUES
('Alice Johnson', 'alice@example.com'),
('Bob Smith',     'bob@example.com'),
('Carol White',   'carol@example.com');
