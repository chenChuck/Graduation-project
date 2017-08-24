#include "head.h"

struct tm *timep;
FILE *fp;
FILE *fstream;
time_t tim;
int sfd;
int flg = 0;
struct STQM qtdata;
struct STQCM camdata;
struct sockaddr_in client;

pthread_mutex_t qdata;


pthread_t qtsvctid;
pthread_t camtid;
pthread_t infraredtid;
pthread_cond_t camid;
pthread_mutex_t cam;


