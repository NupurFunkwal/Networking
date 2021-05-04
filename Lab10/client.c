#include <sys/socket.h>  
#include <stdio.h>
#include <string.h>  
#include <stdlib.h> 
#include <netdb.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

#define SA struct sockaddr
#define SIZE 1024

pthread_t tid[2];

// used to store encrypted file
char encrypted[1024];
char decrypted[1024];

// encrypted & decrypted codes length 
int encrylen;
int decrylen;

// encrypt file path & decrypt file path
char *encry_path;
char * decry_path;

// public key file & private key file
char *pub_key;
char *priv_key;

// encrypt & decrypt file function
int encryptf(char * pub_key,char * encry_path);
int decryptf(char * priv_key,char * decry_path);
FILE * ecyfopen(const char * path, const char * mode);
FILE * dcyfopen(const char * path, const char * mode);

// print decrypted codes
int myprint(char * path,char * mode);

int encryptf(char * pub_key, char *plain)
{
	bzero(encrypted, 1024);
        FILE* pub_fp=fopen(pub_key,"r");
        if(pub_fp==NULL){
                printf("failed to open pub_key file %s!\n", pub_key);
                return -1;
         }

        // read public key from file
        RSA* rsa1=PEM_read_RSA_PUBKEY(pub_fp, NULL, NULL, NULL);
        if(rsa1==NULL){
                printf("unable to read public key!\n");
                return -1;
        }

        if(strlen(plain)>=RSA_size(rsa1)-41){
                printf("failed to encrypt\n");
                return -1;
        }
        fclose(pub_fp);

        // use public key to encrypt
	int fsize = strlen(plain); 
        encrylen=RSA_public_encrypt(fsize, plain, encrypted, rsa1, RSA_PKCS1_PADDING);
        if(encrylen==-1 ){
                printf("failed to encrypt\n");
                return -1;
        }	
	return 0;
	
}

int decryptf(char * priv_key, char *buffer)
{
       
	bzero(decrypted, 1024);
    	FILE* priv_fp=fopen(priv_key,"r");
    	if(priv_fp==NULL){
        	printf("failed to open priv_key file %s!\n", priv_key);
        	return -1;
    	}

    	// read private key from private key file
    	RSA *rsa2 = PEM_read_RSAPrivateKey(priv_fp, NULL, NULL, NULL);
    	if(rsa2==NULL){
        	printf("unable to read private key!\n");
        	return -1; 
    	}
    
    	// use private key to decrypt encrypted data
	int buf_len = 128;
    	decrylen=RSA_private_decrypt(buf_len, buffer, decrypted, rsa2, RSA_PKCS1_PADDING);
    	if(decrylen==-1){
        	printf("failed to decrypt!\n");
        	return -1;
    	}

    	fclose(priv_fp);
        
    	// output decrypted plain text
        printf("The decrypted string is: \n%s\n",decrypted);
}

//send function
void * send_mess(void *arg) 
{ 	
	int sockfd = *((int *)arg);
	
	while(1) { 
		char text[1024];
		bzero(text, 1024);
		int i = 0;

		// taking user input
		printf("Enter the text:\n");
		
		while((text[i++]=getchar()) != '\n');
		text[i-1] = '\0';

		if (strcmp(text,"exit") == 0){
			printf("exiting!\n");
			write(sockfd, text, i);
			break;
		}

		//encrypt the line and store it in encrypted
		encryptf(pub_key, text);
			
		//printf("encrypted text: %s\n\n\n", encrypted);

		//sending the cypher text to server
		int check = write(sockfd, encrypted, encrylen);
		if(check==-1){
			printf("server is busy!\n");
			printf("exiting!\n");
			break;
		}

		printf("Encryption done and sent!\n");
			
		sleep(1);
	} 
	pthread_cancel(tid[0]);
	pthread_exit(NULL);
} 

void * receive(void *arg) 
{ 	
	int sockfd = *((int *)arg);
	char buffer[SIZE]; 
	char allow[2];
	read(sockfd, allow, sizeof(allow));
	if(strcmp(allow, "0") == 0){
		printf("server busy!\n");
		pthread_exit(NULL);
	}
 
	while(1) { 
			
		bzero(buffer, sizeof(buffer));
			

		//reading the text sent from server  
		read(sockfd, buffer, sizeof(buffer)); 

		if (strcmp(buffer,"exit") == 0){
			printf("exiting!\n");
			write(sockfd, buffer, 4);
			break;
		}
		
		//printing the cypher text
		printf("Cypher text received:\n%s\n\n", buffer); 

		//decrypting the cypher
		decryptf(priv_key, buffer);

		sleep(1);
	} 
	//printf("exiting sender\n");
	pthread_cancel(tid[1]);
	pthread_exit(NULL);
} 



//Main function
int main( int argc, char** argv) 	//input IP, port, filename of private key, filename of public key of other client
{ 	
	sleep(1);
	int socketfd, connectionfd; 
	int port = atoi(argv[2]);	//port number

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

	priv_key = argv[3];
	pub_key = argv[4];

	for(int i=0; i<2; i++){
		if(i==0){
			pthread_create(&tid[i], NULL, receive, &socketfd);
		}
		else{
			pthread_create(&tid[i], NULL, send_mess, &socketfd);
		}
	}

	for(int i=0; i<2; i++){
		pthread_join(tid[i], NULL);
	}
	
	// close the socket 
	close(socketfd); 
	
} 

