CC=gcc
CFLAGS=-c -Wall -g
LDFLAGS=
SOURCES=x86ControlFlowGraph.c graph.c ../C-resources/ZpnString.c ../C-resources/ZpnFiles.c ../C-resources/ZpnError.c ../C-resources/ZpnRegex.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=x86CFG_builder

all: $(SOURCES) $(EXECUTABLE)
    
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)
	rm *.o ../C-resources/*.o

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -R x86CFG_builder
