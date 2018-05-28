
#
# Makefile for Steve's simple socket library
#
# (c) Copyright 2012, Steve Anderson
#

LIB_OBJ =	ssocklib.o
TEST_OBJ =	server.o client.o

TARGET = libssock.a
TEST_PROGRAMS =	server client

CC =	gcc
#CC =	cc

LINT_FLAGS =
LINT_FLAGS += -ansi -pedantic -w -Wall -Wextra -Wunused -Wundef
LINT_FLAGS += -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations
LINT_FLAGS += -Wno-deprecated -Wuninitialized -Wtraditional

CFLAGS =	-O2 -Wall
# enable this to turn on more diagnostics and error messages in the library
#CFLAGS +=	-DDEBUG

LDFLAGS =

#LIBS = libssock.a

.c.o:
	$(CC) $(CFLAGS) -c $<

all:	$(TARGET) $(TEST_PROGRAMS)

$(TARGET):	$(LIB_OBJ)
	ar cr $(TARGET) $(LIB_OBJ)

$(TEST_PROGRAMS):	$(TEST_OBJ)

client:		client.o $(TARGET)
	$(CC) client.o $(LDFLAGS) $(TARGET) -o $@

server:		server.o $(TARGET)
	$(CC) server.o $(LDFLAGS) $(TARGET) -o $@

clean:
	/bin/rm -f $(TARGET) $(TEST_PROGRAMS) $(LIB_OBJ) $(TEST_OBJ) 


