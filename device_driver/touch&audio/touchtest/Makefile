#
# Makfile for bmp example
#

CC= arm-linux-gcc

OBJS=touchtest.o

all: touchtest

fbtest: $(OBJS)
	$(CC) -o -static $@ $^

clean:
	rm -f $(OBJS) touchtest
