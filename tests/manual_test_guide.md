# LMS Manual Testing Guide

This guide walks you through verifying the Book, Member, Loan, and Reservation Management modules in your terminal. It includes expected text printouts and references terminal screenshots.

---

## 📅 Due Date & Fines Simulation

### 1. How is the due date set?
When you issue a book, the system automatically sets the due date to exactly **14 days** after the current date.

### 2. How to simulate overdue fines?
We have added a custom feature directly in the console UI:
1. Issue a book: **`3. Loan Management`** ➔ **`1. Issue Book`**.
2. Select **`4. Update Loan Due Date`**.
3. Type the Loan ID and enter a backdated date (e.g., `2026-05-01`).
4. Select **`2. Return Book`**. The system calculates the overdue period and prints the calculated fine based on the active strategy!

---

## 🚀 Step-by-Step CLI Walkthrough

### Step 1: Clean and Seed the Database
To run a clean test session, compile the binary and initialize the DB by running:
```bash
cd /home/kanishkagarwal/Documents/LMS
bash tests/run_interactive.sh
```
This automatically handles compilation, deletes previous database states, runs `schema.sql` and `seed.sql`, and starts the LMS application.

---

### Step 2: Main Menu & Book Verification
The application starts with the Main Menu shell.

![LMS CLI Main Menu](../images/lms_main_menu.png)

#### A. View Existing Seeded Books
1. Type **`1`** (Book Management) and press Enter.
2. Type **`2`** (View All Books).
3. **Expected Output:**
   ```text
   ============================================
      ALL BOOKS
   ============================================

     ID    Title                           Author                  ISBN            Status
     -------------------------------------------------------------------------------------
     1     Clean Code                      Robert C. Martin        9780132350884   Available
     2     Let us c                        Yashwant Kanetkar       9788183331630   Available
     3     Design Patterns                 GoF                     9780201633610   Available
   ```
4. Press Enter to go back to the Book menu.

#### B. Search a Book
1. Type **`3`** (Search Book by Title).
2. Enter keyword: `Code`
3. **Expected Output:**
   ```text
   ============================================
      SEARCH BOOK BY TITLE
   ============================================
     Enter keyword: Code

     ID    Title                           Author                  ISBN            Status
     -------------------------------------------------------------------------------------
     1     Clean Code                      Robert C. Martin        9780132350884   Available
   ```
4. Press Enter. Type **`0`** to return to the Main Menu.

---

### Step 3: Loan Management & Verification
Now we will issue a book to a member.

#### A. View Seeded Members
1. From Main Menu, type **`2`** (Member Management).
2. Type **`2`** (View All Members).

![All Members](../images/lms_member.png)

3. **Expected Output:**
   ```text
   ============================================
      ALL MEMBERS
   ============================================

     ID    Name                            Email
     --------------------------------------------------
     1     Alice                           alice@gmail.com
     2     Bob                             bob@gmail.com
     3     Carol                           carol@gmail.com
   ```
4. Press Enter, and type **`0`** to return to the Main Menu.

#### B. Issue Book 2 to Member 1 (Alice)
1. Type **`3`** (Loan Management) ➔ **`1`** (Issue Book).
2. Enter Book ID: `2`
3. Enter Member ID: `1`

![LMS Issue Book success](../images/lms_issue_book.png)

4. **Expected Output:**
   ```text
   ============================================
      ISSUE BOOK
   ============================================
     Enter Book ID: 2
     Enter Member ID: 1

     Book issued successfully!
     Loan ID: 1
     Issue Date: 2026-06-05
     Due Date: 2026-06-19
   ```
5. Press Enter.

#### C. Try Double Checkout
1. Select **`1`** (Issue Book) again.
2. Enter Book ID: `2` (the book is currently checked out to Alice).
3. Enter Member ID: `2` (Bob).
4. **Expected Output:**
   ```text
   [WARN] Book ID 2 is not available.
   ```
5. Press Enter, then select **`0`** to return to the Main Menu.

---

### Step 4: Reservation & Observer Notifications
Since Book 2 is checked out, Bob can reserve it. On return, Bob will be notified.

#### A. Reserve Book 2 for Bob (Member ID 2)
1. From Main Menu, select **`4`** (Reservation Management).
2. Select **`1`** (Reserve a Book).
3. Enter Book ID: `2`
4. Enter Member ID: `2`
5. **Expected Output:**
   ```text
   ============================================
      RESERVE BOOK
   ============================================
     Enter Book ID: 2
     Enter Member ID: 2

     Book reserved successfully! Reservation ID: 1
   ```
6. Press Enter.

#### B. View Reservations for Book 2
1. Select **`2`** (View Reservations for a Book).
2. Enter Book ID: `2`

![Active Reservations](../images/lms_reservation.png)

3. **Expected Output:**
   ```text
   ============================================
      RESERVATIONS FOR BOOK #2
   ============================================

     Reservation #1  Member #2  Date: 2026-06-05
   ```
4. Press Enter. Select **`0`** to return to the Main Menu.

#### C. Return Book 2 and Trigger Observer Notifications
1. Select **`3`** (Loan Management).
2. Select **`2`** (Return Book).
3. Enter Loan ID: `1`

![LMS Return Book with Observer Alert](../images/lms_return_book.png)

4. **Expected Output:**
   ```text
   ============================================
      RETURN BOOK
   ============================================
     Enter Loan ID: 1

     Book returned successfully!
     Return Date: 2026-06-05
     No fine — returned on time.

   Notification for Bob
   Book Available:
   Let us c

     [NOTIFICATION - Admin Desk] Book Available:
   Let us c
   ```
5. Press Enter.

#### D. Issue to the Reserving Member
1. Select **`1`** (Issue Book).
2. Enter Book ID: `2`
3. Enter Member ID: `2` (Bob).
4. **Expected Output:**
   ```text
   Book issued successfully!
   Loan ID: 2
   ```
5. Select **`0`** to go back to Main Menu.
6. Select **`0`** to Exit.
7. **Expected Output:** `Goodbye!`

---

## 🛠 Troubleshooting

If you see an `[ERROR] SQL Error: database is locked`, it means another instance of LMS is running in the background. Verify there are no hanging CLI runs by checking:
```bash
ps aux | grep LMS
```
Kill any matching processes.
