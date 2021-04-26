# Networking
Solved Networking Problems

## Contents:

### Lab5

Develop a TCP-based server and client using C. The single client requests data from a file on the server. The server opens the file and sends data back to the client.
Given: The server has few text files files in its present working directory. The client and server will work in separate directories.

Input: The server will accept its port number as the input parameter. The client sends a query to server requesting data from <filename> to the server. The client will accept sever's IP address, server' port number, and the requested <filename> as input parameters.

Output: The server parses the query and returns the first 10 bytes of the file <filename>.
On receiving the data, the client creates the file <filename> and stores the data.

Error handling:
If the server is unreachable, the client must say so.
If the requested <filename> file is missing, the server sends an empty file to the client.
  
  
### Lab6

A TCP server and a client for the following.
1. The server as a command line argument accepts the port number to which it should bind.
2. The client, as command line arguments, accepts the IP address and the port number at which it will find the server. 
3. After connecting to the server, the client reads a line from the standard input and sends it to the server. 
4. The server prints the received line in the reverser order and reads a line from the standard input and sends it to the client or all the clients.
5. The client prints the received line in the reverse order and is ready to accept a new line from the user. 
6. The client exits if the user type "exit". 
7. The server at a time accepts at the most four clients. Any client above the limit is rejected. (4 marks) As and when the number is less than the limit, the server accepts the new client.


### Lab7

A TCP client to download and locally save a file.

1. The client, as a command-line argument, accepts the URL of the file. The URL can be HTTP or HTTPS-based.
2. If the URL does not exist or unreachable, the client prints an error and exits.
3. After connecting to the server, the client sends a GET request to the server.
4. The client downloads the whole file and saves it to the local folder with the same file name as mentioned in the URL.
5. If a file with the same name exists, the old file is overwritten.
6. The client exits after downloading the file.


### Lab9

A program that uses public key-private key encryption to encrypt and decrypt a text file. 
The maximum size of the file is 1Kbytes. 

1. The encryption program accepts public key, name of the file, and name of the output file as command line input parameters. It encrypts the file and stores it as the given name of the output file.

2. The decryption program accepts private key, name of the encrypted file, and name of the decrypted file as command line input parameters. It decrypts the file and stores it as the given name of the output file.


### Lab10

There are two clients and a server in the application. The two clients communicate privately with each other via the server. When any client sends an encrypted message, the message and its decryption are displayed in the other client.

1. The server as a command line argument accepts the port number to which it should bind.
2. Each client, as command line arguments, accepts the IP address at which it will find the server, the port number of the server at that IP address, the filename containing its private key, and the filename containing the other client's public key.
3. After connecting to the server, the client keeps on reading a line from the standard input. It encrypts the line and sends it to the server. 
4. The server forwards the message received from one client to another. 
5. When a client receives a message from the other client, it decrypts and displays both ciphertext and plaintext. 
6. Both the clients exit if the user types "exit" in any client. 


