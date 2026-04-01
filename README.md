# Minimal Shell

Industry-level C shell for university assignment.

## Features Supported
- External commands via execvp
- Pipes (`|`)
- Redirections (`>`, `>>`, `<`)
- Background jobs (`&`) with automatic reaping
- Built-ins: `cd`, `exit`, `jobs`
- SIGINT (Ctrl+C) gracefully cancels current input only
- Empty/blank lines ignored (no crash)
- All syscalls error-checked with messages to stderr
- Valgrind clean (no leaks)

## How to Build and Run
```bash
make
./myshell
