#include <linux/kernel.h>
#include <linux/module.h>
#include <asm/msr.h>
#include <linux/timex.h>

MODULE_LICENSE("GPL");

int timer_init(void)
{
        unsigned long long ini, end;

        ini = rdtsc();        
        end = rdtsc();                
        printk("ini: %llu, end: %llu, end-ini: %llu\n", ini, end, end-ini);

        ini = get_cycles();
        end = get_cycles();
        printk("ini: %llu, end: %llu, end-ini: %llu\n", ini, end, end-ini);
        
        return 0;
}

void timer_exit(void)
{
        return;
}

module_init(timer_init);
module_exit(timer_exit);