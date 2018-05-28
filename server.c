
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
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <errno.h>

#include "ssocklib.h"

#define BUFFER_SIZE	(256)
#define HOST_NAME_MAX	BUFFER_SIZE	/* _POSIX_HOST_NAME_MAX is 255 */

static int sockfd;

/* catch SIGINT to clean up before exit... */
static void intHandler(int sig)
{
    fprintf(stderr,"Server caught SIGINT!\n");

    CloseSocket(sockfd); /* ignore errors, we are exiting... */

    exit (EXIT_SUCCESS);
}


int main(int argc, char *argv[])
{
    char	server_host[HOST_NAME_MAX], buffer[BUFFER_SIZE];
    int		port, newsockfd, n;
    bool	connection_alive = false;


    if (argc < 2) {
	fprintf(stderr,"usage: %s hostname port\n",argv[0]);
	exit (EXIT_FAILURE);
    }

	/* template code, this program has no command line arguments yet... */
    while (--argc > 0 && (*++argv)[0] == '-') {
        int	c;
	while ((c = (*++argv)[0])) {
	    switch (c) {
                case 'h':
                case 'u':
                    fprintf(stderr,"usage: server host port\n");
                    exit (EXIT_SUCCESS);
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

    /* catch the SIGINT (control-C on Unix) so when we kill the server
     * with control-C we exit gracefully and dump out the data that was dispatched.
     */
    signal(SIGINT, intHandler);

    fprintf(stderr,"server running on [%s] listening to port [%d]\n\n",server_host, port);

    sockfd = CreateSocket();
    if (sockfd < 0) {
	fprintf(stderr,"ERROR : %s : error creating socket errno = %d\n",__FILE__,errno);
	exit(EXIT_FAILURE);
    }

    if (BindSocket(sockfd, port) < 0) {
	fprintf(stderr,"ERROR : %s : error binding socket [%d] on [%d] errno = %d\n",
		__FILE__,sockfd,port,errno);
	exit(EXIT_FAILURE);
    }

	/* loop forever, accepting any socket connections and reading/echoing what they send us */

    while (true) {
 
        ListenSocket(sockfd, 5); 	/* max of 5 clients waiting in the queue... */

        newsockfd = AcceptSocket(sockfd);	/* take the next client in the queue waiting to connect */
        if (newsockfd < 0) {
           fprintf(stderr,"ERROR : %s : error accepting socket [%d] errno = %d\n",
			__FILE__,sockfd,errno);
	    exit(EXIT_FAILURE);
        }
	
	connection_alive = true;

	while (connection_alive) {

	    bzero(buffer, BUFFER_SIZE);	/* clear the buffer of any previous data */

	    /* don't read the whole BUFFER_SIZE... guarentees no overflow and that the text
	     * will be NULL-terminated 
	     */
	    n = RecvSocket(newsockfd, buffer, BUFFER_SIZE-2);
            if (n == 0) { 	
		/* client closed the connetion, exit this loop */
		connection_alive = false;
	    } else if (n < 0) {
	        fprintf(stderr,"ERROR : %s : error receiving from socket [%d] errno = %d\n",
			__FILE__,sockfd,errno);
	        exit(EXIT_FAILURE);
 	    } else {		/* echo the data that was received over the socket */
	        fprintf(stdout,"%s\n",buffer);
   	    }
   	}

 	n = CloseSocket(newsockfd);
        if (n < 0) {
 	    fprintf(stderr,"ERROR : %s : error closing socket [%d] errno = %d\n",
			__FILE__,newsockfd,errno);
	    exit(EXIT_FAILURE);
        } 
    }

    n = CloseSocket(sockfd);
    if (n < 0) {
	fprintf(stderr,"ERROR : %s : error closing socket [%d] errno = %d\n",
		__FILE__,newsockfd,errno);
	exit(EXIT_FAILURE);
    } 

    exit (EXIT_SUCCESS); 
}

