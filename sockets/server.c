// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const *argv[])
{
    pid_t pid;
    int server_fd, client_socket, valread;
    struct sockaddr_in address;
    int requests = 0;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char response[256] = {0};
       
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Setting socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
       
    // Forcefully attaching socket to the port 8080
    printf("Binding socket\n");
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    printf("Listening at port %d\n", PORT);
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
    	if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    	{
            perror("accept");
            exit(EXIT_FAILURE);
        }
	
	requests++;
	printf("Accepting new client connection, requests = %d\n", requests);
	pid = fork();

        if (pid == -1) 
	{
	    perror("fork");
	    exit(EXIT_FAILURE);
        } 
	else if (pid == 0) 
	{
	    // This is child process, which handles client connection
	    valread = read(client_socket , buffer, 1024);
            printf("%s\n", buffer);
    
    	    sprintf(response, "Response from server, request = %d\n", requests);	    
	    send(client_socket, response, strlen(response), 0);
            printf("Response message sent\n");

	    close(client_socket);
	    
	    return 0;
        }
    }

    close(server_fd);

    return 0;
}
