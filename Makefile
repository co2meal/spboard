
CC     = arm-linux-gcc
CXX     = arm-linux-g++

CLIBS	= -lpthread -ljpeg

INC	= /usr/include/

CFLAGS = -Wall -I $(INC)
	

TARGET = aeap

OBJS = main.o keypad.o camera.o timer.o jpeg_capture.o buzzer.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET) $(CLIBS)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	rm *.o
