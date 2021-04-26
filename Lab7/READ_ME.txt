There is a single client.c file

For compilation:

	gcc client.c -o client

For running the code (command line argument is the URL):

	./client https://www.google.com/images/branding/googlelogo/1x/googlelogo_color_272x92dp.png


Note:
1) The URL taken as command line input is considered valid only if it begins with http or https. Else it will show error and exit. Also if the URL doesn't exist, it shows error.

2) Upon running the code successfully, a file corresponding to the URL will be generated in the present working directory.
