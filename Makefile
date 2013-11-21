
CC     = arm-linux-gcc
CXX     = arm-linux-g++

CLIBS	= -lpthread -ljpeg

INC	= /usr/include/

CFLAGS = -I $(INC)
	

TARGET = aeap

OBJS = main.o keypad.o camera.o timer.o jpeg_capture.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET) $(CLIBS)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	rm *.o
