CC = gcc 
INCLUDE = -I ./src
RM = rm -r 
CFLAGS = $(INCLUDE) -std=gnu11 -Werror 
SRCFILES = src/input_stream.c src/prim.c src/calc_error.c src/dict.c src/term.c src/expr.c
MAINFILE = main.c
LIBS = -lm 
LIBFLAGS = -L/usr/lib 
OBJ = $(subst .c,.o,$(SRCFILES))
DIR = ./release 
SRCDIR = ./src 
EXEC = calc 
AR = ar -rcs
LIBNAME = libcalc.a 

.PHONY: all clean install

all:$(EXEC)

$(EXEC):$(OBJ) $(MAINFILE)
	$(CC) -o $@ $^ $(CFLAGS)  $(LIBFLAGS) $(LIBS)
	
$(OBJ):%.o:%.c
	$(CC) -c $^ -o $@ $(CFLAGS) 

main.o:$(MAINFILE)
	$(CC) -c $^ -o $@ $(CFLAGS)
	
install:
	$(AR) -o $(LIBNAME) $(OBJ)
	mkdir -p ./release/include 
	mkdir -p ./release/lib
	cp ./calc ./release/calc 
	mv $(LIBNAME) ./release/lib
	cp ./src/*.h ./release/include 
		
clean:
	$(RM) *.o
	



