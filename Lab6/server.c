#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> 
#include<pthread.h>
#define SIZE 1024

char buffer[1024];
int count = 0;

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

void * socketThread(void *arg)
{		
  	int sockfd = *((int *)arg);

	while(1) {
		char buff[SIZE];  
		bzero(buff, SIZE); 
		char text[1024];

		// read the message from client which is the text
		read(sockfd, buff, sizeof(buff)); 
		/*if(strcmp(buff,"")==0){
			continue;
		}*/
		if(strcmp(buff,"exit")==0){
			break;
		}

		// print reversed buffer 
		reverse(buff);
		printf("Reversed text of client(id = %d): %s\n", sockfd, buff);

		//read a line from standard input
		int i = 0;
		printf("Enter the text for client (id = %d):\n", sockfd);

		while((text[i++]=getchar()) != '\n');
		text[i-1] = '\0';

		//sending the text to client
		write(sockfd, text, i);
  		sleep(1);	
	} 
  
  	printf("Exit client (id = %d)\n", sockfd);
	count --;
  	close(sockfd);
  	pthread_exit(NULL);
	
}

int main(int argc, char** argv){
	  int serverSocket, newSocket;
	  struct sockaddr_in serverAddr;
	  struct sockaddr_storage serverStorage;
	  socklen_t addr_size;
	  int port = atoi(argv[1]);		//port from command line

	  //Creating socket 
	  serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	  serverAddr.sin_family = AF_INET;
	  serverAddr.sin_port = htons(port);
	  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	  memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	  //Binding 
	  bind(serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr));

	  //Listening
	  if(listen(serverSocket,1)==0){
	    printf("Listening for connections...\n");
	  }
	  else{
	    printf("Error in listening\n");
	  }

	  pthread_t tid[4];		//4 threads for clients
	  int i = 0;
	  while(1)
	    {
		addr_size = sizeof serverStorage;

		/*if(count>=5){
			//printf("Server full\n");
			sleep(1);
			continue;
		}*/

		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
		count++;
		if(count>4){
			char allow[2] = "0\0";		// not allowed
			write(newSocket, allow, 2);
			count --;
			close(newSocket);
			continue;
		}
		else{
			char allow[2] = "1\0";		// allowed
			write(newSocket, allow, 2);
		}

		//make thread for each allowed client
		if( pthread_create(&tid[i++], NULL, socketThread, &newSocket) != 0 )
		   printf("Failed to create thread\n");

		i=0;
	  }
	  return 0;
}
