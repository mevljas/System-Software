# Task 1
- Write a program in C that copies one file to another. Both file names are specified as command line arguments.
Copy the file one block (512 bytes) at a time.
- If the target file does not exist, create it. If it does, let rewrite it.
- Before copying, check:
- whether the program has 2 arguments,
- is the first file readable (can it be opened for reading?) And
- Is the other file writable (can it be written to?)
- otherwise print the appropriate message and end the program.

# Task 2
- Write your version of the cat terminal command using
read, write, open and close.
- The program should print the content of files specified via command line arguments to standard output (stdout).
- If the program is called without arguments, it copies stdin to
stdout.
- Multiple input arguments (files) can also be specified.


# Task 3
- Write a program in C that prints the contents of the file in reverse
order to another file.
- The names of both files are given as program arguments.
- For each system call, check to see if it can be made,
otherwise, print an error message and end the program.
- The program must work for "arbitrarily" long files: it's not
allowed to read entire files into working memory.
Use lseek.
- Program test:  
./naloga1 a.txt b.txt  
./naloga1 b.txt c.txt  
diff a.txt c.txt
# Task 4
- Write a C program tail that prints the last n lines of text file whose name is given as an input argument.
- The program should work similarly to the tail.
- Example of a program call: rep -n4 a.txt
The default value for n should be 5.
