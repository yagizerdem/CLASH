# CLASH

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Example Usage](#example-usage)
- [Build & Run](#build--run)
- [Architecture](#architecture)
  - [Lexer](#lexer)
  - [Parser](#parser)
  - [Expansion](#expansion)
  - [Execution](#execution)
- [Built-in Commands](#built-in-commands)
- [Variables and Special Parameters](#variables-and-special-parameters)
- [PATH Resolution and Cache](#the-path-variable-and-its-cache)
- [Testing](#testing)
- [License](#license)

## Overview

CLASH is a minimal Unix-style shell implemented in C/C++, designed to replicate the core behavior of traditional shells such as Bash while remaining simple and readable. The project focuses on accurately modeling how shells parse input, expand variables, construct pipelines, and execute processes at the OS level.

The shell reads user input, tokenizes it, builds command and pipeline structures, performs environment and quote expansion, and finally executes commands using low-level system calls such as `fork`, `execve`, `pipe`, and `dup2`.

CLASH is not a wrapper around existing shell utilities. It implements its own lexer, parser, and execution logic to provide a clear, educational view of how a Unix shell works internally.


## Features
- Interactive command-line interface with prompt support
- Command parsing with a custom lexer and parser
- Support for pipelines (`|`) with proper file descriptor chaining
- Input and output redirection (`<`, `>`)
- Environment variable expansion (`$VAR`, `$?`)
- Proper handling of single and double quotes
- Built-in commands executed without spawning external processes
- External command execution using `fork` and `execve`
- Exit status tracking and propagation across pipelines
- Error handling for syntax errors and execution failures

### Shell Invocation Modes

CLASH supports multiple startup and execution modes:

- **Interactive Mode**  
  If CLASH is started with no command-line arguments and standard input is a terminal, it runs in interactive mode.  
  A prompt is displayed, and commands are read line-by-line from standard input.  
  Incomplete commands (e.g. unmatched quotes) trigger continuation input until completion.

- **Standard Input Mode (Non-Interactive)**  
  If standard input is not a terminal and no command-line arguments are provided, CLASH reads commands from `stdin` without displaying prompts.  
  This allows CLASH to be used in scripts and pipelines.

- **Command String Mode (`-c`)**  
  If the first argument is `-c`, CLASH executes the following argument as an inline shell script and then exits.  
  If `-c` is provided without a command string, CLASH reads commands from the file specified by the next positional argument.

- **Script File Mode**  
  If CLASH is invoked without `-c` and the first positional argument is a file name, CLASH reads commands from that file instead of standard input and exits at end-of-file.


## Example Usage

### Interactive Mode

```bash
$ ./clash
> echo "hello world"
hello world

> echo hello | wc -w
1

> export NAME=clash
> echo $NAME
clash
```

### Standard Input (Non-Interactive)
```bash
$ echo "echo hi" | ./clash
hi
```

### Command String Execution (-c)
```bash
$ ./clash -c "echo one; echo two"
one
two
```

### Command String Execution (-c) With positional arguments
```bash
$ ./clash -c "echo $1 $2 $3" arg1 arg2 arg3
arg1 arg2 arg3
```

### Script File Execution
```bash 
$ cat script.txt
echo "running script"
ls | wc -l

$ ./clash script.txt
running script
42
```

### Redirection and Pipelines
```bash 
> ls *.cpp > files.txt
> cat files.txt | wc -l
10
```

> **Note**  
> The examples above demonstrate only the basic usage patterns of CLASH.  
> In practice, CLASH supports significantly more complex shell programs, including multi-stage pipelines, nested command substitutions, variable-driven scripts, and mixed usage of redirection, quoting, and expansion rules within the same command stream.


## Build & Run

### Prebuilt Binary (Debian)

A precompiled **Debian-compatible** CLASH binary is included directly in the repository under dist folder.


### Build from Source

CLASH is implemented in C++ and uses CMake as its build system.

```bash
git clone https://github.com/yagizerdem/CLASH.git
cd CLASH
mkdir build && cd build
cmake ..
make
```


## Architecture

CLASH is structured to closely mirror the internal architecture of a real Unix shell.  
The implementation is divided into clear, sequential stages that transform raw input
into executing processes while preserving Bash-like semantics.

The execution flow follows this high-level pipeline:

**Input - Lexer - Substitution - Parser - Glob Expansion - Execution**

---

### Lexer

The lexer performs a left-to-right scan over the raw input stream and converts it into
a sequence of tokens. It is responsible for recognizing:

- Word boundaries
- Operators (`|`, `<`, `>`, `;`)
- Quoting constructs (single and double quotes)
- Escape sequences (backslashes)

The lexer does **not** perform substitutions or expansions. Its sole responsibility
is to preserve enough structural information so that higher stages can correctly
apply shell semantics.

---

### Parser

The parser consumes the token stream and builds structured command representations.

Responsibilities include:

- Grouping words into commands
- Constructing pipelines from `|`-separated commands
- Detecting and associating input/output redirections
- Validating basic syntax errors (e.g. missing redirection targets)

The parser outputs a command model that represents **what should be executed**, but
not **how it should be expanded or run**.

---

### Substitution && Globbing

The expansion stage applies all shell substitutions in the correct order, matching
Bash-like behavior:

- Tilde expansion (`~`, `~user`)
- Variable expansion (`$VAR`, `$0`, `$1`, `$?`, `$*`, `$#`)
- Command substitution using backquotes
- Quote-aware backslash handling
- Word splitting caused by expansions
- Path (glob) expansion (`*`, `?`, `[a-z]`)

This stage is quote-sensitive and ensures that quoted characters suppress splitting
and globbing exactly as in a real shell.

---

### Execution

The execution stage turns expanded commands into running processes.

Key responsibilities:

- Executing built-in commands directly within the shell process
- Spawning external commands using `fork` and `execve`
- Setting up pipelines using `pipe` and `dup2`
- Applying input/output redirections
- Passing exported variables to child processes
- Waiting for subprocesses and collecting exit statuses

Exit status propagation follows Bash semantics, including correct handling of pipelines
(where `$?` reflects the status of the final command).

## Built-in Commands

CLASH implements a set of built-in commands that are executed directly by the shell
process, without spawning a new subprocess. This behavior matches standard Unix shell
semantics and is required for commands that modify the shell’s internal state.

The following built-in commands are supported:

### `cd [dir]`

Changes the current working directory of the shell.

- If `dir` is provided, the shell changes to that directory.

```bash
cd /tmp
```

### `export varName1 varName2`

Marks one or more variables for export so they are passed to child processes as
environment variables.

```bash
export PATH HOME
```

### `unset varName1 varName2`

Removes one or more variables from the shell’s variable table.

```bash
unset TEMP_VAR
```

### Variable Assignment

CLASH supports variable assignment as a special built-in operation.

```bash
NAME=value
COUNT=10
```

### `exit status`

Causes clash to exit, returning status as its exit status. If no status argument is provided, then the exit status will be 0.


```bash
exit 0
```

## Variables and Environment Variables

Clash maintains a table of variable bindings, which are used for variable substitutions. Each variable has a string name and a string value. The following variable names have special meanings defined by clash:


## Variables and Special Parameters

CLASH maintains a table of variables that are used for substitution and execution.
The following variables have special meanings and behaviors:

- **`HOME`**  
  The current user's home directory.  
  Used for tilde (`~`) expansion.

- **`PATH`**  
  A colon-separated list of directories used to locate executable files.

- **`$0`**  
  The shell name or script context:
  - If CLASH is invoked with no arguments, `$0` is the value of `argv[0]`
    (the program name).
  - If invoked with the `-c` option, `$0` is the argument immediately following
    the command string, if provided; otherwise it defaults to `argv[0]`.
  - If invoked with a script file, `$0` is the name of that file.

- **`$1`, `$2`, ...**  
  Positional parameters corresponding to command-line arguments following `$0`.

- **`$#`**  
  The number of positional parameters following `$0`.

- **`$*`**  
  All positional parameters following `$0`, concatenated into a single string
  and separated by spaces.

- **`$?`**  
  The exit status of the most recently executed command.  
  For pipelines, this reflects the exit status of the final command in the pipeline.



When clash starts, it creates one variable for each environment variable received from its parent. When clash creates subprocesses, it passes some of its variables to the subprocess as environment variables. These variables are referred to as exported. When clash creates initial variable bindings from its environment, it marks each of these variables as exported. The export built-in command can be used to mark additional variables as exported.

## The PATH Variable and its Cache

The PATH variable is used by clash to locate executables for commands. It consists of any number of directory names separated by colons. To execute a command, clash searches each of the directories in the PATH variable to see if they contain an executable file whose name is the same as the first word of the command. If so, the first matching file that is executable is used (if no executable file is found, the first non-executable one is used).

However, searching through all the directories in the PATH variable is too expensive to perform for each command execution. Instead, you must keep a cache of mappings from command names to executable file names. You must ensure that the cache is updated whenever the PATH variable changes (you do not need to update the cache when files are created or deleted in directories in the path).

If no PATH variable exists, then use "/usr/local/bin:/usr/local/sbin:/usr/bin:/usr/sbin:/bin:/sbin" as the default.

Note: the PATH mechanism is only used if the first word of a command contains no "/" characters. If the first word contains a "/" character, then the word is used directly as the name of the executable, bypassing the PATH mechanism.

## Testing

CLASH is extensively tested using the **Google Test (gtest)** framework.  
The test suite is designed to validate shell behavior at a fine-grained level,
with a strong focus on correctness, edge cases, and Bash-like semantics.

### Test Coverage

Tests are organized by functional area and cover both common and pathological cases:

- **Lexer**
  - Splitting commands from raw user input
  - Splitting words from raw user input
  - Identifying escaped and quoted globbing characters
  -Determining word context (single-quoted, unquoted, double-quoted)

- **Expansion & Substitution (LTR Scanner)**
  - Variable expansion (`$VAR`, `$0`, `$1`, `$?`, `$#`, `$*`)
  - Boundary-sensitive variable parsing
  - Empty and undefined variables
  - Quote-aware expansion rules
  - Command substitution using backticks
  - Prevention of recursive expansion in a single pass

- **Globbing**
  - Basic wildcard expansion (`*`, `?`, `[a-z]`)
  - Interaction with quotes and variables
  - No-match behavior

- **Parser**
  - Command classification
  - Standard input/output redirection detection
  - Command-line arguments parsing

- **Execution**
  - Process spawning
  - Pipeline execution
  - Exit status propagation
  - Built-in vs external command handling

### Stress and Edge Case Testing

The test suite includes a large number of **stress tests** and **edge cases**, such as:

- Adjacent backticks inside words
- Mixed quoting and substitution in the same token
- Variables containing whitespace, newlines, Unicode, and special characters
- Nested and multiple command substitutions
- Invalid syntax detection with proper exception handling
- Behavior differences between quoted and unquoted expansions

### Example Test (Google Test)

```cpp
TEST(LTRSCANNER, MixedExpansionStress) {
    Env* env = Env::getInstance();
    env->setEnv("USER", "yagiz");
    env->setEnv("CITY", "istanbul");

    LTR_scanner scanner;

    ASSERT_EQ(
        scanner.singlePass("echo '$USER' \"$USER lives in $CITY\" `echo ok`"),
        "echo '$USER' \"yagiz lives in istanbul\" ok"
    );
}
```



## License

This project is licensed under the **MIT License**.