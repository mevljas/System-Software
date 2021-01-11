#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define DEVICE_NAME "ogrodje"
#define BUFSIZE 10

MODULE_LICENSE("GPL");

/*
make -C /usr/src/WSL2-Linux-Kernel-$(uname -r) M=$(pwd) modules
sudo insmod ./ogrodje.ko
lsmod
cat /proc/devices | grep ogrodje
sudo mknod -m 666 /dev/ogrodje c $(cat /proc/devices | grep ogrodje | cut -d" " -f1) 0
sudo dmesg -c
sudo rmmod ogrodje
*/

int zacni_modul(void);
void koncaj_modul(void);
int odpri(struct inode *, struct file *);
int sprosti(struct inode *, struct file *);
ssize_t beri(struct file *, char *, size_t, loff_t *);
ssize_t pisi(struct file *, const char *, size_t, loff_t *);

int Major;
char *buf;
int buf_used;
int prvo_pisanje_ta_open;

int stevilo_odpiranj;

struct file_operations fops = {
	.open = odpri,
	.release = sprosti,
	.read = beri,
	.write = pisi
};

module_init(zacni_modul);
module_exit(koncaj_modul);

int zacni_modul(void)
{
	Major = register_chrdev(0, DEVICE_NAME, &fops);
	if (Major < 0) {
		printk(KERN_ALERT "Registracija znakovne naprave spodletela.\n");
		return Major;
	}
	stevilo_odpiranj = 0;

	buf = kmalloc(BUFSIZE, GFP_KERNEL);
	if(buf == NULL)
	{
		printk(KERN_ERR "Zaseganje pomnilnika ni uspelo.\n");
		koncaj_modul();
	}

	prvo_pisanje_ta_open = 1;

	printk(KERN_INFO "Glavno stevilo je %d.\n", Major);
	return 0;
}

void koncaj_modul(void)
{
	if(buf)
		kfree(buf);
	unregister_chrdev(Major, DEVICE_NAME);
}

int odpri(struct inode *inode, struct file *file)
{ 
	stevilo_odpiranj++;
	//sprintf(msg, "Zivjo!\nOdprt sem bil %d-krat.\n", stevilo_odpiranj);
	printk(KERN_INFO "Odprt sem bil %d-krat", stevilo_odpiranj);
	// Ce je odprto za pisanje
	if( (file->f_flags & O_ACCMODE) == O_WRONLY)
	{
		prvo_pisanje_ta_open = 1;
	}

	return 0; 
}

int sprosti(struct inode *inode, struct file *file)
{ 
	return 0; 
}

ssize_t beri(struct file *filp, char __user *buff, size_t len, loff_t *offset)
{
	if ( *offset >= buf_used)
		return 0;

	if ( len > buf_used - *offset)
		len = buf_used - *offset;

	if ( copy_to_user( buff, buf + *offset, len) )
		return -EFAULT;

	*offset += len;

	printk(KERN_INFO "len: %ld, buf_used: %d, offset: %lld", len, buf_used, *offset);

	return len;
}

ssize_t pisi(struct file *filp, const char __user *user_buff, size_t len, loff_t *offset)
{
	if(prvo_pisanje_ta_open)
	{
		//memset(buf, 0, BUFSIZE); ni treba
		buf_used = 0;
		prvo_pisanje_ta_open = 0;
	}	

	if ( *offset >= BUFSIZE)
		return 0;
	
	//v buf lahko damo samo se toliko, kolikor je preostalo placa
	if (len > BUFSIZE - *offset)
		len = BUFSIZE - *offset;
	
	if ( copy_from_user( buf + *offset, user_buff, len) )
		return -EFAULT;

	*offset += len;
	buf_used += len;

	printk(KERN_INFO "len: %ld, buf_used: %d, offset: %lld", len, buf_used, *offset);

	return len;
}
