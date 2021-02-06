# TASK
- Write a driver for the character device that takes care of periodically
lighting the LED light relative to the switch.
- Use GPIO connectors: The LED should be connected to GPIO 15
(pin 10) and the switch to GPIO 14 (pin 8).
  - LED light connection: attach the light to the wire and then the resistor in series. one connect the light pin to GPIO 15 and the other via a resistor to GND (eg pin 9).
  - "Switch" connection: the switch is a wire with a resistor. Tie one end on GPIO 14 (pin 8) the other via a resistor either at +3.3 V (eg pin 1) either at GND (e.g. pin 6). It depends on the binding of the other end of the wire switch value (1 or 0).
- When the switch is unlocked (connected to GND), the LED should light up alternately turns on and off. Each of the two conditions should last 2 seconds.
- When the switch is closed (connected to 3.3 V), the LED should light up turns on and off for 0.5 seconds, taking into account more the value the user writes to the module (see next page).

- Implement the write function on the module (write).
The user writes 1 byte to the module. The value of this byte is
takes into account when the switch is closed.
- If the user wrote a byte with a value of 0 on the module and is
switch closed, LED does not flash. If the user writes 1,
the LED flashes for 0.5 seconds.
  - You can write to the module as:  
  echo -n -e '\ x00'> / dev / module or  
  echo -n -e '\ x01'> / dev / module.  
- Periodic turning the light on and off is coordinated by the core
timer (see example in lectures).

- In the module initialization function:
  - register the character driver (register_chrdev)
  - we require a piece of memory at address 0x3F200000 size 0xB4
  (request_mem_region). The latter may fail if this piece of memory is already there
  occupied by another GPIO driver, which is common. That is why we do not take failure
  too serious and just print a notification with printk ().
  - map physical addresses to virtual (ioremap)
  - take care of setting the mode of operation of GPIO pins (GPIO 15 is the output, GPIO
  14 pa entrance)
  ▪ initialize the core timer (timer_setup) and set the trigger interval
  (mod_timer); hint: jiffies and msecs_to_jiffies () functions.


- In the module removal function:
  - log off the character driver (unregister_chrdev)
  - free a piece of memory, but only if it has been successfully seized before
  (release_mem_region)
  - clear GPIO pin settings (set the corresponding GPFSEL bits to 0)
  - remove the timer (del_timer)
- In the function that is called when the timer_callback is triggered:
  - check switch value (GPIO 14)
  - Based on the switch value, sets the appropriate period for the next trigger
  clock (mod_timer)
  - turn the LED on or off (depending on the value of the switch and the value
  written byte by the user)

- In the write function in the module:
  - We read the byte from the user memory, based on which enable the LED (byte is 1) or disable it (byte is 0).
- ATTENTION!  
You should never tie any wire between two GPIOs
connection if there is no resistance in between! Use e.g. 220 Ohm.
