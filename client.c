
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
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "ssocklib.h"

#define BUFFER_SIZE	(256)

int main(int argc, char *argv[])
{
    char	server_host[BUFFER_SIZE], line[BUFFER_SIZE], *s;
    int		port, sockfd, n;

    if (argc < 2) {
	fprintf(stderr,"usage: %s server port\n",argv[0]);
	exit (EXIT_FAILURE);
    }

	/* template code, this program has no command line arguments yet... */
    while (--argc > 0 && (*++argv)[0] == '-') {
        int	c;
	while ((c = (*++argv)[0])) {
	    switch (c) {
		case 'h':
		case 'u':
		    fprintf(stderr,"usage: client host port\n");
		    exit (EXIT_SUCCESS);
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

    sockfd = CreateSocket();
    if (sockfd < 0) {
        fprintf(stderr,"ERROR : %s : error creating socket errno = %d.\n",__FILE__,errno);
	exit (EXIT_FAILURE);
    }

    if (ConnectSocket(sockfd, server_host, port) < 0) {
        fprintf(stderr,"ERROR : %s : error connecting to socket [%s:%d] errno = %d.\n",
		__FILE__,server_host,port,errno);
	exit (EXIT_FAILURE);
    }

    while (true) {
	if ((s = fgets(line, BUFFER_SIZE, stdin)) == NULL) {
	    break;	/* user typed control-D to end the input */
	}

	line[strlen(line)-1] = '\0';	/* remove the newline from the input */

        n = SendSocket(sockfd, line, strlen(line));
        if (n < 0) {
	    fprintf(stderr,"ERROR : %s : error writing [%s] to socket [%d] errno = %d\n",
		__FILE__,line,sockfd,errno);
	    exit (EXIT_FAILURE);
        }
    }

    n = CloseSocket(sockfd);
    if (n < 0) {
	fprintf(stderr,"ERROR : %s : error closing socket [%d] errno = %d\n",
		__FILE__,sockfd,errno);
	exit (EXIT_FAILURE);
    }

    exit (EXIT_SUCCESS);
}

