
#
# Makefile for Steve's simple socket library
#
# (c) Copyright 2012, Steve Anderson
#

OBJ =	ssocklib.o

TARGET = libssock.a

#CC =	gcc
CC =	cc

LINT_FLAGS =
LINT_FLAGS += -ansi -pedantic -w -Wall -Wextra -Wundef
LINT_FLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations
LINT_FLAGS += -Wno-deprecated

CFLAGS =	-g -Wall
CFLAGS +=	$(LINT_FLAGS)
# enable this to turn on more diagnostics and error messages in the library
#CFLAGS +=	-DDEBUG

LDFLAGS =

LIBS = libssock.a

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	server client

$(TARGET):	$(OBJ)
	ar cr $(TARGET) $(OBJ)

client:		client.o $(LIBS)
	$(CC) client.o $(LDFLAGS) $(LIBS) -o $@

server:		server.o $(LIBS)
	$(CC) server.o $(LDFLAGS) $(LIBS) -o $@

clean:
	/bin/rm -f $(TARGET) $(OBJ) client server client.o server.o


