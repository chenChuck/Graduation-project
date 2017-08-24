#include "head.h"

int Tcp_Server_init(int port)
{
	int socket_fd = socket(PF_INET,SOCK_STREAM,0);
	if(-1 == socket_fd){bug("socket() ");exit(0);}
	
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = inet_addr("0.0.0.0");
	
	int ret = bind(socket_fd,(struct sockaddr *)(&server_addr),sizeof(server_addr));
	if( -1 == ret){bug("bind() "); exit(0);}

	ret  = listen(socket_fd,3);
	if(-1 == ret){bug("listen() "); exit(0);}

	return socket_fd;
}
