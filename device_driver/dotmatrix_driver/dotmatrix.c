#include <linux/init.h>
#include <linux/module.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <asm/ioctl.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <asm/io.h>

#define DRIVER_AUTHOR			"Hanback Electronics"
#define DRIVER_DESC				"Dotmatrix Test Program"
#define DOT_MAJOR				0
#define DOT_NAME 				"DOT"
#define DOT_MODULE_VERSION 		"DOT V0.1"
#define DOT_PHY_ADDR			0x1480A000
#define DOT_ADDR_RANGE 			0x3000
#define NUMSIZE					4
#define DELAY					3

//Global variable
static int dot_usage = 0;
static int dot_major = 0;

static unsigned int dot_ioremap;
static unsigned short *dot_row_addr;
static unsigned short *dot_col_addr;

unsigned short font_num[40]= {
	0x41, 0x22, 0x14, 0x08,  // 0
	0x08, 0x14, 0x22, 0x41,  // 1
	0x7f, 0x41, 0x41, 0x7f,  // 2//0
	0x41, 0x22, 0x14, 0x7f,  // 3//k
	0x10, 0x7f, 0x10, 0x1f,  // 4
	0x79, 0x49, 0x49, 0x4f,  // 5
	0x79, 0x49, 0x49, 0x7f,  // 6
	0x7f, 0x01, 0x01, 0x01,  // 7
	0x7f, 0x49, 0x49, 0x7f,  // 8
	0x7f, 0x49, 0x49, 0x4f,  // 9
};

// define functions...
int dot_open(struct inode *minode, struct file *mfile) {
	if(dot_usage != 0) return -EBUSY;
	
	dot_ioremap = (unsigned int)ioremap(DOT_PHY_ADDR,DOT_ADDR_RANGE);
	if(!check_mem_region(dot_ioremap, DOT_ADDR_RANGE))
		request_region(dot_ioremap, DOT_ADDR_RANGE, DOT_NAME);
	else	printk("driver: unable to register this!\n");
	
	dot_col_addr = (unsigned short *)dot_ioremap;
	dot_row_addr = (unsigned short *)(dot_ioremap+0x2000);

	dot_usage = 1;
	return 0;
}
	
int dot_release(struct inode *minode, struct file *mfile) {
	release_region(dot_ioremap, DOT_ADDR_RANGE);
	iounmap((unsigned short *)dot_ioremap);
	dot_usage = 0;
	return 0;
}

ssize_t dot_write(struct file *inode, const char *gdata, size_t length, loff_t *off_what) {
	int i,j,k,up_digit=0,quit=1,counter;
	int size=0,cnt=0,ret;
	unsigned int second_data, dispdata[8];
	unsigned int init_col1=0x100, init_col2=0x008;

	ret = copy_from_user(&counter, gdata, 4);
	if(ret < 0) return -1;

	while(quit) {
		if(cnt == 10) {
			cnt = 0;
			up_digit++;
		}

		if(up_digit == 10) up_digit=0;

		if(up_digit == 0) {
			size = 4;
			for(k=0;k<4;k++) {
				dispdata[k] = font_num[cnt*NUMSIZE+k];
			}
		} else {
			size = 8;
			second_data = up_digit%10;
			for(k=0;k<4;k++) {
				dispdata[k] = font_num[cnt*NUMSIZE+k];
				dispdata[k+4] = font_num[second_data*NUMSIZE+k];
			}
		}


		for(i=0;i<150;i++) {
			for(j=0;j<size;j++) {
				if(up_digit*10 + cnt == 10&&counter==10){
				if(j<4)	{
					*dot_col_addr = init_col1 >> j;
					*dot_row_addr = dispdata[j];
				} else {
					*dot_col_addr = init_col2 >> (j-4);
					*dot_row_addr = dispdata[j];
				}
				mdelay(DELAY);
				}
			}
			
		}

		
		for(i=0;i<150;i++) {
			for(j=0;j<size;j++) {
				if((up_digit*10 + cnt == 23) && counter == 23){
				if(j<4)	{
					*dot_col_addr = init_col1 >> j;
					*dot_row_addr = dispdata[j];
				} else {
					*dot_col_addr = init_col2 >> (j-4);
					*dot_row_addr = dispdata[j];
				}
				mdelay(DELAY);
				}
			}
			
		}


	/*

	for(i=0; i<30; i++){
for(j=0;j<size;j++) {
				if(j<4)	{
		*dot_col_addr = init_col1;
		*dot_row_addr = dispdata[0]; 
}
}
}*/


		if(counter == (up_digit*10 + cnt)) {
			quit = 0;
			break;
		}
		cnt++;
	}

	return length;
}

struct file_operations dot_fops = {
	.owner		= THIS_MODULE,
	.write		= dot_write,
	.open		= dot_open,
	.release	= dot_release,
};


int dot_init(void) {
	int result;
	result = register_chrdev(DOT_MAJOR, DOT_NAME, &dot_fops);
	
	if(result < 0) {
		printk(KERN_WARNING"Can't get any major\n");
		return result;
	}
	dot_major = result;
	printk("init module, dotmatrix major number : %d\n", result);
	
	return 0;
}

void dot_exit(void) {

	unregister_chrdev(dot_major, DOT_NAME);

}

module_init(dot_init);
module_exit(dot_exit);

MODULE_AUTHOR(DRIVER_AUTHOR); 
MODULE_DESCRIPTION(DRIVER_DESC); 
MODULE_LICENSE("Dual BSD/GPL");
