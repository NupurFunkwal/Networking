//2018A7PS0624G

#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>

#define SIZE 1024
#define MAX_SIZE 1024*1024
 
void substring(char *substr, char *str, int start, int end) {
    int i;

    for(i = start; i < end; i++)
        substr[i-start] = str[i];
 
    substr[i] = '\0';
}
 
void getURL(char *site, char domain[], char path[], char file_name[]) {
    char s1[SIZE], s2[SIZE];
    substring(s1, site, 0, 7);
    substring(s2, site, 0, 8);

    int i;
    int pos = 0;
 
    // derive domain name from the site
    if(strcmp(s2, "https://") == 0) {
        for(i = 8; site[i] != '/' && site[i] != '\0'; i++);
        substring(domain, site, 8, i);
    }
    else if(strcmp(s1, "http://") == 0) {
        for(i = 7; site[i] != '/' && site[i] != '\0'; i++);
        substring(domain, site, 7, i);
    }
    else{
        printf("Invalid site\n");
        exit(0);
    }
 
    //path
    substring(path, site, i, strlen(site)+1);
 
    //filename
    for(i = 0; i < strlen(path); i++) {
        if(path[i] == '/')
            pos = i;
    }
 
    substring(file_name, path, pos + 1, strlen(path)+1);
}
 
int HttpStatus(int sock_fd){
    char c;
    char buff[SIZE] = " ";
    char *ptr = buff + 1;
    int bytes_received, status_code;
 
    // receiving 
    while(bytes_received = recv(sock_fd, ptr, 1, 0)){
        if(bytes_received == -1){
            perror("ReadHttpStatus");
            exit(1);
        }

        // end of the status line
        if((ptr[-1]=='\r')  && (*ptr=='\n')){ 
            break;
        }

        ptr++;
    }
 
    //end of status
    *ptr = '\0';    
    ptr = buff + 1;
 
    //status code
    sscanf(ptr, "%*s %d ", &status_code);  
 
    printf("%s\n", ptr);

    printf("status = %d\n",status_code);

    if(bytes_received > 0){
        return status_code;
    }
    else return 0;
}
 
 
int parseHeader(int sock_fd){
    char c;
    char buff[SIZE] = "";
    char *ptr = buff + 4;
    int bytes_received;
 
    // Receiving the data from the server, one byte at a time
    while(bytes_received = recv(sock_fd, ptr, 1, 0)){
        if(bytes_received == -1){
            printf("Receiving failed...\n");
            exit(1);
        }
 
        // checking \r\n\r\n
        if((ptr[-3]=='\r') && (ptr[-2]=='\n') && (ptr[-1]=='\r') && (*ptr=='\n')){
            break;
        }

        ptr++;
    }
 
    *ptr = '\0';
    ptr = buff + 4;
    char *temp;
    
    if(bytes_received){
        temp = strstr(ptr, "content-length:");
        if(temp){
            sscanf(temp, "%*s %d", &bytes_received);    
        }
        else {
            temp = strstr(ptr, "Content-Length:");
            if(temp){
                sscanf(temp, "%*s %d", &bytes_received);  
            }
            else{
                bytes_received = -1; //unknown
            }
        }
 
        printf("Content-Length: %d\n ", bytes_received);
 
    }
 
    return bytes_received ;
}
 
 
int main(int argc, char **argv) {
    char *url = argv[1];
    char domain[SIZE];
    char path[SIZE];
    char file_name[SIZE];
    getURL(url, domain, path, file_name);
 
    printf("Domain: %s\n", domain);
    printf("Path: %s\n", path);
    printf("File : %s\n", file_name);
    printf("\n");
 
    int sock_fd, bytes_received;
    char send_data[MAX_SIZE];
    char recv_data[MAX_SIZE];
    struct sockaddr_in server_addr;
    struct hostent *he;
 
    he = gethostbyname(domain);
    if(he == NULL) {
        printf("Invalid URL\n");
        exit(0);
    }
 
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    if(sock_fd == -1) {
        printf("Socket creation failed...\n");
        exit(1);
    }
    else{
        printf("Socket successfully created...\n");
    }
 
    server_addr.sin_family = AF_INET;       
    server_addr.sin_port = htons(80);       
    server_addr.sin_addr = *((struct in_addr *)he->h_addr);
 
    // connect the client socket to the server socket
    if(connect(sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) { 
        printf("Connection with the server failed!\n"); 
        exit(0); 
    } 
    else{
        printf("Connected to the server\n"); 
    }
 
    snprintf(send_data, sizeof(send_data), "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", path, domain);
 
    if(send(sock_fd, send_data, strlen(send_data), 0)==-1){
        printf("Couldn't send the request!\n");
        exit(1); 
    }
    else{
        printf("Request sent\n");
    }
 
    printf("Receiving data..\n");
    int content_length = 0, status_code;
 
    if((status_code = HttpStatus(sock_fd)) && (content_length = parseHeader(sock_fd))) {
        if(status_code != 200) {
            printf("Incorrect request!\n");
            exit(1);
        }
 
        int bytes = 0;
        FILE *file = fopen(file_name, "wb");
        printf("Saving data\n");
 
        while(bytes_received = recv(sock_fd, recv_data, MAX_SIZE, 0)) {
            if(bytes_received == -1) {
                printf("Download error!\n");
                exit(0);
            }
 
            fwrite(recv_data, 1, bytes_received, file);
            bytes += bytes_received;
            

            if(bytes == content_length)
                break;
        }
        
        fclose(file);
        printf("\nDownload complete :)\n\n");
    }
 
    close(sock_fd);
    //printf("\nDownload complete :)\n\n");
    return 0;
}