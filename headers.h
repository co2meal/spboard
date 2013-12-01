
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string>
#include <jpeglib.h>
#include<sys/mman.h>
#include<sys/ioctl.h>
#include <linux/input.h>
#include <iostream>

using namespace std;


extern int active_mode, waiting_mode;
