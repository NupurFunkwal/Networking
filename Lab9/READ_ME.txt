There are 2 .c files: encrypt.c contains encryption program and decrypt.c contains decryption program.
"nupur.txt" is the text file
"encoded_file" is the encryption of "nupur.txt"
The folder also contains "public.pem" and "private.pem" keys

For Compilation:

gcc encrypt.c -o encrypt -lssl -lcrypto

gcc decrypt.c -o decrypt -lssl -lcrypto



For execution:

1) encrypt.c needs 3 command line arguements: public key file name, filename of file to be encrypted, filename of encryption
Eg: ./encrypt public.pem nupur.txt encoded_file

2) decrypt.c needs 3 command line arguements: private key file name, filename of encrypted file, filename of decryption
Eg: ./decrypt private.pem encoded_file decoded_file
 

After execution, encrypted and decrypted files will be created in the same working folder.


