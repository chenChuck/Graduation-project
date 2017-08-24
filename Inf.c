#include "head.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <limits.h>
#include <asm/ioctls.h>
#include <time.h>
#include <pthread.h>


void *Inf(void *arg)
{
#if 0
	int fd;
	char buf[1] = {0};
	int i = 0;

	fd = open("/dev/imx28x_led", O_RDWR);
	if (fd < 0) {
		perror("open /dev/imx283_led");
	}

	buf[0] = 0;
#endif
	while(1)
	{
	//	i = write(fd, buf, 1);
		sleep(2);
	//	if(i == 1)
	//	{
			pthread_cond_signal(&camid);			
	//	}
	}
}
