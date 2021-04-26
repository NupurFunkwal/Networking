#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#define MAX 80  
#define SA struct sockaddr 

// Function designed for chat between client and server. 
void func(int sockfd) 
{ 
	char buff[MAX]; 
	int n;
	FILE *fp; 
	// infinite loop for queries 
	for (int j=0;j<1;j++) { 
		bzero(buff, MAX); 

		// read the message from client and copy it in buffer, the message would be file name
		read(sockfd, buff, sizeof(buff)); 
		if(strcmp(buff,"")==0){
			return;
		}

		// print buffer which contains the client filename
		printf("File request from client: %s\n", buff); 

		//reading 10 bytes from the file
		int count = 0;
		unsigned char file_values[11];
		char ch;
		fp = fopen(buff,"r");
		if( fp==NULL){
			printf("File does not exist! Sending an empty file\n");
			char *empty = "";
			write(sockfd, empty, sizeof(empty));
			//exit(0);
		}
		else{
			while( (ch = fgetc(fp)) != EOF && count<10 ){
				file_values[count++] = ch;
			}
			fclose(fp);	//closing the file
			// send the file values to client 
			file_values[count]="\0";
			write(sockfd, file_values, count);

		}
		
		/*bzero(buff, MAX); 
		n = 0;
		printf("If you want to exit, enter YES else enter NO\n");
		while ((buff[n++] = getchar()) != '\n');
		
		if (strncmp("YES", buff, 3) == 0) { 
			printf("Server Exit...\n"); 
			break; 
		}
  
		bzero(buff,MAX);
		sleep(15); */
		
	} 
} 

// Driver function 
int main(int argc, char **argv)		//port 
{ 
	while(1){
		printf("\n");
		int sockfd, connfd, len; 
		struct sockaddr_in servaddr, cli; 
		int port= atoi(argv[1]);

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
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 		//tells kernel to choose IP itself
		servaddr.sin_port = htons(port); 	//need to give port input here

		//to avoid binding error
		int on =1;
		setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		// Binding newly created socket to given IP and verification 
		if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
			printf("socket bind failed...\n"); 
			exit(0); 
		} 
		else
			printf("Socket successfully binded..\n"); 
	
	
		// Now server is ready to listen and verification 
		if ((listen(sockfd, 5)) != 0) { 
			printf("Listen failed...\n"); 
			exit(0); 
		} 
		else
			printf("Server listening..\n"); 
		len = sizeof(cli); 

		// Accept the data packet from client and verification 
		connfd = accept(sockfd, (SA*)&cli, &len); 
		if (connfd < 0) { 
			printf("server acccept failed...\n"); 
			exit(0); 
			
		} 
		else
			printf("server acccept the client...\n"); 

		// Function for chatting between client and server 
		func(connfd); 
		sleep(1);
		// After chatting close the socket 
		close(sockfd); 
	}	
} 

