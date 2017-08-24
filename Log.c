#include "head.h"


void Start_Log(void)
{
	fp = fopen("./svclog.log","a");
	if(NULL == fp)
	{
		bug("log open fiald");
		exit(0);
	}
	time(&tim);
	timep = localtime(&tim);
	fprintf(fp,"data: %d-%02d-%02d %02d:%02d:%02d\n************************SERVER START********************\n\n",timep->tm_year + 1900,timep->tm_mon +1,timep->tm_mday,timep->tm_hour,timep->tm_min,timep->tm_sec);
	fflush(fp);
}
