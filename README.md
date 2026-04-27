# Task A - Vulnerability Discovery and Remediation

## Objective

Task A demonstrates a real buffer-overflow weakness in a C access-control program, then validates a secure remediation. The vulnerable program uses unsafe input handling (`gets`) that can corrupt adjacent memory, while the fixed program enforces bounded input and keeps authorization state safe.

## Files

- `vulnerable.c` - intentionally vulnerable login/admin check demo
- `fixed.c` - remediated secure version with bounded input and safe authorization behavior

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

## Evidence Walkthrough (Screenshots)

### Figure A1 - Compile evidence

**Image file:** `succesfull_compile_output_1-8546b839-8be2-4b50-8d48-24ef27ba015a.png`  
Shows successful compilation of the vulnerable binary and warnings that `gets` is unsafe/deprecated.

![Figure A1 - Compile output](</workspaces/TASK-A/succesfull_compile_output_1-8546b839-8be2-4b50-8d48-24ef27ba015a.png>)

### Figure A2 - Vulnerable program normal run

**Image file:** `shows_vulnerable_run_with_normal_admin_input_2-12abe263-ebf0-4286-b7f5-5020ea147995.png`  
Run of `./vulnerable` with a normal username (`destiny`) showing baseline variable prints before exploitation.

![Figure A2 - Vulnerable normal run](</workspaces/TASK-A/shows_vulnerable_run_with_normal_admin_input_2-12abe263-ebf0-4286-b7f5-5020ea147995.png>)

### Figure A3 - Buffer overflow proof (vulnerable)

**Image file:** `full_terminal_output_proving_overflow_evidence_3-9c7d4f51-076d-4765-b07d-58b80bff42dd.png`  
Long input overflows `username`, `isAdmin` changes to `0x41414141`, and the program terminates with segmentation fault. This is direct memory-corruption evidence.

![Figure A3 - Overflow evidence](</workspaces/TASK-A/full_terminal_output_proving_overflow_evidence_3-9c7d4f51-076d-4765-b07d-58b80bff42dd.png>)

### Figure A4 - Fixed program valid admin

**Image file:** `ixed_program_accepts_valid_admin_correctly._4-1b48b21f-b8d7-410b-9efa-0f275047f22a.png`  
Run of `./fixed` with `admin`: `isAdmin` moves from `0` to `1` only after authorization check, then access is granted as expected.

![Figure A4 - Fixed valid admin](</workspaces/TASK-A/ixed_program_accepts_valid_admin_correctly._4-1b48b21f-b8d7-410b-9efa-0f275047f22a.png>)

### Figure A5 - Fixed program denies non-admin

**Image file:** `fixed_program_blocks_non-admin_user._5-b09ca7af-d025-4fde-bf36-621598ad098d.png`  
Run of `./fixed` with `student123`: `isAdmin` stays `0`, and output is `Access denied. Not an admin.`

![Figure A5 - Fixed denies non-admin](</workspaces/TASK-A/fixed_program_blocks_non-admin_user._5-b09ca7af-d025-4fde-bf36-621598ad098d.png>)

### Figure A6 - Fixed program handles overflow attempt safely

**Image file:** `fixed_program_safely_rejects_long_input._6-cb438fb3-b7ec-4f54-8038-3c12cdfec43f.png`  
Run of `./fixed` with oversized input: truncation is detected, `isAdmin` remains safe at `0`, and access is denied with no crash.

![Figure A6 - Fixed safe overflow handling](</workspaces/TASK-A/fixed_program_safely_rejects_long_input._6-cb438fb3-b7ec-4f54-8038-3c12cdfec43f.png>)

## Conclusion

The evidence set shows full vulnerability lifecycle coverage:

1. vulnerable build and unsafe API warning,
2. vulnerable runtime overflow and memory corruption,
3. secure fix behavior for valid admin,
4. secure denial for non-admin and oversized attacker input.

This demonstrates both technical exploitability and effective remediation.
