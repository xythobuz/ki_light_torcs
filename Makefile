
# Configuration

CC       =  g++
CPPFLAGS = -Wall -g
SDL      = $(shell sdl-config --libs)
UNAME    = $(shell uname -s)
EXTFLAGS = $(shell sdl-config --cflags)
EXTFLAGS += -I ann_1.1.2/include

# Files

OBJECTS = build/CarControl.o
OBJECTS += build/CarState.o
OBJECTS += build/client.o
OBJECTS += build/controller.o
OBJECTS += build/keyboard.o
OBJECTS += build/SimpleDriver.o
OBJECTS += build/SimpleParser.o

HEADERS = src/BaseDriver.h
HEADERS += src/CarControl.h
HEADERS += src/CarState.h
HEADERS += src/controller.h
HEADERS += src/keyboard.h
HEADERS += src/SimpleDriver.h
HEADERS += src/SimpleParser.h

# Rules

all: intro client

run: intro client
	./client

intro:
	mkdir -p build

build/%.o : src/%.cpp $(HEADERS)
	$(CC) $(CPPFLAGS) $(EXTFLAGS) -c $< -o $@

client: ann $(OBJECTS)
	$(CC) $(CPPFLAGS) $(EXTFLAGS) $(SDL) -o client $(OBJECTS) -L ann_1.1.2/lib -l ANN

clean:
	rm -f client
	rm -rf build *.dSYM

annclean:
	@make -C ann_1.1.2 clean

ifeq ($(UNAME),Darwin)
ann:
	@make -C ann_1.1.2 macosx-g++
	@echo
	@echo "Built ANN library!"
	@echo "Compiling client..."
	@echo
else
ann:
	@make -C ann_1.1.2 linux-g++
	@echo
	@echo "Built ANN library!"
	@echo "Compiling client..."
	@echo
endif

