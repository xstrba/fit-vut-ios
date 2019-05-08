CFLAGS=-std=gnu99 -Wall -Wextra -Werror -pedantic
CC=gcc
LDLIBS= -lpthread
TARGET=proj2
SOURCES=$(shell ls *.c)
OBJECTS=$(SOURCES:.c=.o)
HEADERSTMP=$(SOURCES:.c=.h)
HEADERS=$(HEADERSTMP:proj2.h=)
DOCOUT=doc
PACK=proj2.zip

.PHONY: all clean doc run pack

all: $(TARGET)

$(TARGET): $(OBJECTS)

doc: Doxyfile
	doxygen $<
	rm -rf latex 

Doxyfile:
	doxygen -g

clean:
	rm -rf *.o $(DOCOUT) $(TARGET) *.out *.zip html latex

pack:
	zip -r $(PACK) *.c *.h Makefile

run:
	./proj2 6 1000 1000 2000 2000 5

