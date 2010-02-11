CC=gcc
CFLAGS=-c -Wall -std=gnu99 -D_GNU_SOURCE
LDFLAGS=-lmpdclient
SOURCES=ncmpstatus.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ncmpstatus

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ncmpstatus ncmpstatus.o

