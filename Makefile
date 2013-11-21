
CC     = arm-linux-gcc
CXX     = arm-linux-g++

CLIBS	= -lpthread
	

TARGET = aeap

OBJ = main.o keypad.o camera.o timer.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET) $(CLIBS)

.c.o:
	$(CXX) $(CFLAGS) $(INCLUDES) -c $<  -o $@
