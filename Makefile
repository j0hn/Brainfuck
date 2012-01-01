TARGET=brainfuck
CC=gcc
CFLAGS+= -ansi -Werror -Wall -Wbad-function-cast -Wcast-qual -Wstrict-prototypes -Wno-unused-parameter -g

SOURCES=$(shell echo src/*.c)
OBJECTS=$(SOURCES:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $^ -o $@

clean:
	rm -f $(TARGET) $(OBJECTS) .depend
	
test: $(TARGET)
	cd tests && ./run_tests.sh

.depend: *.[ch]
	$(CC) -MM $(SOURCES) >.depend

-include .depend

.PHONY: clean all test
