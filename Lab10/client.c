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
//char * encry_path="encrydata";
//char * decry_path="decrydata";
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

/*FILE * ecyfopen(const char * path, const char * mode)
{
        FILE * fp_src;
        fp_src=fopen(path,"r");
        // encrypt source file & store the encrypted codes into a new file
        encryptf(fp_src,pub_key,encry_path);

        // open encrypted file, & return the file pointer
        FILE* encyfp=fopen(encry_path,mode);
        return encyfp;
}
*/

/*FILE * dcyfopen(const char * path, const char * mode)
{
        FILE * fp_encry;
        fp_encry=fopen(encry_path,"r");
        // decrypt encrypted file & store the decrypted file into a new file
        decryptf(fp_encry,priv_key,decry_path);
        //open decrypted file, & return the file pointer
        FILE* decryfp=fopen(decry_path,mode);
        return decryfp;
}
*/


int encryptf(char * pub_key, char *plain)
{
        //read file to str pointer
        /*char * plain;
        int fsize;
        fseek(fp,0,SEEK_END);
        fsize=ftell(fp);
        fseek(fp,0,SEEK_SET);
        plain=(char *)malloc(fsize * sizeof(char));
        fread(plain,sizeof(char),fsize,fp);
//        printf("file size is:\n%d\n",fsize);
//        printf("Source is:\n%s\n",plain);
//        printf("strlen is: \n%d\n",strlen(plain));
        fclose(fp);
	*/


        // -------------------------------------------------------
        // use public key to encrypt plain text
        // -------------------------------------------------------
        // open public key file

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

        //printf("in encryptf func, encrylen is:\n%d\n",encrylen);

        // output encrypted data to original file
        /*FILE* ffp=fopen(encry_path,"w");
        if(ffp){
             fwrite(encrypted,encrylen,1,ffp);
             fclose(ffp);
        }
	*/
	
	return 0;
	
}

int decryptf(char * priv_key, char *buffer)
{
        //read file to str pointer
	//char * encrypted;
        //encrypted=(char *)malloc(encrylen * sizeof(char));
        //fread(encrypted,encrylen,1,fp);
        //fclose(fp);

       

    	// -------------------------------------------------------
    	// use private key to decrypt the encrypted file
    	// -------------------------------------------------------
    	// out private key file 

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
        
//        printf("in decryptf func, decrylen is:\n%d\n",decrylen);

    	// output decrypted plain text
        printf("The decrypted string is: \n%s\n",decrypted);

        // output decrypted data to a new file
        /*FILE* ffp=fopen(decry_path,"w");
        if(ffp){
             fwrite(decrypted,decrylen,1,ffp);
             fclose(ffp);
        }*/
}

/*
int myprint()
{
        //FILE * decry_fp;
        char * dcypted_code;
        //decry_fp=dcyfopen(path,mode);
        dcypted_code=(char *)malloc(decrylen * sizeof(char));
        fread(dcypted_code,decrylen,1,decry_fp);
        printf("output decrypted code:\n%s\n",dcypted_code);
        return 1;
}
*/


//send function
void * send_mess(void *arg) 
{ 	
	int sockfd = *((int *)arg);
	//char buffer[SIZE]; 
	//char allow[2];
	/*read(sockfd, allow, sizeof(allow));
	if(strcmp(allow, "0") == 0){
		printf("server busy!\n");
		return;
	}*/
 
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
			
/*		bzero(buffer, sizeof(buffer));			


		//reading the text sent from server  
		read(sockfd, buffer, sizeof(buffer)); 

		if (strcmp(buffer,"exit") == 0){
			printf("exiting!\n");
			return;
		}
		
		//printing the cypher text
		//printf("Cypher text received: %s\n", buffer); 

		//decrypting the cypher
		decryptf(priv_key, buffer);
*/
		sleep(1);
	} 
	//printf("cancelling receive\n");
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
		//char text[1024];
		//bzero(text, 1024);
		//int i = 0;

		// taking user input
/*		printf("Enter the text:\n");
		
		while((text[i++]=getchar()) != '\n');
		text[i-1] = '\0';

		if (strcmp(text,"exit") == 0){
			printf("exiting!\n");
			write(sockfd, text, i);
			return;
		}

		printf("Starting encryption\n");
		//encrypt the line and store it in encrypted
		encryptf(pub_key, text);
			
		//printf("encrypted text: %s\n\n\n", encrypted);

		//sending the cypher text to server
		int check = write(sockfd, encrypted, encrylen);
		if(check==-1){
			printf("server is busy!\n");
			printf("exiting!\n");
			return;
		}
*/			
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

	// function for chat with server 
	//chat(socketfd); 
	
	//printf("Creating threads\n");
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
	
	//printf("Threads completed\n");
	

	// close the socket 
	close(socketfd); 
	
} 

