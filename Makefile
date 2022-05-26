UNAME := $(shell uname -s)

CC=clang++
CFLAGS=-c -std=c++11 -fPIC -march=native -O3 -Wall
LDFLAGS=-shared -Wl
LIBRARIES=-lz
INCLUDES=-I$(JAVA_HOME)/include \
	 -I ./sparser 

ifeq ($(UNAME),Linux)
  INCLUDES += -I$(JAVA_HOME)/include/linux
  TARGET=libs/libsparser.so
  LINK=/usr/lib/libsparser.so
else ifeq ($(UNAME),Darwin)
  JAVA_HOME=$(shell /usr/libexec/java_home)
  INCLUDES += -I$(JAVA_HOME)/include/darwin
  TARGET=libs/libsparser.dylib
  LINK=/Library/Java/Extensions/libsparser.dylib
endif

.PHONY: all clean

all: $(TARGET)

$(TARGET): com_meylism_sparser_Sparser.o libs link
	$(CC) $(INCLUDES) -o $(TARGET) $(LIBRARIES) $(LDFLAGS) com_meylism_sparser_Sparser.o

com_meylism_sparser_Sparser.o:
	$(CC) $(CFLAGS) $(INCLUDES) com_meylism_sparser_Sparser.cpp

libs:
	mkdir libs

link:
	sudo ln -sfn $(shell pwd)/$(TARGET) $(LINK)

clean:
	rm -rf *.o libs