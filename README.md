# Custom Minimal Shell

A custom minimal Unix shell. Contains modular files, full error handling, and zero memory leaks.

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
```

## Project Structure
- `main.c` — REPL loop and signal handling
- `parser.c/h` — tokenizes input, builds command linked list
- `executor.c/h` — forks, pipes, redirects, and executes commands
- `builtins.c/h` — cd, exit, jobs (run in parent process)
- `job.c/h` — background job linked list and SIGCHLD reaper
- `shell.h` — shared structs, defines, and declarations

## Known Limitations
- Operators must have spaces around them (`ls | grep` not `ls|grep`)
- No quoted arguments with spaces
- No command history or tab completion
- No fg/bg for background jobs

## Requirements
- GCC
- GNU Make
- POSIX system (Linux/macOS or WSL on Windows)

## Author
Cyrus Hossain
