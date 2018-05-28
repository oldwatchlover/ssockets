
/*
 *
 * ssocklib.h
 *
 *
 * Steve's Simple unix Socket Library
 *
 * The purpose of this simple library abstracts the "guts" of setting up and using sockets for simple
 * IPC.
 *
 * (c) Copyright 2012, Steve Anderson
 *
 */

/*
 * Simple tutorial for usage:
 *
 * It can be a little confusing to learn socket programming at first becuase there
 * are many include files and system data structures exposed, as well as many possible
 * uses of sockets  - this library attempts to simpify that. Further, there
 * is some overlap of thse calls (some calls are "server only", some "client only", and
 * some called by both - even more confusing if the socket is used for two-way
 * communication (also possible).
 *
 * GENERALLY, this library is designed for a simple client-server model.
 *
 * The server, is set up to wait for connections (from multiple processes or computers
 * on the network), and the client is an instance of one of those processes that wishes
 * to connect to the server.
 *
 * A client must know the host and port number to find the server. Generally,
 * ports in the range 0 - 4096 are used by the system and "known services" on the internet.
 * (for legacy reasons, this is not authoritative!). You should use a port > 5000
 * and less than 64K (65,534).  See /etc/services for unix ports used by the system.
 *
 * When launching, the server doesn't really need to know the hostname, it can be inferred from the 
 * computer running the server program (for clarity my test program requires it, however).
 *
 *
 * Here is psuedocode of how it all works: (error checking omitted for brevity)
 *
 *
 *  For the server:
 *
 *      The main loop for the server usually looks something like this:
 *
 *                fd = CreateSocket(); 
 *                BindSocket(fd, port);
 *
 *                while (1) { 
 *
 *                    ListenSocket(sockfd, maxq);
 *                    active_fd = AcceptSocket(fd);
 *
 *                    n = ReadSocket(active_fd, buffer, buffer_sz);
 *
 *                    close(active_fd);
 *
 *                }
 *
 *                close (fd);
 *
 *
 * Notice that in the above code there are 2 socket file descriptors... the first
 * is used to "listen" on the externally published port. Since multiple clients can
 * try to contact a server, they queue up (as many as maxq). The accept() call returns
 * a DIFFERNT file descriptor, the 'active' fd connecting to the client taken from the 
 * queue of waiting clients. THAT is the file descriptor the server uses to communicate with
 * that client.
 *
 * In the above server code, the "while (1)" looks ugly, but in practice it is not
 * so bad... the AcceptSocket() call will block waiting for connections (so this
 * loop is not resource-heavy or a busy wait condition eating up CPU cycles).
 *
 * Note that the ReadSocket() call can also block (it just wraps read() after all). 
 * This is not an issue, except if you wanted to do two way communication you
 * might prevent other clients from connecting while you block on I/O to the current
 * client... in practice, most real servers spawn threads to handle each connection so
 * the server can process more waiting connections... this is beyond the scope of this
 * example (although you could use this library to work that way with very little 
 * modification.
 *
 *
 * For the client:
 * 
 *      The main loop for the client usually looks something like this:
 *
 *                fd = CreateSocket(); 
 *                ConnectSocket(fd, host, port);
 *
 *                n = WriteSocket(fd, buffer, strlen(buffer));
 *
 *                CloseSocket(fd);
 *
 * Depending on what the client wants to do, it might excecute this fragment
 * many times sending (or receiving) data; or it might keep the connection open if it needs
 * frequent communication.
 *
 * That's about it!
 *
 * p.s. after adding the above tutorial and comments for each function, this .h file is now
 * ridiculously long... it really only declares a handful of library functions:
 *
 *        int CreateSocket(void);
 *        int CloseSocket(int fd);
 *        int BindSocket(int sockfd, int port);
 *        int AcceptSocket(int sockfd);
 *        int ListenSocket(int sockfd, int maxq);
 *        int ConnectSocket(int sockfd, char *serv_name, int port);
 *        int ReadSocket(int sockfd, char *buffer, int buffer_sz);
 *        int WriteSocket(int sockfd, char *buffer, int buffer_sz);
 *
 * Because I have simplified the use cases and hidden the kernel data structures and flags in the library,
 * there are not even any other #defines or variables...
 *
 * (see below for the detailed description of each function)
 *
 */


#ifndef __SSOCKLIB_H__
#define __SSOCKLIB_H__

/*
 * Create a socket.
 *
 * This is a wrapper around the unix socket() call.
 *
 * Returns -1 if it fails, errno remains set.
 *
 */
extern int CreateSocket(void);

/*
 * Close a socket.
 *
 * This is a wrapper around the unix close() call.
 *
 * Returns 0 if successful otherwise will return -1 and errno remains set.
 *
 */
extern int CloseSocket(int fd);

/*
 * Bind a name to a socket.
 *
 * This is a wrapper around the unix bind() call.
 *
 * This is called by the (logical) host, so only a port is needed.
 *
 * Returns 0 if successful, otherwise will return -1 and errno remains set.
 *
 */
extern int BindSocket(int sockfd, int port);

/*
 * Accept a connection on a socket.
 *
 * This is a wrapper around the unix accept() call.
 *
 * If successful, it returns a non-negative integer that is the file descriptor
 * for the socket. If it fails, it returns -1 and errno remains set.
 *
 */
extern int AcceptSocket(int sockfd);

/*
 * Listen for connections on a socket.
 *
 * This is a wrapper around the unix listen() call.
 *
 * The maxq parameter is the maximum backlog of the queue for pending connections.
 * If a connection request arrives with the queue full, the client may receive
 * an error indication of ECONNREFUSED (see errno).
 *
 */
extern int ListenSocket(int sockfd, int maxq);

/*
 * Initiate a connection on a socket.
 *
 * This is a wrapper around the unix connect() call.
 *
 * If successful, 0 is returned. Otherwise -1 is returned and errno remains set.
 *
 */
extern int ConnectSocket(int sockfd, char *serv_name, int port);

/*
 * Read from a socket
 *
 * A slightly socket specific wrapper around the unix read() call.
 *
 * Returns -1 if fail (and errno is set), else the number of bytes read.
 * For sockets, a return of 0 indicates the client closed the connection.
 *
 * Up to buffer_sz bytes are read into buffer.
 *
 */
extern int ReadSocket(int sockfd, char *buffer, int buffer_sz);

/*
 * Write to a socket
 *
 * A slightly socket specific wrapper around the unix write() call.
 *
 * Returns the number of bytes written, or -1 if fail (and errno is set)
 * Writes up to buffer_sz bytes from buffer to the socket sockfd.
 *
 */
extern int WriteSocket(int sockfd, char *buffer, int buffer_sz);

/*
 * Send a message to a socket
 *
 * A wrapper around native send() call, it ignores flags (sends 0x0)
 *
 */
extern int SendSocket(int sockfd, char *buffer, int buffer_sz);

/*
 * Receive a message on a socket
 *
 * A wrapper around native recv() call, it ignores flags (sends 0x0)
 *
 */
extern int RecvSocket(int sockfd, char *buffer, int buffer_sz);

#endif /* __SSOCKLIB_H__ */




