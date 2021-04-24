# Makefile que facilmente se adapta a qualquer tipo de exercicio
# author Vitor Carreira
# date 2009-09-23
#
CC=cc
# Bibliotecas a incluir
LIBS=-lX11 -lpng -lz -lpthread -ljson-c -ljpeg

# Flags para o compilador
CFLAGS=-Wall -W -g -Wmissing-prototypes 

# Flags para a indentacao de codigo 
IFLAGS=-br -brs -npsl -ce -cli4

DIR=/home/micron/sav/Trabalhos/xertin
# nome do executavel 
PROGRAM=xertin

# Nome do ficheiro de opcoes do gengetopt (caso exista)
PROGRAM_OPT=args
FOLDERS=decoders 
EVERYFILE= $(patsubst %, %/*.c, $(FOLDERS))
PROGRAM_OBJS=main.o funcaux.o pngdec.o jpegdec.o bmpdec.o gifdec.o grapdec.o gfx.o config.o ${PROGRAM_OPT}.o
PROGRAM_OBJSDIR=$(patsubst %, obj/%, $(PROGRAM_OBJS))
.PHONY: clean

all: ${PROGRAM}

# compilar com depuracao
depuracao: CFLAGS += -D SHOW_DEBUG 
depuracao: ${PROGRAM}

debug: 
	gdb -se xertin -d decoders
#program_objsdir:echo "${PROGRAM_OBJS}" || sed "s/m/ m/" || sed "s/ / obj\//g"
piramideDeDependencias:  
	${CC} -MM *.c ${EVERYFILE} 

${PROGRAM}: ${PROGRAM_OBJS}
	${CC} ${CFLAGS}-o $@ ${PROGRAM_OBJSDIR} ${LIBS}

# Dependencias 
${PROGRAM_OPT}.o: ${PROGRAM_OPT}.c ${PROGRAM_OPT}.h
config.o: config.c config.h funcaux.h
funcaux.o: funcaux.c funcaux.h
gfx.o: gfx.c gfx.h config.h funcaux.h
grapdec.o: grapdec.c grapdec.h gfx.h config.h funcaux.h
main.o: main.c decoders/pngdec.h funcaux.h ${PROGRAM_OPT}.h config.h funcaux.h decoders/jpegdec.h decoders/gifdec.h grapdec.h gfx.h config.h

bmpdec.o: decoders/bmpdec.o
decoders/bmpdec.o: decoders/bmpdec.c decoders/bmpdec.h ${PROGRAM_OPT}.h config.h funcaux.h funcaux.h

gifdec.o: decoders/gifdec.o
decoders/gifdec.o: decoders/gifdec.c decoders/gifdec.h ${PROGRAM_OPT}.h funcaux.h config.h funcaux.h

jpegdec.o: decoders/jpegdec.o
decoders/jpegdec.o: decoders/jpegdec.c decoders/jpegdec.h ${PROGRAM_OPT}.h funcaux.h config.h funcaux.h

pngdec.o: decoders/pngdec.o
decoders/pngdec.o: decoders/pngdec.c decoders/pngdec.h funcaux.h ${PROGRAM_OPT}.h config.h funcaux.h

%.o : %.c
	${CC} ${CFLAGS}-c $*.c -o obj/$(patsubst decoders/%,%,$@)
	
	
${PROGRAM_OPT}.h: ${PROGRAM_OPT}.ggo
	gengetopt < ${PROGRAM_OPT}.ggo --file-name=${PROGRAM_OPT}

clean:
	rm -rf obj/*.o coredump/*.coredump *~ ${PROGRAM} *.bak ${PROGRAM_OPT}.h ${PROGRAM_OPT}.c

docs: Doxyfile
	doxygen Doxyfile

Doxyfile:
	doxygen -g Doxyfile

nomedofich :=  Xertin_$(shell date +d%dm%my%y-%Hh%Mm%Ss).coredump
codedump:
	coredumpctl -o ${nomedofich} dump ${DIR}/xertin
	mv ${nomedofich} ${DIR}/coredump
	gdb ${PROGRAM} coredump/${nomedofich}

