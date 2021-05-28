CC = gcc
CFLAGS = -D_REENTRANT
CLIBS = -pthread
CMDS  = serv clnt

all : $(CMDS)

serv : serv.c
	$(CC) $(CFLAGS) $^ -o $@ $(CLIBS)

clnt : clnt.c
	$(CC) $(CFLAGS) $^ -o $@ $(CLIBS)

clean :
	rm $(CMDS) core 

