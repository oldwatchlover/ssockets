
/*
 * Sample server for Steve's Simple Socket Library.
 *
 * (c) Copyright 2012, Steve Anderson.
 *
 *
 * This simple server program that listens on a port for connections, and then
 * it receives some data (text) and echos it to stdout.
 * 
 * Note that it only reads data sent by the client, it does not send any data to the
 * client (that would be an easy extension to the demo)
 *
 */


#include <stdio.h>
#include "ssocklib.h"

#define BUFFER_SIZE	(256)

int main(int argc, char *argv[])
{
    char	server_host[BUFFER_SIZE], buffer[BUFFER_SIZE];
    int		port, sockfd, newsockfd, n;


    if (argc < 2) {
	fprintf(stderr,"usage: %s hostname port\n",argv[0]);
	exit (-1);
    }

	/* template code, this program has no command line arguments yet... */
    while (--argc > 0 && (*++argv)[0] == '-') {
        int	c;
	while (c = *++argv[0]) {
	    switch (c) {
                case 'h':
                case 'u':
                    fprintf(stderr,"usage: server host port\n");
                    exit (1);
		    break;
		default:
		    fprintf(stderr,"unknown option [%c]\n",c);
		    break;
	    }
	}
    }

	/* after processing options, hostname and port are left here: */
    strcpy(server_host, argv[0]);
    port = atoi(argv[1]);

    fprintf(stderr,"server running on [%s] listening to port [%d]\n\n",server_host, port);

    sockfd = CreateSocket();
    if (sockfd < 0) {
	fprintf(stderr,"ERROR : error creating socket\n");
	exit(-1);
    }

    if (BindSocket(sockfd, port) < 0) {
	fprintf(stderr,"ERROR : error binding socket [%d] on [%d]\n",sockfd,port);
	exit(-1);
    }

	/* loop forever, taking keyboard input and sending it over a socket to the server */

    while (1) {
 
        ListenSocket(sockfd, 5); 	/* max of 5 clients waiting in the queue... */

        newsockfd = AcceptSocket(sockfd);	/* take the next client in the queue waiting to connect */
        if (newsockfd < 0) {
           fprintf(stderr,"ERROR : error accepting socket [%d]\n",sockfd);
           exit (-1);
        }

	bzero(buffer, BUFFER_SIZE);	/* clear the buffer of any previous data */

	n = ReadSocket(newsockfd, buffer, BUFFER_SIZE);
        if (n == 0) { 	
		/* client closed the connetion, do nothing */
	} else if (n < 0) {
	    fprintf(stderr,"ERROR : error reading from socket [%d]\n",sockfd);
	    exit (-1);
 	} else {		/* echo the data that was received over the socket */
	    fprintf(stdout,"Server got this message [%s]\n",buffer);
   	}

 	n = CloseSocket(newsockfd);
        if (n < 0) {
 	    fprintf(stderr,"ERROR : error closing socket [%d]\n",newsockfd);
	    exit (-1);
        } 
    }

    n = CloseSocket(sockfd);
    if (n < 0) {
	fprintf(stderr,"ERROR : error closing socket [%d]\n",newsockfd);
    	exit (-1);
    } 

    exit (1); /* success! */
}

