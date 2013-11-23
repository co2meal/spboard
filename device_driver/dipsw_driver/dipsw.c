//filename : dipsw.c

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm/fcntl.h>
#include <linux/ioport.h>


#include <asm/ioctl.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <asm/io.h>

#define DRIVER_AUTHOR       "Hanback"
#define DRIVER_DESC			"DipSW Test Program"
#define DIP_MODULE_VERSION	"DIPSW V0.1"
#define	DIP_NAME			"DIPSW"
#define	DIP_MAJOR_NUMBER	0

#define ADDR_OF_DIP			0x14803000
#define DIP_ADDRESS_RANGE	0x2000
#define TIMER_INTERVAL		20


//Global variable
static unsigned int dipsw_usage = 0;
static unsigned int dipsw_major = 0;
static unsigned int dipsw_ioremap;
static unsigned char *dipswS6, *dipswS19;
static unsigned char value[4];
static struct timer_list mytimer;
static pid_t id;

int dipsw_open (struct inode *inode, struct file *filp)
{
	if(dipsw_usage != 0) return -EBUSY;

	dipsw_ioremap = (unsigned int)ioremap(ADDR_OF_DIP, DIP_ADDRESS_RANGE);
		
	if(!check_mem_region(dipsw_ioremap,DIP_ADDRESS_RANGE)) 
		request_region(dipsw_ioremap, DIP_ADDRESS_RANGE, DIP_NAME);
	else
		printk(KERN_WARNING"Can't get IO Region 0x%x\n", dipsw_ioremap);

	dipswS6 = (unsigned char *)(dipsw_ioremap);
	dipswS19 = (unsigned char *)(dipsw_ioremap + 0x1000);
	
	dipsw_usage = 1;	
	return 0;
}


int dipsw_release (struct inode *inode, struct file *filp)
{
	del_timer(&mytimer);	
	release_region(dipsw_ioremap, DIP_ADDRESS_RANGE);
	iounmap((unsigned char *)dipsw_ioremap);

	dipsw_usage = 0;
	return 0;
}

void mypollingfunction(unsigned long data)
{
	value[0] = (*(dipswS6) & 0x0f);
	value[1] = (*(dipswS6) >> 4);
	value[2] = (*(dipswS19) & 0x0f);
	value[3] = (*(dipswS19) >> 4);
	 
	kill_proc(id,SIGUSR1,1);
	mytimer.expires = get_jiffies_64() + TIMER_INTERVAL;
	add_timer(&mytimer);
}

ssize_t dipsw_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
 	
        get_user(id,(unsigned int *)gdata);

        init_timer(&mytimer);
        mytimer.expires = get_jiffies_64() + TIMER_INTERVAL;
        mytimer.function = &mypollingfunction;
        add_timer(&mytimer);
        return length;
			
}

ssize_t dipsw_read(struct file *inode, char *gdata, size_t length, loff_t *off_what)
{
		int ret;
        ret = copy_to_user(gdata,&value,4);
		if(ret < 0) return -1;
        return length;
}

			
struct file_operations dipsw_fops = 
{
	.owner		= THIS_MODULE,
	.open		= dipsw_open,
	.write		= dipsw_write,
	.read		= dipsw_read,
	.release	= dipsw_release,

};

int dipsw_init(void)
{
	int result;

	result = register_chrdev(DIP_MAJOR_NUMBER, DIP_NAME, &dipsw_fops);
	if (result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}

	dipsw_major = result;
	printk("init module, dipsw major number : %d\n", result);
	return 0;
}
		
	
void dipsw_exit(void)
{
	unregister_chrdev(DIP_MAJOR_NUMBER,DIP_NAME);
}


module_init(dipsw_init);
module_exit(dipsw_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");



