# Networking
Solved Networking Problems

Contents:

##Lab5

Develop a TCP-based server and client using C. The single client requests data from a file on the server. The server opens the file and sends data back to the client.
Given: The server has few text files files in its present working directory. The client and server will work in separate directories.

Input: The server will accept its port number as the input parameter. The client sends a query to server requesting data from <filename> to the server. The client will accept sever's IP address, server' port number, and the requested <filename> as input parameters.

Output: The server parses the query and returns the first 10 bytes of the file <filename>.
On receiving the data, the client creates the file <filename> and stores the data.

Error handling:
If the server is unreachable, the client must say so.
If the requested <filename> file is missing, the server sends an empty file to the client.
