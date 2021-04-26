#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

// encrypted & decrypted codes length 
int encrylen;
int decrylen;

// encrypt file path & decrypt file path
char * encry_path;
char * decry_path;

char * priv_key;

// encrypt & decrypt file function
int encryptf(FILE * fp,char * pub_key,char * encry_path);
int decryptf(FILE * fp,char * priv_key,char * decry_path);
FILE * ecyfopen(const char * path, const char * mode);
FILE * dcyfopen(const char * path, const char * mode);

// print decrypted codes
int myprint(char * path,char * mode);

FILE * dcyfopen(const char * path, const char * mode)
{
        FILE * fp_encry;
        fp_encry=fopen(encry_path,"r");
        // decrypt encrypted file & store the decrypted file into a new file
        decryptf(fp_encry,priv_key,decry_path);
        //open decrypted file, & return the file pointer
        FILE* decryfp=fopen(decry_path,mode);
        return decryfp;
}

int decryptf(FILE * fp,char * priv_key,char * decry_path)
{
        //read file to str pointer
	    char * encrypted;
        encrypted=(char *)malloc(encrylen * sizeof(char));
        fread(encrypted,encrylen,1,fp);
        fclose(fp);

        char decrypted[1024];

    	// -------------------------------------------------------
    	// use private key to decrypt the encrypted file
    	// -------------------------------------------------------
    	// out private key file 
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
    	decrylen=RSA_private_decrypt(encrylen, encrypted, decrypted, rsa2, RSA_PKCS1_PADDING);
    	if(decrylen==-1){
        	printf("failed to decrypt!\n");
        	return -1;
    	}

    	fclose(priv_fp);
        
        // output decrypted data to a new file
        FILE* ffp=fopen(decry_path,"w");
        if(ffp){
             fwrite(decrypted,decrylen,1,ffp);
             fclose(ffp);
        }
}

int myprint(char * path,char * mode)
{
        FILE * decry_fp;
        char * dcypted_code;
        decry_fp=dcyfopen(path,mode);
        dcypted_code=(char *)malloc(decrylen * sizeof(char));
        fread(dcypted_code,decrylen,1,decry_fp);
        printf("output decrypted code:\n%s\n",dcypted_code);
        return 1;
}

int main(int argc, char** argv)		// private key, filename, decrypt file
{
        FILE * encry_fp = fopen(argv[2], "r");
        FILE * decry_fp;
        char * ecypted_code;
        char * dcypted_code;
	encry_path = (char *) argv[2];
	decry_path = (char *) argv[3];
	priv_key = (char *) argv[1];

	fseek(encry_fp,0,SEEK_END);
        int fsize=ftell(encry_fp);
	encrylen = fsize;
        ecypted_code=(char *)malloc(encrylen * sizeof(char));
        fread(ecypted_code,encrylen,1,encry_fp);

        myprint(argv[2],"r");
        return 1;
}
