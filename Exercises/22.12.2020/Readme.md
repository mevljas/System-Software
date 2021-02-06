# TASK
- Write a driver for the character device you can use by
writing a string (when calling write) and from which this string can then
we also read.
. When the module is initialized
(module_init), book
(kmalloc) and initialize
(memset) memory desired
size (e.g. 512 B).
- When the module is removed (module_exit), release the seized one
memory (kfree).
- Make sure the function called when writing returns a number
successfully written bytes (or 0 if not written
none, and a negative value in the event of an error).
- Test the driver: ▪ with the echo (echo –n "test string"> / dev / module) and cat ▪ commands with a special test program.

# TASK - UPGRADE
- Upgrade the program from the previous task so that
the character device looks like memory divided into
N_BLK blocks, where each size is BLK_SIZE (specified
Optional).
- Reading and writing should be quantized. Ob
each block is always written one block at a time
at once. So if we want to write 20 bytes and it's a block
large 10 bytes, will write 10 bytes when writing.
- Printk as well for each read / write operation
number of bytes (lazy) and offset (offset) so we can see
the course of quantized reading / writing.
- When writing, the module should allocate memory in real time - as needed. Size N_BLK * BLK_SIZE must not exceed.
- Test the module with the echo and cat commands.
