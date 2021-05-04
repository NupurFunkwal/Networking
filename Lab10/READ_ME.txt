There are 2 .c files: server.c and client.c
Client 1 has private-key1.pem and its corresponding public-key1.pem
Client 2 has private-key2.pem and its corresponding public-key2.pem

For compilation of server.c:

	gcc server.c -o server -lpthread

For compilation of client.c:

	gcc client.c -o client -lssl -lcrypto -lpthread

For running server.c:	(command line input contains port number)

	./server 8081

For running Client 1:	(command line input contains IP of server, port number of server, filename of its private key and other client's public key) 
	
	./client 127.0.0.1 8081 private-key1.pem public-key2.pem

For running Client 2:	(command line input contains IP of server, port number of server, filename of its private key and other client's public key)

	./client 127.0.0.1 8081 private-key2.pem public-key1.pem


Now, each client can enter any input to send to the other client in any order(no sequencing order between the clients). Enter "exit" to exit both clients.


A sample run of the code:
Steps:

1) Compile and run server.c in a terminal window.
2) Open two new terminal windows for 2 clients and compile and run client.c.
3) Now enter "Hi" in Client 1.
	Display of Client 1:

		Enter the text:
		Hi
		Encryption done and sent!

	Display in Client 2:
		Cypher text received: �j��9U���1��6�H=�

                                         =�ߓD*��<o��������������vP�y��1��		
                                                                            	 �<�?c�6�*����
            	f]a|��z�L�8R#�o�✇gLl��#J�^���J��r>���p��޻a�

		The decrypted string is: 
		Hi

		
4) Now enter "Hi" in Client 2.
	Display in Client 2:
		Hi
		Encryption done and sent!

	Display in Client 1:
		Cypher text received: &j_r� ��#�혬�����F�0t�hc_���Hx��E��d�>�^	�;P��� 
  		wݙ��%9Q�_
		*�z�%.�I*�%bғ��U)>�H�t�n�N���qt�9�%�~��?޲r�ht_�

		The decrypted string is: 
		Hi

5) Now enter "This is Nupur" in Client 2.
	Display in client 2:
		Enter the text:
		This in Nupur
		Encryption done and sent!

	Display in client 1:
		Cypher text received: `vU24	�&�[����.��X�}Gny����k���J1�c4�
                                                                	'ϲ�rQ�<>6ɝ�_��
		"�.y�D
      		Cq�ڃT�"��{9U
                    	uT��		�WD`��)��_}���f�����cv?M_ɿ��M{�+Z�\r


		The decrypted string is: 
		This in Nupur

6) Now enter "This is Nayanika" in Client 1.
	Display in client 1:
		This is Nayanika
		Encryption done and sent!

	Display in client 2:
		Cypher text received: k.��M��V�������-b����Gݒ�����,�kYK���2U���o�<����������^Ѝ1�x��xq�5��
			eZ���t��a8' @���9�YC���mk_���1�%�ы�K�b�^aW�9hm

		The decrypted string is: 
		This is Nayanika

7) Now enter "exit" in client 2:
	Display in client 2:
		exit
		exiting!

	Display in client 1:
		exiting!






	




