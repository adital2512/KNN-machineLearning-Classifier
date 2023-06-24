OBJS	= ExtendedVector.o HandleClient.o HandleFile.o Kclosest.o calcs.o classifier.o server1.o client.o
OUT	= server.out,client.out

OBJS0	= ExtendedVector.o HandleClient.o HandleFile.o Kclosest.o calcs.o classifier.o server1.o
SOURCE0	= ExtendedVector.cpp HandleClient.cpp HandleFile.cpp Kclosest.cpp calcs.cpp classifier.cpp server1.cpp
HEADER0	= ExtendedVector.h HandleClient.h HandleFile.h Kclosest.h calcs.h classifier.h
OUT0	= server.out

OBJS1	= client.o
SOURCE1	= client.cpp
HEADER1	=
OUT1	= client.out

CC	 = g++
FLAGS	 = -g -c -Wall

all: server.out client.out

server.out: $(OBJS0)
	$(CC) -g $(OBJS0) -o $(OUT0) -lpthread

client.out: $(OBJS1)
	$(CC) -g $(OBJS1) -o $(OUT1) -lpthread

ExtendedVector.o: ExtendedVector.cpp
	$(CC) $(FLAGS) ExtendedVector.cpp -std=c++11

HandleClient.o: HandleClient.cpp
	$(CC) $(FLAGS) HandleClient.cpp -std=c++11

HandleFile.o: HandleFile.cpp
	$(CC) $(FLAGS) HandleFile.cpp -std=c++11

Kclosest.o: Kclosest.cpp
	$(CC) $(FLAGS) Kclosest.cpp -std=c++11

calcs.o: calcs.cpp
	$(CC) $(FLAGS) calcs.cpp -std=c++11

classifier.o: classifier.cpp
	$(CC) $(FLAGS) classifier.cpp -std=c++11

server1.o: server1.cpp
	$(CC) $(FLAGS) server1.cpp -std=c++11

client.o: client.cpp
	$(CC) $(FLAGS) client.cpp -std=c++11


clean:
	rm -f $(OBJS) $(OUT)
