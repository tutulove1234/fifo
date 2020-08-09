#! Makefile

CC = g++
CXXFLAG = -std=c++1y

TARGET = fifo.out
SOURCE = $(shell find ./ -iname "*.cc")
SOURCE += $(shell find ./ -iname "*.cpp")

.PHONY: $(TARGET) clean all

all: 
	$(CC) -o $(TARGET) $(SOURCE) $(CXXFLAG)

clean:
	$(RM) -f *.o $(TARGET)

