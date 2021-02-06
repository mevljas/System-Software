# TASK
Write the TCP server and TCP client code with
sockets.
- The client sends strings to the server (read from
standard input) in which the arithmetic operations are
(+, -, *, /) over two operands (eg 5 + 7).
- The server should read them, decipher (recommended use
function sscanf), calculates and sends back the result
(can be as a number or as a string). Calculation should be
also enabled for non-integers (double), e.g. “1.2 *
3.4 “. The server accepts connections in an infinite loop.
- The server and client can be on the same computer,
therefore, use address 127.0.0.1 and any port that
are specified as a command line parameter.

# TASK - UPGRADE
Upgrade the program so that the server can serve more
requirements at the same time using the thread.
- After each accepted connection, have the server create a new thread,
which communicates with a specific client. It prints
let also the IP and port of the client.
- Determine the number of threads yourself. When this number is reached, let
the server waits for all threads to finish before again
accept new connections. Another option is to lead
records of free threads and new threads are created after
need and not just at the end of all.
- The server and client can be on the same computer,
therefore, use address 127.0.0.1 and any port that
are specified as a command line parameter.
