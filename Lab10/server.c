#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h> 
#include<pthread.h>

#define SIZE 128

char buffer[128];
int count = 0;
pthread_t tid[2];		//2 threads for clients
int sock_ids[2];

void * socketThread(void *arg)
{		
  	int sockfd = *((int *)arg);

	while(1) {
		char buff[SIZE];  
		bzero(buff, SIZE); 

		// read the message from client which is the cypher text
		read(sockfd, buff, sizeof(buff)); 

		//printf("The cypher text is: %s\n", buff);

		/*if(strcmp(buff,"")==0){
			continue;
		}*/
			
		//sending exit message to other client
		if(strcmp(buff,"exit")==0){
			if(count==2){
				if(sock_ids[0] == sockfd){
					write(sock_ids[1], buff, 4);
				}
				else{
					write(sock_ids[0], buff, 4);
				}
				break;
			}
			else if(count==1){	
				break;	
			}
		}

		//sending the cypher text to the other client
		if(sock_ids[0] == sockfd){
			write(sock_ids[1], buff, sizeof(buff));
		}
		else{
			write(sock_ids[0], buff, sizeof(buff));
		}

  		sleep(1);	
	} 
  
  	printf("\nExit client (id = %d)\n", sockfd);
	count--;
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

	  
	  int i = 0;
	  while(1)
	    {
		addr_size = sizeof serverStorage;

		newSocket = accept(serverSocket, (struct sockaddr *) &serverStorage, &addr_size);
		count++;
		if(count>2){
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
		sock_ids[i] = newSocket;
		printf("new socket id = %d for i =%d\n ", sock_ids[i], i);
		if( pthread_create(&tid[i++], NULL, socketThread, &newSocket) != 0 ){
		   	printf("Failed to create thread\n");
		}
		

		if(count == 2){
			i=0;
		}
	  }
	  return 0;
}
