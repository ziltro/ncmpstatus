CC=gcc
CFLAGS=-c -Wall -std=gnu99
LDFLAGS=-lmpdclient
SOURCES=ncmpstatus.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=ncmpstatus

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


#ncmpstatus:
#	gcc -Wall -lmpdclient ncmpstatus.c -o ncmpstatus
#
#all:
#	ncmpstatus
#
clean:
	rm -f ncmpstatus ncmpstatus.o

