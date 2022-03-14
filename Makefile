CLI_SRC = vcs.c vcs_socket.c util.c
DAE_SRC = daemon.c vcs_socket.c util.c
OBJ = *.o
CLI_EXE = vcs
DAE_EXE = vcs_daemon
CC  = gcc
LIB = -lpthread

linux:
	$(MAKE) -s make_cli
	$(MAKE) -s clean
	$(MAKE) -s make_daemon
	$(MAKE) -s clean

clean:
	rm *.o

make_cli:
	$(CC) -c $(CLI_SRC)
	$(CC) -o $(CLI_EXE) $(OBJ) $(LIB)
	echo [INFO]: Successfully build client!

make_daemon:
	$(CC) -c $(DAE_SRC)
	$(CC) -o $(DAE_EXE) $(OBJ) $(LIB)
	echo [INFO]: Successfully build daemon!
