
CC     = arm-linux-gcc
CXX     = arm-linux-g++

CLIBS	= -lpthread -lz -ljpeg -ljson_linux-gcc-4.1.1_libmt
#CLIBS	= -lpthread -ljpeg

INC	= includes

CFLAGS = -Wall -I $(INC)

LDFLAGS = -L libs

TARGET = aeap

OBJS = main.o keypad.o camera.o timer.o jpeg_capture.o buzzer.o mutex.o led.o charlcd.o popen.o
#OBJS = main.o keypad.o camera.o timer.o jpeg_capture.o buzzer.o mutex.o led.o charlcd.o 

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET) $(CLIBS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

.c.o:
	$(CXX) $(CFLAGS) -c $<  -o $@

clean:
	rm *.o
	rm $(TARGET)
