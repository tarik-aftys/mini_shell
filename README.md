# mini_shell
Mini-shell interactif en C, permettant l'exécution de commandes Unix avec support des redirections, pipes multiples, historique des commandes et commandes internes.

## ✅ Requirements

- **GNU Readline Library**  
  Required for command history and line editing.  
  To install on Ubuntu/Debian:

  ```bash
  sudo apt update
  sudo apt install libreadline-dev
# GCC Compiler
To compile the C sources.
# Features
🔹 Prompt
Displays the current user and working directory with color formatting

🔹 Command Execution
External commands via execvp()

Built-in commands handled internally

🔹 Parsing & Operators
Pipelines: |

Input redirection: <

Output redirection: >

Background execution: &

🔹 Built-in Commands
| Command   | Description                           |
| --------- | ------------------------------------- |
| `cd`      | Change current directory              |
| `pwd`     | Print current working directory       |
| `clear`   | Clear the terminal screen             |
| `exit`    | Exit the shell                        |
| `help`    | (optional) Display help (to add)      |
| `history` | View previous commands (via readline) |
# Build and Run
🔹 Compile with make
make
🔹 Run the shell
./mini_shell
#  Usage Examples
# List files
ls -l

# Pipeline example
ls | grep .c

# Output redirection
echo Hello > hello.txt

# Input redirection
cat < hello.txt

# Background process
sleep 5 &
