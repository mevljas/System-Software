# TASK 1
Write the program upgrade from the previous exercise (command line), as follows,
to be resistant to the CTRL + C combination that sends to the current process
SIGINT signal.
- When the user presses the key combination CTRL + C, a warning should be displayed that
the SIGINT signal will be taken into account if the user presses this combination again in
within 3 seconds.
- If the program does not receive the SIGINT signal again within three seconds, the status will change
resets and again requires two CTRL + C pressures less than 3
seconds to abort the process.
- Hint: alarm function().
- Example  
$ ./ukaznaVrstica  
>>> ^ C  
To exit the program, press CTRL + C once within 3 seconds.  
^ C  
Goodbye!  

# TASK 2
Now expand your command line so that it can execute the commands that are
"connected" to each other by a tube.
- Instead of the sign |, we will use the string -> for the pipe
- Example:  
ps -ef -> grep root  
The ps program should be executed with the -ef argument and everything it prints on
standard output, is redirected to the writing end of the tube. Reading
the end of the tube is connected to the standard inlet from which it reads
program grep.
- Limit yourself to one pipe, ie: <command1> -> <command2>
- The tube exists between the parent and the child; the parent should perform the right side
(<command2>) and the child on the left (<command1>). Hint: the "parent" process is
the child of the process reading the user strings from the std. input (see exercise 3).
- Use dup2 to redirect std. input and output.
