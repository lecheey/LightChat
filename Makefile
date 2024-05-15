CC = g++
SRC = Main.cpp
TARGET = lightchat

lightchat: $(SRC) lib
	$(CC) -o $(TARGET) $(SRC) -L. -lMyLib

lib: Chat.o Func.o Sha1.o
	ar rc libMyLib.a Chat.o Func.o Sha1.o

libchat: Chat.cpp
	$(CC) -o Chat.o Chat.cpp -c

libfunc: Func.cpp
	$(CC) -o Func.o Func.cpp -c

libsha1: Sha1.cpp
	$(CC) -o Sha1.o Sha1.cpp -c

clean:
	rm *.o *.a
