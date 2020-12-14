#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h> // lahko je tudi #include <asm/uaccess.h>

int zacni(void);
void koncaj(void);
ssize_t beri(struct file *filp, char *buff, size_t len, loff_t *offset);
ssize_t pisi(struct file *filp, const char *buff, size_t len, loff_t *offset);

struct file_operations fops = { // tagged structure
    .open = NULL,               // ni implementirano
    .release = NULL,
    .read = beri,
    .write = pisi
};

int Major;

module_init(zacni); // pove, katera funkcija je inicializacijska
module_exit(koncaj);

int zacni(void) //init funckija
{
    Major = register_chrdev(0, "ogrodje", &fops);
    if (Major < 0)
    {
        printk(KERN_ALERT "Registracija naprave ni uspela.\n");
        return Major;
    }
    printk(KERN_INFO "Modul je vstavljen v jedro.\n");^

    printk(KERN_INFO "Glavno stevilo je %d.\n", Major);
    return 0;
}

void koncaj(void) // cistilna funckija
{
    unregister_chrdev(Major, "ogrodje");
    printk(KERN_INFO "Modul odstranjen iz jedra.\n");^
}

ssize_t beri(struct file *filp, char *buff, size_t len, loff_t *offset)
{
    // Len ej dolzina bufferja, ki ga amo v usersapcu.Po defuatlu je enkaa reltivnoveliak vrednost.
    // velikost bufferja len je velika vrednost, npr. 128K
    char *msg = "Hello!\n";
    int size = strlen(msg);

    // offset pvoe, kej tnreutno se nahaja. Bralnca funckiaj bere.
    if (*offset >= size)
        return 0;
    if (len > size - *offset)
        len = size - *offset;
    if (copy_to_user(buff, msg, len))
        return -EFAULT;
    *offset += len;
    return len;
}
// Bralno funkcijo aplikacije tipično kličejo tolikokrat, da prebere vse!
// (Možno je tudi, da je size > len)

// bere iz modlua k upraobniku. Npr če reeč cat /dev/ogordje. Upraobnik bo bral.
// branej mpomeni read iz gonilnika.

ssize_t pisi(struct file *filp, const char *buff, size_t len, loff_t *offset)
{
    return 0;
} // ne zna pisat