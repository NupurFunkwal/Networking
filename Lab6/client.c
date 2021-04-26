#include <sys/socket.h>  
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#include <netdb.h>

#define SA struct sockaddr
#define SIZE 1024 

char *reverse(char *s)
{
      	char *a = s;
      	char *b = s + strlen(s) - 1 ;	

      	if (! *s || ! s){
            	return s;
	}

      	for ( ; a < b ; a++, b--){
            	*a = *a ^ *b;
           	*b = *b ^ *a;
            	*a = *a ^ *b;
     	}

      	return s;
}


//chat function
void chat(int sockfd) 
{ 
	char buffer[SIZE]; 
	char allow[2];
	read(sockfd, allow, sizeof(allow));
	if(strcmp(allow, "0") == 0){
		printf("server busy!\n");
		return;
	}
 
	while(1) { 
		char text[1024];
		int i = 0;
		printf("Enter the text:\n");
		
		while((text[i++]=getchar()) != '\n');
		text[i-1] = '\0';
			
		//sending the text to server
		int check = write(sockfd, text, i);
		if(check==-1){
			printf("server is busy!\n");
			printf("exiting!\n");
			return;
		}
		if (strcmp(text,"exit") == 0){
			printf("exiting!\n");
			return;
		}
		
		bzero(buffer, sizeof(buffer));			

		//reading the text sent from server  
		read(sockfd, buffer, sizeof(buffer)); 
		
		//printing reverse text and exiting
		reverse(buffer);
		printf("Reversed text: %s\n", buffer); 
		sleep(1);
	} 
} 


//Main function
int main( int argc, char** argv) 	//input IP and port in command line input
{ 	
	sleep(1);
	int socketfd, connectionfd; 
	int port = atoi(argv[2]);

	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	socketfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (socketfd == -1) { 
		printf("socket creation failed!\n"); 
		exit(0); 
	} 
	else{
		printf("Socket successfully created.\n");
	} 

	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(port);	// port number input
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); 		// IP input
	 

	// connect the client socket to server socket 
	if (connect(socketfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed!\n"); 
		exit(0);
	} 
	else{
		; 
	}

	// function for chat with server 
	chat(socketfd); 

	// close the socket 
	close(socketfd); 
	
} 

