# 🐚 Minishell  

```ansi
███╗   ███╗██╗███╗   ██╗██╗███████╗██╗  ██╗███████╗██╗     ██╗
████╗ ████║██║████╗  ██║██║██╔════╝██║  ██║██╔════╝██║     ██║
██╔████╔██║██║██╔██╗ ██║██║███████╗███████║█████╗  ██║     ██║
██║╚██╔╝██║██║██║╚██╗██║██║╚════██║██╔══██║██╔══╝  ██║     ██║
██║ ╚═╝ ██║██║██║ ╚████║██║███████║██║  ██║███████╗███████╗███████╗
╚═╝     ╚═╝╚═╝╚═╝  ╚═══╝╚═╝╚══════╝╚═╝  ╚═╝╚══════╝╚══════╝╚══════╝
```  

> A lightweight Bash-inspired shell implemented in C as part of the **42 School curriculum**.  
> It re-implements essential features of a Unix shell: parsing, AST execution, environment handling, pipes, redirections, heredocs, signals, and built-in commands.  

---

## ✨ Features  

- **Custom Prompt** (`Minishell$`) with history  
- **AST-based parsing** ensuring correct operator precedence  
- **Pipes & Redirections**: `<`, `>`, `>>`, `<<`  
- **Heredocs** with variable expansion and safe cleanup  
- **Environment Management** (`t_env_list` linked list)  
- **Variable Expansion**: `$VAR`, `$?`, `$$`  
- **Signal Handling**:  
  - `Ctrl-C` → interrupt and redraw prompt  
  - `Ctrl-D` → exit shell  
  - `Ctrl-\` → ignored in prompt  
- **Builtins**: `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`  

---

## 📂 Project Structure  

```
minishellv2/
├── Makefile
├── minishell.c        # entry point
├── minishell.h        # headers & structures
├── srcs/
│   ├── builtins/      # cd, echo, pwd, env, export, unset, exit
│   ├── env_var/       # environment list & helpers
│   ├── execute/       # executor, redirections, heredocs
│   ├── parse/         # tokenizer, parser, AST builder
│   ├── utils/         # syntax, helper functions, mini-libft
│   ├── banner.c       # ASCII art banner
│   ├── signals.c      # signal handling
│   ├── free.c         # cleanup functions
└── obj/               # compiled object files
```

---

## 🏗️ Architecture  

### 🔹 Parsing & Execution Flow  

1. **Input** is read via `readline()`.  
2. **Tokenizer** splits it into tokens (`WORD`, `PIPE`, `REDIR`, …).  
3. **Parser** validates syntax and builds an **AST** (`t_ast_node`).  
4. **Executor** walks the AST recursively:  
   - Runs builtins directly.  
   - Spawns child processes for external commands.  
   - Manages pipes and redirections.  

### 🔹 Example AST  

For the command:  

```bash
cat hello.txt | wc -l | grep foo
```

The AST looks like:

```
PIPE
  CMD: cat
    ARG: hello.txt
  PIPE
    CMD: wc
      ARG: -l
    CMD: grep
      ARG: foo
```

For a command with redirections:

```bash
echo hello > out.txt
```

The AST looks like:

```
CMD: echo
  ARG: hello
  REDIR_OUT: out.txt
```

---

## ⚙️ Installation  

Clone the repository and build:  

```bash
git clone https://github.com/JosephGhattas/My_Minishell.git
cd My_Minishell
make
```

Requirements:  
- GCC / Clang  
- GNU Readline library  

---

## 🚀 Usage  

Run the shell:  

```bash
./minishell
```

Examples:  

```bash
Minishell$ echo "Hello World"
Hello World

Minishell$ export NAME=Joseph
Minishell$ echo $NAME
Joseph

Minishell$ ls -l | grep minishell > out.txt
```

Exit with `Ctrl-D` or the `exit` builtin.  

---

## 🔎 Notes  

- Strictly follows **42 Minishell** requirements  
- No memory leaks (excluding `readline`)  
- Unsupported features: `&&`, `||`, wildcards, subshells  

---

## 👨‍💻 Author  

**Joseph Ghattas**  
42 Beirut — Chemistry student turned systems programmer  
