
CC     = arm-linux-gcc
CXX     = arm-linux-g++

CLIBS	= -lpthread -ljpeg -lcurl -ljson_linux-gcc-4.1.1_libmt 

INC	= /usr/local/arm-linux-4.1.1/include/

CFLAGS = -Wall -I $(INC)

LDFLAGS = -L /usr/local/arm-linux-4.1.1/lib/
	

TARGET = aeap

OBJS = main.o keypad.o camera.o timer.o jpeg_capture.o buzzer.o mutex.o led.o charlcd.o curltest.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CFLAGS) $(OBJS) -o $(TARGET) $(CLIBS) $(LDFLAGS)

.cpp.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	rm *.o
