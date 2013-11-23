#include <linux/init.h>
#include <linux/module.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/ioctl.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/signal.h>

#define DRIVER_AUTHOR 			"Hanback Electronics" 
#define DRIVER_DESC	 			"keypad test program" 
#define KEYPAD_MAJOR			0
#define KEYPAD_NAME 			"KEYPAD"
#define KEYPAD_MODULE_VERSIO	"KEYPAD V0.1"
#define KEYPAD_PHY_ADDR			0x14806000
#define KEYPAD_ADDR_RANGE 		0x3000
#define TIMER_INTERVAL			(2*HZ/10)	

//Global variable
static int keypad_usage = 0;
static int keypad_major = 0;
static struct timer_list mytimer;
static unsigned char value;
static pid_t id;
static unsigned int  keypad_ioremap;
static unsigned char *keypad_row_addr;
static unsigned char *keypad_col_addr;
static unsigned char *buzzer_addr;

// define functions...
int keypad_open(struct inode *minode, struct file *mfile)
{
	if(keypad_usage != 0) return -EBUSY;
		
	keypad_ioremap = (unsigned int)ioremap(KEYPAD_PHY_ADDR,KEYPAD_ADDR_RANGE);

	if(!check_mem_region(keypad_ioremap, KEYPAD_ADDR_RANGE))
		request_region(keypad_ioremap, KEYPAD_ADDR_RANGE, KEYPAD_NAME);
	else
		printk("driver: unable to register this!\n");

	keypad_row_addr = (unsigned char *)keypad_ioremap;
	keypad_col_addr = (unsigned char *)(keypad_ioremap+0x1000);
	buzzer_addr = (unsigned char *)(keypad_ioremap+0x2000);

	keypad_usage = 1;
	return 0;
}
	
int keypad_release(struct inode *minode, struct file *mfile)
{
	release_region(keypad_ioremap, KEYPAD_ADDR_RANGE);
	iounmap((unsigned char *)keypad_ioremap);
	del_timer(&mytimer);
	keypad_usage = 0;
	return 0;
}

void mypollingfunction(unsigned long data)
{
	int j=1,k,i;
    unsigned char tmp[4] = {0x01, 0x02, 0x04, 0x08};

	value =0;
    for(i=0;i<4;i++) {
        *keypad_row_addr = tmp[i];
        value = *keypad_col_addr & 0x0f;
        if(value > 0) {
            for(k=0;k<4;k++) {
                if(value == tmp[k])  {
                    value = j+(i*4);
					if(value != 0x00) goto stop_poll;
                }
                j++;
            }
        }
    }

	stop_poll:
		if(value>0) {
			*buzzer_addr = 0x1;
			mdelay(50);
			*buzzer_addr = 0x0;
			kill_proc(id,SIGUSR1,1);
		}
		else *keypad_row_addr = 0x00;

		mytimer.expires = get_jiffies_64() + TIMER_INTERVAL;
		add_timer(&mytimer);
}

ssize_t keypad_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what)
{
	get_user(id,(int *)gdata);
	
	init_timer(&mytimer);
	mytimer.expires = get_jiffies_64() + TIMER_INTERVAL;
	mytimer.function = &mypollingfunction;
	add_timer(&mytimer);
	return length;
}

ssize_t keypad_read(struct file *inode, char *gdata, size_t length, loff_t *off_what)
{
	int ret;
	ret = copy_to_user(gdata, &value, 1);
	if(ret < 0) return -1;
	return length;
}

struct file_operations keypad_fops = {
	.owner		= THIS_MODULE,
	.read		= keypad_read,
	.write		= keypad_write,
	.open		= keypad_open,
	.release	= keypad_release,
};


int keypad_init(void) 
{
	int result;
	result = register_chrdev(KEYPAD_MAJOR, KEYPAD_NAME, &keypad_fops);
	
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	keypad_major = result;
		
	printk("init module, keypad major number : %d\n", result);
	
	
	return 0;
}

void keypad_exit(void) 
{

	unregister_chrdev(keypad_major, KEYPAD_NAME);
	
}

module_init(keypad_init);
module_exit(keypad_exit);

MODULE_AUTHOR(DRIVER_AUTHOR); 
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_LICENSE("Dual BSD/GPL");
