#include "head.h"


int main(int argc, const char *argv[])
{
	Start_Log();
	

	pthread_create(&qtsvctid,NULL,QtSvc,NULL);
	pthread_create(&camtid,NULL,Cam,NULL);
	pthread_create(&infraredtid,NULL,Inf,NULL);


	pthread_join(camtid,NULL);
	pthread_join(infraredtid,NULL);
	pthread_join(qtsvctid,NULL);


	return 0;
}

