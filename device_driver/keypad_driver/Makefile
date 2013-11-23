
# keypad Device Driver Makefile

CC     = arm-linux-gcc

obj-m := keypad.o

KDIR := /working/linux-2.6.25-sm2_p320

PWD := $(shell pwd)

MODULE_OBJS = keypad.o
MODULE_SRCS = keypad.c

TEST_TARGET = test
TEST_SRCS = test.c

all : $(MODULE_OBJS) $(TEST_TARGET)

$(MODULE_OBJS) : $(MODULE_SRCS)
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

$(TEST_TARGET) : $(TEST_SRCS)
	$(CC) $(TEST_SRCS) -o $@

clean:
	rm -f *.ko *.o *.mod.* .*.cmd Module.symvers modules.order
	rm -f $(TEST_TARGET)
