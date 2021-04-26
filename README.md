# Networking
Solved Networking Problems

##Contents:

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
4. The server prints the received line in the reverser order (2 marks) and reads a line from the standard input and sends it to the client or all the clients.
5. The client prints the received line in the reverse order and is ready to accept a new line from the user. 
6. The client exits if the user type "exit". 
7. The server at a time accepts at the most four clients. (4 marks) Any client above the limit is rejected. (4 marks) As and when the number is less than the limit, the server accepts the new client.
