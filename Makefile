
CC     = arm-linux-gcc
CXX     = arm-linux-g++

CLIBS	= -lpthread -lz -ljpeg -ljson_linux-gcc-4.1.1_libmt

INC	= includes

CFLAGS = -Wall -I $(INC)

LDFLAGS = -L libs

TARGET = aeap

OBJS = main.o keypad.o camera.o segment.o jpeg_capture.o buzzer.o mutex.o led.o charlcd.o attendance.o dotmatrix.o touch.o EasyBMP.o

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
