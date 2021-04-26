#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>

// encrypted & decrypted codes length 
int encrylen;
int decrylen;

// encrypt file path & decrypt file path
char *encry_path;
char *pub_key;

// encrypt & decrypt file function
int encryptf(FILE * fp,char * pub_key,char * encry_path);
int decryptf(FILE * fp,char * priv_key,char * decry_path);
FILE * ecyfopen(const char * path, const char * mode);
FILE * dcyfopen(const char * path, const char * mode);

// print decrypted codes
int myprint(char * path,char * mode);

FILE * ecyfopen(const char * path, const char * mode)
{
        FILE * fp_src;
        fp_src=fopen(path,"r");
        // encrypt source file & store the encrypted codes into a new file
        encryptf(fp_src,pub_key,encry_path);

        // open encrypted file, & return the file pointer
        FILE* encyfp=fopen(encry_path,mode);
        return encyfp;
}

int encryptf(FILE * fp,char * pub_key,char * encry_path)
{
        //read file to str pointer
        char * plain;
        int fsize;
        fseek(fp,0,SEEK_END);
        fsize=ftell(fp);
        fseek(fp,0,SEEK_SET);
        plain=(char *)malloc(fsize * sizeof(char));
        fread(plain,sizeof(char),fsize,fp);
        fclose(fp);

        // used to store encrypted file
        char encrypted[1024];

        // -------------------------------------------------------
        // use public key to encrypt plain text
        // -------------------------------------------------------
        // open public key file
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
        encrylen=RSA_public_encrypt(fsize, plain, encrypted, rsa1, RSA_PKCS1_PADDING);
        if(encrylen==-1 ){
                printf("failed to encrypt\n");
                return -1;
        }

        // output encrypted data to original file
        FILE* ffp=fopen(encry_path,"w");
        if(ffp){
             fwrite(encrypted,encrylen,1,ffp);
             fclose(ffp);
        }
}

int main(int argc, char** argv)			// public key , filename, encrypt name
{
        FILE * encry_fp;
        char * ecypted_code;
	
	encry_path = argv[3];
	pub_key = argv[1];
	
        encry_fp=ecyfopen(argv[2],"r");
        ecypted_code=(char *)malloc(encrylen * sizeof(char));
        fread(ecypted_code,encrylen,1,encry_fp);
        printf("output encrypted code:\n%s\n",ecypted_code);
        return 1;
}
