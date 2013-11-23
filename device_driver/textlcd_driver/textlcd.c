//fimename : textlcd.c

#include <linux/init.h>
#include <linux/module.h>
#include <asm/hardware.h>
#include <asm/uaccess.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <asm-generic/ioctl.h>
#include <linux/ioport.h>
#include <asm/io.h>
#include <linux/delay.h>
#include "textlcd.h"

/*
 * Text LCD Pin Status *
 * 0 ~ 7 Bit 	: Data BUS
 * 8(E) Bit 	: Enable
 * 9(RW) Bit 	: Read/Write ( 1- Read , 0 - Write)
 * 10(RS) Bit 	: Register Selection ( 1 - Data Register , 0 - Command Register)
 * */

void setcommand(unsigned short command)
{
	command &= 0x00FF;

	outl((command | 0x0000), textlcd);
    outl((command | 0x0100), textlcd);
    outl((command | 0x0000), textlcd);
	udelay(50);

}

void writebyte(char ch)
{
	unsigned short data;
	data = ch & 0x00FF;

	outl((data | 0x400), textlcd);
	outl((data | 0x500), textlcd);
	outl((data | 0x400), textlcd);
	udelay(50);
}

void initialize_textlcd(void)
{
        function_set(2,0);	//Function Set:8bit,display 2l ines,5x7 mod
        display_control(1,0,0); 	// Display on, Cursor off
        clear_display(); 	// Display clear
        return_home();	// go home
        entry_mode_set(1,0); 	// Entry Mode Set : shift right cursor
        udelay(1000);
}

int function_set(int rows, int nfonts)
{
        unsigned short command = 0x30;

        if(rows == 2) command |= 0x08;
        else if(rows == 1) command &= 0xf7;
        else return -1;

        command = nfonts ? (command | 0x04) : command;
        setcommand(command);

        return 1;
}

int display_control(int display_enable, int cursor_enable, int nblink)
{
        unsigned short command = 0x08;
        command = display_enable ? (command | 0x04) : command;
        command = cursor_enable ? (command | 0x02) : command;
        command = nblink ? (command | 0x01) : command;
        setcommand(command);

        return 1;
}

int cusrsor_shit(int set_screen, int set_rightshit)
{
        unsigned short command = 0x10;
        command = set_screen ? (command | 0x08) : command;
        command = set_rightshit ? (command | 0x04) : command;
        setcommand(command);

        return 1;
}

int entry_mode_set(int increase, int nshift)
{
        unsigned short command = 0x04;
        command = increase ? (command | 0x2) : command;
        command = nshift ? ( command | 0x1) : command;
        setcommand(command);
        return 1;
}

int return_home(void)
{
        unsigned short command = 0x02;
        setcommand(command);

        udelay(2000);
        return 1;
}

int clear_display(void)
{
        unsigned short command = 0x01;
        setcommand(command);
        udelay(2000);
        return 1;
}

int set_ddram_address(int pos)
{
        unsigned short command = 0x80;
        command += pos;
        setcommand(command);
        return 1;
}

static int textlcd_open(struct inode *minode, struct file *mfile) 
{
	if(textlcd_usage != 0) return -EBUSY;

	textlcd_ioremap= (unsigned int)ioremap(TEXTLCD_ADDRESS,TEXTLCD_ADDRESS_RANGE);
		if(!check_mem_region(textlcd_ioremap,TEXTLCD_ADDRESS_RANGE))
			request_region(textlcd_ioremap,TEXTLCD_ADDRESS_RANGE,TEXTLCD_NAME);
		else printk(KERN_WARNING"Can't get IO Region 0x%x\n",TEXTLCD_ADDRESS);
		textlcd_usage = 1;

		textlcd = (unsigned short *)textlcd_ioremap;
        initialize_textlcd();
        return 0;
}

static int textlcd_release(struct inode *minode, struct file *mfile) 
{
	release_region(textlcd_ioremap,TEXTLCD_ADDRESS_RANGE);
	iounmap((unsigned short *)textlcd_ioremap);
	textlcd_usage = 0;
	return 0;
}

static ssize_t textlcd_write(struct file *inode, const char*gdata, size_t length, loff_t *off_what) 
{
	int i,ret;
	char buf[100];
	ret = copy_from_user(buf,gdata,length);
	if(ret < 0) return -1;	

	for(i=0;i<length;i++)
		writebyte(buf[i]);
	return length;
}

static int textlcd_ioctl(struct inode *inode, struct file *file,unsigned int cmd,unsigned long gdata) 
{
        struct strcommand_varible strcommand;
		int ret;

        ret = copy_from_user(&strcommand,(char *)gdata,32);
		if(ret < 0) return -1;
	
        switch(cmd) 
		{
        	case TEXTLCD_COMMAND_SET:
                setcommand(strcommand.command);
               	break;
        	case TEXTLCD_FUNCTION_SET:
                function_set((int)(strcommand.rows+1),(int)(strcommand.nfonts));
               	break;
	        case TEXTLCD_DISPLAY_CONTROL:
    	           display_control((int)strcommand.display_enable,
				(int)strcommand.cursor_enable,(int)strcommand.nblink);
               	break;
			case TEXTLCD_CURSOR_SHIFT:
                cusrsor_shit((int)strcommand.set_screen,(int)strcommand.set_rightshit);
                break;
	        case TEXTLCD_ENTRY_MODE_SET:
				entry_mode_set((int)strcommand.increase,(int)strcommand.nshift);
                break;
	        case TEXTLCD_RETURN_HOME:
                return_home();
                break;
    	    case TEXTLCD_CLEAR:
                clear_display();
                break;
        	case TEXTLCD_DD_ADDRESS:
                set_ddram_address((int)strcommand.pos);
                break;
	        case TEXTLCD_WRITE_BYTE:
                writebyte(strcommand.buf[0]);
                break;
    	    default:
                printk("driver : no such command!\n");
                return -ENOTTY;
        }
        return 0;
}

static struct file_operations textlcd_fops = {
		.owner		= THIS_MODULE,
        .write		= textlcd_write,
        .ioctl		= textlcd_ioctl,
        .open		= textlcd_open,
        .release	= textlcd_release,
};

int textlcd_init(void)
{
        int result;
        result = register_chrdev(TEXTLCD_MAJOR,TEXTLCD_NAME,&textlcd_fops);

        if(result < 0) {
                printk(KERN_WARNING"Can't get any major\n");
                return result;
        }
        textlcd_major = result;
        printk("init module, textlcd major number : %d\n",result);
        return 0;
}

void textlcd_exit(void) 
{

	unregister_chrdev(textlcd_major,TEXTLCD_NAME);

}

module_init(textlcd_init);
module_exit(textlcd_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("Dual BSD/GPL");
