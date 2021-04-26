#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#define MAX 80 
#define SA struct sockaddr 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	FILE *fp;
	for (int j=1;j<2;j++) { 
		char filename[30];
		int i=0;
		printf("Enter the filename:\n");
		
		while((filename[i++]=getchar()) != '\n');
		filename[i-1] = '\0';
			
		if((strncmp(filename, "exit", 4)) ==0){
			printf("Client Exit...\n");
			exit(0);	
		}
		//sending the filename to server
		write(sockfd, filename, i);
		
		bzero(buff, sizeof(buff));
		//reading the data from the file  
		read(sockfd, buff, sizeof(buff)); 
		
		//creating a new file for client 
		fp = fopen(filename, "w");
		if(fp == NULL){
			printf("Cannot make new file!\n");
			exit(1);
		}
		
		int results = fputs(buff, fp);
		if(results == EOF) {
			printf("No input received!\n");
			exit(1);
		}
		fclose(fp);
		printf("\n");

		
	} 
} 

int main( int argc, char** argv) 	//IP port
{ 	
	int reconnect = 2;
	printf("Enter exit to exit from the client\n");
	while(1){
		sleep(1);
		int sockfd, connfd; 
		int port = atoi(argv[2]);
		struct sockaddr_in servaddr, cli; 

		// socket create and verification 
		sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
		if (sockfd == -1) { 
			printf("socket creation failed...\n"); 
			exit(0); 
		} 
		else
			printf("Socket successfully created..\n"); 
		bzero(&servaddr, sizeof(servaddr)); 

		// assign IP, PORT 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
		servaddr.sin_port = htons(port); 

		// connect the client socket to server socket 
		if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
			printf("connection with the server failed...\n"); 
			//exit(0);
			printf("Trying to reconnect\n");
			reconnect--;
			if(reconnect==0){
				exit(1);
			}
			else{
				sleep(5);
				continue; 
			}
		} 
		else
			printf("connected to the server..\n"); 

		// function for chat with server 
		func(sockfd); 

		// close the socket 
		close(sockfd); 
	}
} 

