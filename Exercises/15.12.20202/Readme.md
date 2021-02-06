# TASK
- Use the framework.c file to write characters in the online classroom
driver (module) that implements functions for:
  - module initialization (module_init),
  - open,
  - read,
  - release and
  - end of module (module_exit).
- Each time you read (eg cat / dev / <module name>) the driver displays
how many times have we already opened it (e.g. "I've been open 3 times."). Note that
each call to the cat command invokes open, read, and release functions
module.
- Write a short test program that will open the module (file / dev / <name
module>), read the message from it and then close it (close).
- Use the printk() function and the dmesg terminal command to debug.
