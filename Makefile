program=strelba

HEAD=poc_uloha.h hledani_korenu.h
SRC=main.c poc_uloha.c hledani_korenu.c
OBJ=main.o poc_uloha.o hledani_korenu.o

# implicit switches for gcc
CFLAGS=-O1 -pipe -DDEBUG -Wall

# switches for linker
LDFLAGS=-lm

# compiler
CC=gcc
#CC=g++

# build, install, uninstall, clean a distrib do not have a file on their own
.PHONY: build
.PHONY: clean
.PHONY: distrib
.PHONY: distrib-git
.PHONY: tarball

# default target
build: ${program}

# cleans files form compilation
clean:
	rm -f ${OBJ} ${program}

${program}: ${OBJ}
	$(CC) $(LDFLAGS) ${OBJ} -o ${program}

# every object file depends on all header files
${OBJ}: ${HEAD}

# rest is done automagically by implicit rules
