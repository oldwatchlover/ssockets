
/*
 * Sample client for Steve's Simple Socket Library.
 *
 * (c) Copyright 2012, Steve Anderson.
 *
 *
 * This simple client opens a socket at hostname:port and then reads
 * stdin and passes whatever you type to the server using the socket. 
 *
 * Note that it only sends data to the server, it does not receive any data from the
 * server (that would be an easy extension to the demo)
 *
 */

#include <stdio.h>
#include "ssocklib.h"

#define BUFFER_SIZE	(256)

int main(int argc, char *argv[])
{
    char	server_host[BUFFER_SIZE], line[BUFFER_SIZE], buffer[BUFFER_SIZE], *s;
    int		port, sockfd, n;
    size_t	buffsize;

    if (argc < 2) {
	fprintf(stderr,"usage: %s server port\n",argv[0]);
	exit (-1);
    }

	/* template code, this program has no command line arguments yet... */
    while (--argc > 0 && (*++argv)[0] == '-') {
        int	c;
	while (c = *++argv[0]) {
	    switch (c) {
		case 'h':
		case 'u':
		    fprintf(stderr,"usage: client host port\n");
		    exit (1);
		    break;
		default:
		    fprintf(stderr,"unknown option [%c]\n",c);
		    break;
	    }
	}
    }

	/* after processing options, hostname and port are left here */
    strcpy(server_host, argv[0]);
    port = atoi(argv[1]);

    fprintf(stderr,"client connecting to [%s] port [%d]\n",server_host, port);

	/* loop forever, reading stdin and sending it over the socket to the server */
    while (1) {

	if (s = fgets(line, BUFFER_SIZE, stdin) == NULL) {
	    break;	/* user typed control-D to end the input */
	}

        sockfd = CreateSocket();
        if (sockfd < 0) {
	    fprintf(stderr,"ERROR : error creating socket.\n");
	    exit (-1);
        }

        if (ConnectSocket(sockfd, server_host, port) < 0) {
	    fprintf(stderr,"ERROR : error connecting to socket [%s:%d].\n",server_host,port);
	    exit (-1);
        }

	line[strlen(line)-1] = '\0';	/* remove the newline from the input */

        n = WriteSocket(sockfd, line, strlen(line));
        if (n < 0) {
	    fprintf(stderr,"ERROR : error writing [%s] to socket [%d]\n",line,sockfd);
   	    exit (-1); 
        }

        n = CloseSocket(sockfd);
	if (n < 0) {
	    fprintf(stderr,"ERROR : error closing socket [%d]\n",sockfd);
	    exit(-1);
	}
    }

    exit (1); /* success! */
}

