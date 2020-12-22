#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/jiffies.h>

MODULE_LICENSE("GPL");

int timer_init(void)
{
        unsigned long j, j2, stamp_1, stamp_3;
        
        j = jiffies;
        j2 = jiffies;
        stamp_1 = jiffies + HZ;
        stamp_3 = jiffies + 3*HZ;
        
        printk("HZ: %d, j: %lu, j2: %lu, stamp_1: %lu, stamp:3: %lu\n", HZ, j, j2, stamp_1, stamp_3);
        
        return 0;
}

void timer_exit(void)
{
        return;
}

module_init(timer_init);
module_exit(timer_exit);