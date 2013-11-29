#
# Makfile
#

CC= arm-linux-gcc

OBJS= main.o audio_lib.o keyboard.o


all: Audio_exam

Audio_exam: $(OBJS)
	$(CC)  -o $@ $^

clean:
	rm -f $(OBJS) Audio_exam
