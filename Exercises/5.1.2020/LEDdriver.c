#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define GPIO_BASE (0x3F000000 + 0x200000)
#define RANGE (0xB4)
#define DEVICE_NAME "LEDdriver"
#define LED 15
#define BUTTON 14

MODULE_LICENSE("GPL");

struct GpioRegisters{
	uint32_t GPFSEL[6]; // set pin mode (input/output/AF)
	uint32_t Reserved1;
	uint32_t GPSET[2]; // set pin to 1, (offset +28)
	uint32_t Reserved2;
	uint32_t GPCLR[2]; // clear pin to 0 (+40)
	uint32_t Reserved3;
	uint32_t GPLEV[2]; // get level of pin (+52)
};
struct GpioRegisters* pGpio;

int Major;
int got_mem_region = 0;
struct timer_list timer1;
unsigned char flashing; // ali lučka utripa (ko je button == 1)
unsigned char button_val;

int module_init_my(void); 
void module_cleanup_my(void); 
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *inode, struct file *file);
static ssize_t device_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char __user *, size_t , loff_t *);

module_init(module_init_my);
module_exit(module_cleanup_my);

static struct file_operations fops = {
	.owner   = THIS_MODULE,
	.open    = device_open,
	.release = device_release,
	.read    = device_read,
	.write   = device_write
};


void SetGPIO_Function(int GPIO, int mode){
	//GPFSEL
	int regIdx = GPIO / 10; // register
	int bit = (GPIO % 10) *3; // zamik
	uint32_t oldVal = pGpio->GPFSEL[regIdx];
	uint32_t mask = 0b111 << bit;
	pGpio->GPFSEL[regIdx] = (oldVal & ~mask) | ((mode << bit) & mask);
}

void SetGPIO_Out(int GPIO, bool val){
	int regIdx = GPIO / 32;
	if (val)
		pGpio->GPSET[regIdx] = (1 << (GPIO % 32));
	else
		pGpio->GPCLR[regIdx] = (1 << (GPIO % 32));
}

int GetGPIO_Value(int GPIO){
	int regIdx = GPIO / 32;
	uint32_t val;
	val = pGpio->GPLEV[regIdx];
	val &= (1 << GPIO % 32);
	return val > 0;
}



void timer_callback(struct timer_list* timer){
	
	int delta;
	static bool on = false; // inicializacija samo ob prvem klicu fun.

	printk(KERN_INFO "LEDdriver: timer_callback");
	button_val = (char) GetGPIO_Value(BUTTON);

	if(button_val == 0){
		delta = 2000;
	}
	else{
		delta = 500;
	}
	mod_timer(&timer1, jiffies + msecs_to_jiffies(delta));
	
	if(flashing || button_val == 0){
		on = !on;
	}
	else{
		on = false;
	}
	SetGPIO_Out(LED, on);
}




int module_init_my(void){
    Major = register_chrdev(0, DEVICE_NAME, &fops);
	printk(KERN_INFO "LEDdriver: Module init - got major number %d", Major);
	if (Major < 0) {
	  printk(KERN_ALERT "LEDdriver: Registering char device failed with %d", Major);
	  return Major;
	}
	got_mem_region = 1;
	if(request_mem_region(GPIO_BASE, RANGE, DEVICE_NAME) == NULL){
		printk(KERN_ERR "LEDdriver: request memory region failed, so what");
		got_mem_region = 0;
	}
	// preslikava fizičnih naslovov
	pGpio = ioremap(GPIO_BASE, RANGE);

	// incializacija GPIO pinov 14 in 15
	SetGPIO_Function(BUTTON, 0); // BUTTON je input
	SetGPIO_Function(LED, 0); // najprej reset
	SetGPIO_Function(LED, 1); // LED je output

	// incializacija časovnika
	timer_setup(&timer1, timer_callback, 0);
	mod_timer(&timer1, jiffies + msecs_to_jiffies(2000) );
	return 0;
}

void module_cleanup_my(void){

	// reset GPIO pinov 14 in 15
	SetGPIO_Function(BUTTON, 0); // BUTTON je input
	SetGPIO_Function(LED, 0); // najprej reset

	unregister_chrdev(Major, DEVICE_NAME);

	if(got_mem_region){
		release_mem_region(GPIO_BASE, RANGE);
	}
	if ( del_timer_sync(&timer1) == 0)
		printk(KERN_ERR "LEDdriver: timer not deleted");
	
	printk(KERN_INFO "LEDdriver: Module cleanup - success!");
}


static int device_open(struct inode *inode, struct file *file){
	printk(KERN_INFO "LEDdriver: Device opened\n");
	return 0;
}

static int device_release(struct inode *inode, struct file *file){
	printk(KERN_INFO "LEDdriver: Device released\n");
	return 0;
}

static ssize_t device_read(struct file *filp, char __user *buffer,	size_t len, loff_t * offset){
    
	int size = 1;
    unsigned long retval;
    char flashing_ascii = flashing + 48;
    
    if(*offset >= size)
        return 0;

    if(len > size - *offset)
        len = size - *offset;
    
    retval = copy_to_user(buffer, &flashing_ascii, len);
    if(retval != 0)
        printk(KERN_ERR "Cannot copy to user!");

    *offset += len;
    return len;
}

static ssize_t device_write(struct file *filp, const char __user *buff, size_t len, loff_t * offset){
    
	if(buff && len>0){
		if (buff[0] == 1 ){
			flashing = 1;
		}
		else{
			flashing = 0;
		}
		return 1; // št bajtov zapisanih
	}	
	return 0;
}