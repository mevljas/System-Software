# TASK 1
- Write a simplified version of the command *ls -la* in C, which displays the names of all files in the current
directory (".") as well as the file type (normal,
directory, socket, block, character, ...), accessible
permissions (mode) and the number of hard links
(st_nlink).
- Remember S_ISUID, S_ISGID and S_ISVTX.
- The use of "exec ls -la" is not allowed.
- Example printout:  
-rwsr-Sr-x 1 lsl  
-rw-r - r-- 1 lsl.c  
drwxr-xr-t 2.  
drwxr-xr-x 6 ..  
-rw ------ T 1 numbers  

# TASK 2
- Write a program myChmod that will work similarly to the terminal
command chmod.
- When starting the program, we give two arguments:
  - new permits (modes) and
  - the name of the file whose permissions we are changing.
- Permissions are given as an octal number (eg 0777, 0644, 4700) or
but as a three-letter string. We allow only four forms of strings:
- u + s (set bit "Set-User-ID")
- u-s (delete bit "Set-User-ID")
- g + s (set bit "Set-Group-ID")
- g-s (delete bit "Set-Group-ID")
- Example of program operation:  
$ ll test  
-rwxr ----- 1 student student 0 2012-11-05 10:47 test *  
$ ./mojChmod 0664 test  
New permissions: rw-rw-r--  
$ ./mojChmod g + s test  
New permissions: rw-rwSr--  
$ ./mojChmod 4700 test  
New licenses: rws ------  
$ ./mojChmod u-s test  
New licenses: rwx ------  
