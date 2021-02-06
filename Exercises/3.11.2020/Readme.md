# TASK
- Write a program that acts as a simple command line and in infinite
loop executes the entered commands.
- The program ends by pressing CTRL + D.
- Read the entire command from the standard input to the character string. You can
assume that the command string is no longer than 1024 characters. You can read
use fgets() or just read().
- For each command, have the process create a child by calling fork(), whichs
calls execv() or execvp() to execute the command.
- Enable execution of commands with arguments.
- Break the command by spaces: replace spaces with '\0' and point pointers at the beginning of subsets
which you pass later to the execvp() function show.
- You can also use the strtok() function from the library instead of manual parsing
string.h.
- You can assume that there will be no more than 64 arguments.
