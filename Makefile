# Define C compiler & flags
CC = gcc
CFLAGS = -Wall -g
# Define libraries to be linked (for example -lm)
LDLIBS = -lm 

# Define sets of source files and object files
SRC = allocate.c sjf.c memory.c rr.c
# OBJ is the same as SRC, just replace .c with .o
OBJ = $(SRC:.c=.o)
 
# Define the executable name(s)
EXE = allocate
# The first target:
$(EXE): $(OBJ) 
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LDLIBS)

clean:
	rm -f $(OBJ) $(EXE)