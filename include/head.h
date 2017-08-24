#ifndef __HEAD__
#define __HEAD__




/********************************头文件*********************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
/******************************宏定义***********************************/

#define GETDATELIST 	0
#define GETPICLIST 		1
#define GETPICTURE 		2
#define ON 				3	
#define OFF 			4
#define DELETE 			5


			  						// 		日志宏定义	
#define logmsg(format,...) time(&tim);\
	timep = localtime(&tim) ;\
	fprintf(fp,"data: %d-%02d-%02d %02d:%02d:%02d\t",timep->tm_year + 1900,timep->tm_mon +1,timep->tm_mday,timep->tm_hour,timep->tm_min,timep->tm_sec);\
	fprintf(fp,format"\n",##__VA_ARGS__);\
fflush(fp)  						
									// 		bug报错宏定义
//#define bug(format,...) printf("FILE:"__FILE__",Line:%05d"format"\n",__LINE__,##VA_ARGS__)
#define bug(format,...) printf("FILE:"__FILE__",Line:%05d:"format"\n",__LINE__,##__VA_ARGS__)



/******************************函数声明*******************************/
int Tcp_Server_init(int port);   	// 		tcp服务注册	
void Start_Log(void); 				// 		日志服务启动
void Organize_Data(char *ord);
void *QtSvc(void *arg);
void *Cam(void *arg);
void *Inf(void *arg);
void SvcOver(int sign_no);
void ClientOut(int sign_no);

/******************************消息结构体******************************/
struct STQM{
	int mode;
	int num;
	char nbuff[100][16];
};
struct STQCM{
	int mode;
	int num;
	char buff[1024 * 100];
};




/****************************引用静态变量********************************/
extern struct tm *timep; 			// 		日志时间指针 
extern FILE *fp; 					// 		日志文件流指针
extern FILE *fstream; 				// 		命令文件流指针
extern time_t tim; 					// 		时间变量
extern int sfd;
extern struct STQM qtdata;
extern struct STQCM camdata;
extern struct sockaddr_in client;
extern pthread_mutex_t qdata;
extern pthread_mutex_t cam;
extern pthread_t qtsvctid;
extern pthread_t camtid;
extern pthread_t infraredtid;
extern pthread_cond_t camid;
extern int flg;

#endif
