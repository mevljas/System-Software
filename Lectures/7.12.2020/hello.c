#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("Dual BSD/GPL");
static int hello_init(void)     // se izvede pri isnmod
{
    // Funkcija, ki je definirana v jedru - print kernel.
    // Podobno kto prtintf
    printk(KERN_ALERT "Hello, world\n");
    return 0;
}
static void hello_exit(void)        // se izvede pri rmmod
{
    printk(KERN_ALERT "Goodbye, cruel world\n");
}

module_init(hello_init);
module_exit(hello_exit);
