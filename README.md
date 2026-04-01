# Custom Minimal Shell

A minimal Unix shell written in C.
Modular architecture, full syscall error handling, signal-safe, and Valgrind clean.

---

## Table of Contents
1. [Quick Start](#quick-start)
2. [Repository Structure](#repository-structure)
3. [How to Build and Run](#how-to-build-and-run)
4. [Supported Features](#supported-features)
5. [Architecture](#architecture)
6. [Test Cases](#test-cases)
7. [Known Limitations](#known-limitations)
8. [Requirements](#requirements)

---

## Quick Start
```bash
git clone https://github.com/Cyrus-Hossain/custom-minimal-shell.git
cd custom-minimal-shell
make
./myshell
```

---

## Repository Structure
```
minimal_shell/
├── Makefile          # Build system — strict flags, separate compilation, clean target
├── shell.h           # Shared structs (Command, Job), includes, defines, externs
├── main.c            # REPL loop, signal setup, input reading, cleanup
├── parser.c          # Tokenizes input, builds linked-list of Command structs
├── parser.h
├── executor.c        # Forks, pipes, redirects, and executes command pipelines
├── executor.h
├── builtins.c        # cd, exit, jobs — executed in parent process
├── builtins.h
├── job.c             # Background job linked list and SIGCHLD reaper
├── job.h
├── LICENSE
└── README.md
```

---

## How to Build and Run

**Build:**
```bash
make
```

**Run:**
```bash
./myshell
```

**Clean:**
```bash
make clean
```

---

## Supported Features

| Feature | Example |
|---|---|
| External commands | `ls -la` |
| Pipes | `ls \| grep .c` |
| Output redirection | `echo hello > out.txt` |
| Append redirection | `echo world >> out.txt` |
| Input redirection | `cat < input.txt` |
| Background jobs | `sleep 5 &` |
| List background jobs | `jobs` |
| Change directory | `cd /tmp` |
| Exit shell | `exit` |
| Ctrl+C handling | Cancels input, gives new prompt |
| Blank lines | Ignored, no crash |

---

## Architecture
```
User Input  (stdin)
     │
     ▼
  main.c
  • REPL loop via getline
  • Signal setup with sigaction (SIGINT, SIGCHLD)
  • Calls parser then executor
     │
     ▼
  parser.c
  • Tokenizes input with strtok_r (reentrant, safe)
  • Detects pipes, redirections, background flag
  • Builds linked list of Command structs
  • Returns NULL on empty input
     │
     ▼
  executor.c
  • Detects built-ins vs external commands
  • Launches pipelines with fork/pipe/dup2
  • Handles input/output redirection per command
  • Waits or backgrounds based on & flag
     │
     ├──► builtins.c
     │    • cd, exit, jobs
     │    • Run in parent process (no fork)
     │
     └──► job.c
          • Linked list of background jobs
          • SIGCHLD reaper removes finished jobs automatically
```

**Key design decisions:**
- Separate modules for parsing, execution, builtins, and job control keep each component focused and independently maintainable
- `sigaction` used over `signal()` — safer, more portable, industry standard
- `strtok_r` used over `strtok` — reentrant and thread-safe
- Every `malloc` paired with a `free` — confirmed with Valgrind

---

## Test Cases

| Command | What it tests |
|---|---|
| `ls -la` | Basic external command execution |
| `ls \| grep .c` | Pipe between two processes |
| `echo "hello" > input.txt` | Output redirection |
| `cat < input.txt > output.txt` | Input and output redirection together |
| `sleep 5 &` | Background job execution |
| `jobs` | Background job listing |
| `cd /tmp` | Built-in cd |
| Ctrl+C | SIGINT handling — new prompt, shell stays alive |
| Blank enter | Robustness — no crash |
| `exit` | Built-in exit and cleanup |

**Memory check:**
```bash
valgrind --leak-check=full --show-leak-kinds=all ./myshell
```
Result: 0 bytes definitely lost, 0 bytes indirectly lost.

---

## Known Limitations
- Operators must have spaces around them (`ls | grep` works, `ls|grep` does not)
- No quoted arguments containing spaces
- No command history or tab completion
- No `fg`/`bg` for background jobs
- Single pipeline per line only
- POSIX systems only (Linux, macOS, WSL)

---

## Requirements
- GCC
- GNU Make
- POSIX system (Linux / macOS / WSL on Windows)

---
