#ifndef HEAD_H
#define HEAD_H
#include <qdebug.h>
#include <QCloseEvent>

#define GETDATELIST     0
#define GETPICLIST      1
#define GETPICTURE      2
#define DELETE          5
#define ON              3
#define OFF             4


typedef struct orddata
{
    int mode;
    int num;
    char buff[100][16];
}ord;






#endif // HEAD_H
