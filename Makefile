CC= gcc 
CFLAGS = -Wall -g

TARGET= main

OBJS = main.o server.o request/request.o response/response.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c server.h request/request.h response/response.h
	$(CC) $(CFLAGS) -c main.c


server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

request/request.o: request/request.c request/request.h
	$(CC) $(CFLAGS) -c request/request.c -o request/request.o


response/response.o: response/response.c response/response.h
	$(CC) $(CFLAGS) -c response/response.c -o response/response.o

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
