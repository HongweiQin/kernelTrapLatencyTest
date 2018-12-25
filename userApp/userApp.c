#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include<time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	struct timespec tstart={0,0}, tend={0,0};
	char mybuf[20][4096];
	char directBuf[10][4096];
	int fd = open("/proc/ktlt/tf", O_RDWR);
	long i = atol(argv[1]);
	int isDirect = atoi(argv[2]);
	long total;
	volatile long dummy;
	if (i < 0)
		i = 0;
	total = i;
	memset(mybuf, 0, 4096*20);
	memset(directBuf, 0, 4096*10);

    clock_gettime(CLOCK_MONOTONIC, &tstart);
	while (i--) {
		dummy = i;
		if (isDirect)
			//;
			memcpy(directBuf[i%10], mybuf[i%20], 4096);
		else
			write(fd, mybuf[i%20], 4096);

		dummy = i;
		__asm__ __volatile__("": : :"memory");
	}
    clock_gettime(CLOCK_MONOTONIC, &tend);

    printf("%ld\t%lld\n",
		total,
		(tend.tv_sec - tstart.tv_sec)*1000000000 + tend.tv_nsec - tstart.tv_nsec);
	close(fd);
	return 0;
}
