# TASK (1/4)
Write three programs: generator, broker, and
recorder.
Programs use shared memory and message type
To send messages to each other: 
- The shared memory is used by the generator and
messaging agent. Access to shared
memory must be synchronized using
semaphore.
- The message type is used by the broker and
the recorder, namely the broker sends the messages
to the recorder.
- The generator and broker must use the same key for
access to shared memory and semaphore. Also
the broker and the recorder use the same key for
message type (use ftok() or write
any number).

# TASK (2/4)
Generator
:
- Creates a piece of shared memory and a semaphore. 
Also set semaphore to a initial value.
- Writes a shared message every 2 seconds
memory. The message should have its own number and
some arbitrary text. Example of two messages: 
  - ID 1: This is a message. 
  - ID 2: This is the message.
- Send 5 messages. The last (5th) should be empty ('\0').
- Before writing to shared memory, the program must
lock the semaphore and must unlock it after writing.
Wait another 2 seconds before unlocking. So:
the program spends 2 seconds outside the semaphore and 2
seconds inside semaphore (protected area).

# TASK (3/4)
Mediator
:
- Create a new message type. Get it
semaphore identifier and shared memory.
- Access shared memory for one second
and read the content. Access is coordinated with
semaphore generator.
- The content is then sent to the message queue, however
only if it was different from the previous one. With that
we want to ensure that they are in the message queue only
unique messages.
- The read and send loop ends when it
reads an empty message (5th message
generator). It also sends a blank message
forward to the message queue. It then clears the semaphore
field and shared memory and ends.

# TASK (4/4)
Recorder:
- Opens a write file - the name is given as
input argument. Obtain an identifier
message types.
- Reads messages from the queue and writes them to
file.
- When you receive a blank message, close the file
deletes the message queue and ends.  
Useful:  
terminal commands ipcs(status) and ipcrm(delete)
