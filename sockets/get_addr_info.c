#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>

#include <sys/types.h>
#include <netdb.h>


int main()
{
    int status = 0;

    struct addrinfo hints;
    struct addrinfo *servinfo_list; // список адресов
 
    memset(&hints, 0, sizeof hints); 
    hints.ai_family = AF_INET; // IPv4 
    hints.ai_socktype = SOCK_STREAM; // TCP 
 
    if ((status = getaddrinfo("bsu.by", NULL, &hints, &servinfo_list)) != 0) 
    {
	fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
	exit(1);
    }
 
    char ipstr[INET6_ADDRSTRLEN];
    struct addrinfo *info = servinfo_list;
 
   for (; info != NULL; info = info->ai_next) 
   {
	struct sockaddr_in *ipv4 = (struct sockaddr_in *)info->ai_addr;
	void *addr = &(ipv4->sin_addr);
	inet_ntop(info->ai_family, addr, ipstr, sizeof ipstr);
	printf("%s\n", ipstr);
    }



}



