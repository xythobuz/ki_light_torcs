CC            =  g++
CPPFLAGS      = -Wall -g
SDL	      = $(shell sdl-config --libs)

# Uncomment the following line for a verbose client
#CPPFLAGS      = -Wall -g -D __UDP_CLIENT_VERBOSE__

#Put here the name of your driver class
DRIVER_CLASS = SimpleDriver
#Put here the filename of your driver class header 
DRIVER_INCLUDE = '"$(DRIVER_CLASS).h"' 
DRIVER_OBJ = $(DRIVER_CLASS).o

EXTFLAGS = -D __DRIVER_CLASS__=$(DRIVER_CLASS) -D __DRIVER_INCLUDE__=$(DRIVER_INCLUDE)
EXTFLAGS += $(shell sdl-config --cflags) -I ann_1.1.2/include

OBJECTS = WrapperBaseDriver.o SimpleParser.o CarState.o CarControl.o keyboard.o controller.o $(DRIVER_OBJ)

all: $(OBJECTS) client


.SUFFIXES : .o .cpp .c

.cpp.o :
	$(CC) $(CPPFLAGS) $(EXTFLAGS) -c $<

.c.o :
	$(CC) $(CPPFLAGS) $(EXTFLAGS) -c $<


client: client.cpp $(OBJECTS) ann
		$(CC) $(CPPFLAGS) $(EXTFLAGS) $(SDL) -o client client.cpp $(OBJECTS) -L ann_1.1.2/lib -l ANN

clean:
	rm -f *.o client  

ann:
	make -C ann_1.1.2 linux-g++
