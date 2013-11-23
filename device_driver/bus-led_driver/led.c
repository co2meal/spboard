//fimename : led.c

#include <linux/init.h>
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/io.h>
#include <linux/kernel.h>
#include <linux/ioport.h>

#define DRIVER_AUTHOR        		"Hanback Electronics"
#define DRIVER_DESC          		"Led Test Program"
#define LEDIOPORT_MAJOR   			0
#define LEDIOPORT_NAME    			"LED IO PORT"
#define LEDIOPORT_MODULE_VERSION 	"LED IO PORT V0.1"
#define LEDIOPORT_ADDRESS 			0x14805000
#define LEDIOPORT_ADDRESS_RANGE 	0x1000

//Global variable
static int ledioport_usage = 0;
static int ledioport_major = 0;
static unsigned int ledioport_ioremap;

// define functions...
int ledioport_open(struct inode *minode, struct file *mfile) {
	
	if(ledioport_usage != 0) return -EBUSY;

	ledioport_ioremap= (unsigned int )ioremap(LEDIOPORT_ADDRESS,LEDIOPORT_ADDRESS_RANGE);

	if(!check_mem_region(ledioport_ioremap,LEDIOPORT_ADDRESS_RANGE))
		request_region(ledioport_ioremap,LEDIOPORT_ADDRESS_RANGE,LEDIOPORT_NAME);
	else 
		printk(KERN_WARNING"Can't get IO Region 0x%x\n", ledioport_ioremap);
	
	ledioport_usage = 1;
	return 0;
}

int ledioport_release(struct inode *minode, struct file *mfile) {
	
	release_region(ledioport_ioremap,LEDIOPORT_ADDRESS_RANGE);
	iounmap((unsigned char *)ledioport_ioremap);
	ledioport_usage = 0;
	return 0;
}

ssize_t ledioport_write_byte(struct file *inode, const char *gdata, size_t length, loff_t *off_what) {

	unsigned char *addr;
	unsigned char  c;

	get_user(c,gdata); 

	addr = (unsigned char *)(ledioport_ioremap);
	*addr = c;
	return length;
}

static struct file_operations led_fops = {
	.owner		= THIS_MODULE, 
	.write		= ledioport_write_byte,
	.open		= ledioport_open,
	.release	= ledioport_release,
};

int led_init(void) {
	
	int result;
	result = register_chrdev(LEDIOPORT_MAJOR,LEDIOPORT_NAME,&led_fops);
	
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	
	ledioport_major = result;

	printk("init module, led major number : %d\n",result);
	return 0;
}

void led_exit(void) {

	unregister_chrdev(ledioport_major,LEDIOPORT_NAME);

}


module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");











