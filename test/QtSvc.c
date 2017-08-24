#include "head.h"

void getdatelist(int fd);
void getpiclist(int fd,char *path);
void getpicture(int fd,char *filename);
void lsfun(int fd,int mode);
void delpic(char *s);


void SvcOver(int sign_no)
{
	time(&tim);
	timep = localtime(&tim);
	fprintf(fp,"data: %d-%02d-%02d %02d:%02d:%02d\n************************SERVER STOP ********************\n\n\n\n",timep->tm_year + 1900,timep->tm_mon +1,timep->tm_mday,timep->tm_hour,timep->tm_min,timep->tm_sec);
	fflush(fp);
	fclose(fp);
	exit(0);
}

void ClientOut(int sign_no)
{
	if(sign_no == SIGPIPE)
	{	
		//////****
		logmsg("CLIENT IPADDR: %s,  PORT : %d  EXIT",(char *)inet_ntoa(client.sin_addr),client.sin_port);
		printf("pipe\n");
		flg = 0;
	}
}

void *QtSvc(void *arg)
{

	sfd = Tcp_Server_init(8888);
	socklen_t len = sizeof(client);

	int ret = 0;
	int readret = 0;

	signal(SIGPIPE,ClientOut);
	signal(SIGINT,SvcOver);

START:
	while(1)
	{
		int new_fd = accept(sfd,(struct sockaddr *)(&client),&len);
		logmsg("CLIENT IPADDR: %s,  PORT : %d  JOIN",(char *)inet_ntoa(client.sin_addr),client.sin_port);

		if(-1 == new_fd){
			logmsg("accept()");
			bug("accept()");
			continue;
		}
		flg = 1;
		while(flg){
			int sum = 0;
			int readret = sizeof(qtdata);
			pthread_mutex_lock(&qdata);
			while(readret)
			{	
				ret = read(new_fd,&qtdata+sum,sizeof(qtdata)-sum);
				if(ret == 0)
				{
					logmsg("CLIENT IPADDR: %s,  PORT : %d  EXIT",(char *)inet_ntoa(client.sin_addr),client.sin_port);
					printf("pipe\n");
					flg = 0;
					close(new_fd);
					pthread_mutex_unlock(&qdata);
					goto START;
				}
				sum += ret;
				readret -= ret;
			}
			switch(qtdata.mode)
			{
				case GETDATELIST:
					getdatelist(new_fd); break;
				case GETPICLIST:
					getpiclist(new_fd,qtdata.nbuff[0]);
					break;
				case GETPICTURE:
					getpicture(new_fd,qtdata.nbuff[0]);
					pthread_mutex_unlock(&qdata);
					break;
				case ON:
					logmsg("camera open");
					pthread_mutex_unlock(&qdata);
					break;
				case OFF:
					logmsg("camera close");
					pthread_mutex_unlock(&qdata);
					break;
				case DELETE:
					delpic(qtdata.nbuff[0]);
					logmsg("delete picture/%s picture",qtdata.nbuff[0]);
					break;
			}
		}

	}
}

void delpic(char *s)
{
//	chdir("/home/cody/work-cody/picture/");
//	chdir(s);
	char cbuff[64] = {0};
	sprintf(cbuff,"rm -rf %s",s);
	system(cbuff);
	pthread_mutex_unlock(&qdata);
}

void getdatelist(int fd)
{
	logmsg("get date list");
	chdir("/home/cody/work-cody/picture");
	lsfun(fd,0);
}
void getpiclist(int fd, char *path)
{
	logmsg("get picture list");
	chdir("/home/cody/work-cody/picture");
	chdir(path);
	lsfun(fd,1);
}
void getpicture(int fd,char *filename)
{
	logmsg("get picture");
	FILE *fp = NULL;
	printf("%s\n",filename);
	fp = fopen(filename,"r");
	if(fp == NULL)
	{
		printf("error\n");
		return;
	}

	char buff[1024*100];

	memset(buff,0,sizeof(buff));

	int sum = 0;
	int ret = 0;

	while(ret = fread(buff+sum,1,1024,fp))
	{
		sum += ret;
	}
	camdata.mode = GETPICTURE;
	camdata.num = sum;
	strcpy(camdata.buff,buff);

	ret = 	write(fd,&camdata,8);
	ret = write(fd,buff,sum);
	printf("send %d\n",ret);
	fclose(fp);
	return;
}
#if 0
	int fd = Tcp_Server_init(8889);
	int new_fd = accept(fd,NULL,NULL);
#endif




void lsfun(int fd,int mode)
{
	fstream=NULL;  
	memset(&qtdata,0,sizeof(qtdata));
	if(NULL==(fstream=popen("ls","r")))  
	{ 
		bug("execute command failed: %s",strerror(errno));  
		return;  
	} 
	int num = 1;
	qtdata.mode = mode;
	while(num)
	{
		if(NULL!=fgets(qtdata.nbuff[num], sizeof(qtdata.nbuff[0]), fstream)) 
		{
			qtdata.nbuff[num][strlen(qtdata.nbuff[num])-1] = '\0';
			printf("%s\n",qtdata.nbuff[num]);
			num++;
		} 
		else
		{
			qtdata.num = num - 1;
			break;
		}
	}
	int ret = write(fd,&qtdata,sizeof(qtdata));
	printf("send %d\n",ret);
	pthread_mutex_unlock(&qdata);
	memset(&qtdata,0,sizeof(qtdata));
}
