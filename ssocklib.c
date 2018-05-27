
/*
 * ssocklib.c
 * 
 * Simple unix TCP/IP socket library (does NOT do UDP packets)
 *
 * The purpose of this simple library abstracts the "guts" of setting up and using sockets for simple
 * IPC, hiding all the unix kernal include files, data structures, and obscure option flags in here.
 *
 * (c) Copyright 2012, Steve Anderson
 *
 */

/* you can define DEBUG up here if you want to print out more verbose information and error details */

#ifdef DEBUG
#include <stdio.h>
#endif
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <errno.h>

#include "ssocklib.h"

/*
 * create a socket
 *
 */
int CreateSocket(void)
{
    int	sockfd;

    sockfd = socket(AF_INET, SOCK_STREAM, 0); /* TCP/IP sockets only here for now */

#ifdef DEBUG
    fprintf(stderr,"%s : CreateSocket(void) returning %d\n",__FILE__,sockfd);

    if (sockfd < 0) {
        fprintf(stderr,"ERROR : %s : socket() creation failed. errno = %d\n",
		__FILE__, errno);
    }
#endif

    return (sockfd);
}

/*
 * close a socket
 *
 */
int CloseSocket(int sockfd)
{
    int retval;

    retval = close(sockfd);

#ifdef DEBUG
    fprintf(stderr,"%s : CloseSocket(%d) returning %d\n",__FILE__,sockfd,retval);

    if (retval < 0) {
        fprintf(stderr,"ERROR : %s : close() socket failed. errno = %d\n",
		__FILE__, errno);
    }
#endif

    return (retval);
}


/*
 * bind a socket to a port
 *
 * A host is not needed (only a port) becuase this function gets called from the server
 * and the host is implied
 *
 */
int BindSocket(int sockfd, int port)
{
    int	retval;
    struct sockaddr_in serv_addr;

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(port);

    retval = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

#ifdef DEBUG
    fprintf(stderr,"%s : BindSocket(%d, %d) returning %d\n",__FILE__,sockfd,port,retval);

    if (retval < 0) {
        fprintf(stderr,"ERROR : %s : socket bind(%d, %d) failed. errno = %d\n",
		__FILE__, sockfd, port, errno);
    }
#endif

    return (retval);
}

/*
 * accept a connection on a socket
 *
 * (this call blocks if there are now connections waiting)
 *
 */
int AcceptSocket(int sockfd)
{
    int		newsockfd, client_len;
    struct	sockaddr_in client_addr;

    client_len = sizeof(client_addr);

#ifdef DEBUG
    fprintf(stderr,"%s : AcceptSocket(%d) waiting...",__FILE__,sockfd);
#endif

    newsockfd = accept(sockfd, (struct sockaddr *) &client_addr, (socklen_t *) &client_len);

#ifdef DEBUG
    fprintf(stderr,"AcceptSocket(%d) returning %d\n",sockfd,newsockfd);

    if (newsockfd < 0) {
        fprintf(stderr,"ERROR : %s : socket accept(%d) failed. errno = %d\n",
		__FILE__, sockfd, errno);
    }
#endif

    return (newsockfd);
}

/*
 * listen to a socket for up to maxq connections
 *
 */
int ListenSocket(int sockfd, int maxq)
{
    int		retval;

#ifdef DEBUG
    fprintf(stderr,"%s : ListenSocket(%d, %d) waiting...",__FILE__,sockfd,retval);
#endif

    retval = listen(sockfd, maxq);

#ifdef DEBUG
    fprintf(stderr,"ListenSocket(%d, %d) returning %d\n",sockfd,maxq,retval);
 
    if (retval < 0) {
        fprintf(stderr,"ERROR : %s : socket listen(%d, %d) failed. errno = %d\n",
		__FILE__, sockfd, maxq, errno);
    }
#endif

    return (retval);
}


/*
 * connect to a socket at a host and port
 */
int ConnectSocket(int sockfd, char *host_name, int port)
{
    int			retval;
    struct hostent	*host;
    struct sockaddr_in	host_addr;

#ifdef DEBUG
    fprintf(stderr,"%s : ConnectSocket(%d, %s, %d) getting host by name...",__FILE__,sockfd,host_name,port);
#endif

    host = gethostbyname(host_name);
    if (host == NULL) {
#ifdef DEBUG
	fprintf(stderr,"ERROR : %s : host name [%s] does not exist.\n",
		__FILE__, host_name);
#endif
	return (-1);
    }

#ifdef DEBUG
    fprintf(stderr,"success!\n");
#endif

    bzero((char *) &host_addr, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    bcopy((char *) host->h_addr, (char *) &host_addr.sin_addr.s_addr, host->h_length);
    host_addr.sin_port = htons(port);

#ifdef DEBUG
    fprintf(stderr,"%s : ConnectSocket() connecting...",__FILE__);
#endif

    retval = connect(sockfd, (struct sockaddr *) &host_addr, sizeof(host_addr));

#ifdef DEBUG
    if (retval < 0) {
        fprintf(stderr,"ERROR : %s : socket connect(%d, %s, %d) failed. errno = %d\n",
		__FILE__, sockfd, host_name, port, errno);
    } else {
        fprintf(stderr,"success! returning (%d)\n",retval);
    }
#endif

    return (retval);
}

/*
 * reads at most buffer_sz bytes from sockfd, fills buffer
 */
int ReadSocket(int sockfd, char *buffer, int buffer_sz)
{
    int n;

#ifdef DEBUG
    fprintf(stderr,"%s : ReadSocket(%d, 0x%08x, %d) reading...",__FILE__, sockfd, buffer, buffer_sz);
#endif

    n = read(sockfd, buffer, buffer_sz);

#ifdef DEBUG
    if (n < 0) {
        fprintf(stderr,"ERROR : %s : socket read(%d, 0x%08x, %d) failed. errno = %d\n",
		__FILE__, sockfd, buffer, buffer_sz, errno);
    } else {
        fprintf(stderr,"success! read %d bytes\n",n);
    }
#endif

    return (n);
}

/*
 * writes buffer_sz bytes from buffer to sockfd
 *
 */
int WriteSocket(int sockfd, char *buffer, int buffer_sz)
{
    int n;

#ifdef DEBUG
    fprintf(stderr,"%s : WriteSocket(%d, 0x%08x, %d) writing...",__FILE__, sockfd, buffer, buffer_sz);
#endif

    n = write(sockfd, buffer, buffer_sz);

#ifdef DEBUG
    if (n < 0) {
        fprintf(stderr,"ERROR : %s : socket write(%d, 0x%08x, %d) failed. errno = %d\n",
		__FILE__, sockfd, buffer, buffer_sz, errno);
    } else {
        fprintf(stderr,"success! wrote %d bytes\n",n);
    }
#endif

    return (n);
}


