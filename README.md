# Task A - Vulnerability Discovery and Remediation

## Objective

Task A demonstrates a real buffer-overflow weakness in a C access-control program, then validates a secure remediation. The vulnerable program uses unsafe input handling (`gets`) that can corrupt adjacent memory, while the fixed program enforces bounded input and keeps authorization state safe. Static analysis with Flawfinder is also used as independent automated evidence of risk reduction.

## Files

- `vulnerable.c` - intentionally vulnerable login/admin check demo
- `fixed.c` - remediated secure version with bounded input and safe authorization behavior
- `image evidence/` - screenshots used as proof in this README

## What was vulnerable

In `vulnerable.c`, `username` is a 16-byte field and input is read with `gets`, which has no length limit. A long payload can overflow the buffer and overwrite nearby variables such as `isAdmin`, causing privilege errors or crashes.

## What was fixed

`fixed.c` keeps the same business logic and admin allowlist (`root`, `admin`, `superuser`) but replaces unsafe input with bounded `fgets`, detects overlong input, flushes extra bytes, and preserves `isAdmin` integrity so unauthorized users are denied.

## Reproduction Commands

```bash
gcc -fno-stack-protector -z execstack -o vulnerable vulnerable.c
./vulnerable

gcc -o fixed fixed.c
./fixed
```

## Static Analysis with Flawfinder

Flawfinder was run on both versions to provide independent, automated evidence of the security improvement.

### Install Flawfinder (Ubuntu)

```bash
sudo apt update
sudo apt install -y flawfinder
flawfinder --version
```

### Run scans

```bash
flawfinder vulnerable.c
flawfinder fixed.c
```

### Findings summary

- `vulnerable.c`:
  - **Level 5 (highest risk)** finding on `gets` (CWE-120, CWE-20): "Does not check for buffer overflows. Use fgets() instead."
  - **Level 2** finding on the statically-sized `char` buffer (CWE-119/CWE-120).
  - Total: **2 hits**, including 1 at level 5.
- `fixed.c`:
  - **No** level 5 findings.
  - Remaining items are **low-risk advisories** (level 1-2) on `strlen`, `getchar`, and the static buffer, which are normal for any C program using bounded input.
  - Total: **3 advisories**, all low-risk.

This shows the original critical buffer-overflow risk has been **eliminated**, and only baseline advisories remain in the fixed code.

## Evidence Walkthrough (Screenshots)

> All screenshots referenced below are stored in the `image evidence/` folder next to this README so they render correctly on GitHub.

### Figure A1 - Compile evidence (vulnerable)

Shows successful compilation of the vulnerable binary and the linker/compiler warnings that `gets` is unsafe and dangerous.

![Figure A1 - Compile output](image evidence/succesfull_compile_output_1.png)

### Figure A2 - Vulnerable program normal run

Run of `./vulnerable` with a normal username showing baseline variable prints (buffer size, `isAdmin` initial value) before any exploitation.

![Figure A2 - Vulnerable normal run](image evidence/shows_vulnerable_run_with_normal_admin_input_2.png)

### Figure A3 - Buffer overflow proof (vulnerable)

Long input overflows `username`, `isAdmin` changes to `0x41414141`, and the program terminates with a segmentation fault. This is direct memory-corruption evidence.

![Figure A3 - Overflow evidence](image evidence/full_terminal_output_proving_overflow_evidence_3.png)

### Figure A4 - Fixed program valid admin

Run of `./fixed` with `admin`: `isAdmin` moves from `0` to `1` only after the authorization check, then access is granted as expected.

![Figure A4 - Fixed valid admin](image evidence/ixed_program_accepts_valid_admin_correctly._4.png)

### Figure A5 - Fixed program denies non-admin

Run of `./fixed` with `student123`: `isAdmin` stays `0`, and the output is `Access denied. Not an admin.`

![Figure A5 - Fixed denies non-admin](image evidence/fixed_program_blocks_non-admin_user._5.png)

### Figure A6 - Fixed program handles overflow attempt safely

Run of `./fixed` with oversized input: truncation is detected, `isAdmin` remains safe at `0`, and access is denied with no crash.

![Figure A6 - Fixed safe overflow handling](image evidence/fixed_program_safely_rejects_long_input._6.png)

### Figure A7 - Flawfinder scan (before fix)

Flawfinder run on `vulnerable.c`. Reports a **level 5** dangerous-function finding on `gets` (CWE-120, CWE-20) plus a level 2 static-buffer advisory. Confirms the high-severity buffer-overflow risk independently.

![Figure A7 - Flawfinder before fix](image evidence/Flawfinder_before_fix.png)

### Figure A8 - Flawfinder scan (after fix)

Flawfinder run on `fixed.c`. The level 5 `gets` finding is gone; only **low-risk advisories** (level 1-2) remain. Confirms the remediation removed the critical defect.

![Figure A8 - Flawfinder after fix](image evidence/flawfinder_after_fix.png)

## How to set up the evidence folder for GitHub

## Conclusion

The evidence set shows full vulnerability lifecycle coverage:

1. vulnerable build and unsafe API warning,
2. vulnerable runtime overflow and memory corruption,
3. independent Flawfinder confirmation of the high-severity risk,
4. secure fix behavior for valid admin,
5. secure denial for non-admin and oversized attacker input,
6. Flawfinder confirmation that the critical defect is removed in the fixed version.

This demonstrates both technical exploitability and effective remediation, validated by manual runtime evidence and automated static analysis.
