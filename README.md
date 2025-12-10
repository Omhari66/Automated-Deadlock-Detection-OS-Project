# Automated Deadlock Detection Tool (CSE316 – Operating Systems)
**Student:** Om Hari Shukla  
**Roll No:** 16  
**Course:** CSE316 – Operating Systems  
**University:** Lovely Professional University  

This project implements an **Automated Deadlock Detection and Recovery Tool** using C.  
It simulates the behavior of an Operating System when detecting and recovering from deadlocks through process termination.

---

## 📌 Features
- Accepts Allocation, Request, and Available matrices.
- Detects processes that are in deadlock.
- Allows user-driven **recovery by aborting processes**.
- Updated request & allocation matrices after recovery.
- Modular, clean, beginner-friendly C code.
- Includes 4 experimental test cases.

---

## 📂 Project Structure
The project is organized as follows:


---

## 🧩 Module-Wise Breakdown

### **1. Input Module**
- Takes number of processes (n) and resource types (r).
- Takes input for Allocation, Request, and Available matrices.
- Allocates memory dynamically using `malloc`.

### **2. Deadlock Detection Module**
- Implements OS Deadlock Detection Algorithm.
- Uses:
  - `work[]` → temporary resource array  
  - `finish[]` → marks processes finished or not  
- If `request[i][j] <= work[j]`, process is assumed to complete.
- Unfinished processes are reported as deadlocked.

### **3. Recovery Module**
- Allows user to abort a process.
- Releases all of its allocated resources back to Available[].
- Resets Allocation and Request for that process.
- Runs detection again to verify recovery.

---

## 🛠 Compilation Instructions (C Program)

### **Windows (MinGW or TDM-GCC)**
```bash
gcc os.c -o os
./os
