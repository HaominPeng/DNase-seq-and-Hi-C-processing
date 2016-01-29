CC = gcc
CPP = g++
COMPILEC = ${CC} -c -s -O2
COMPILECPP = ${CPP} -g -rdynamic -c -s -O2
MAKEEXE = $(CPP) -s -O2 -lm -lpthread

OBJ = calculatereadnumber.o \
hic.o \
main.o \

EXE = samprocess

all: $(EXE)

$(EXE): $(OBJ)
	$(MAKEEXE) $^ -o $@

%.o: %.cpp
	$(COMPILECPP) $< -o $@

%.o: %.c
	$(COMPILEC) $< -o $@

clean:
	rm -f *.o $(EXE)
