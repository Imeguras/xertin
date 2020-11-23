# Makefile que facilmente se adapta a qualquer tipo de exercicio
# author Vitor Carreira
# date 2009-09-23
#

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

PROGRAM_OBJS=main.o funcaux.o jpegdec.o pngdec.o grapdec.o gfx.o config.o ${PROGRAM_OPT}.o
PROGRAM_OBJSDIR=obj/main.o obj/funcaux.o obj/pngdec.o obj/jpegdec.o obj/grapdec.o obj/gfx.o obj/${PROGRAM_OPT}.o obj/config.o
.PHONY: clean

all: ${PROGRAM}

# compilar com depuracao
depuracao: CFLAGS += -D SHOW_DEBUG 
depuracao: ${PROGRAM}
#${PROGRAM_OBJS}: obj/%.o : %.c
#	${CC} ${CFLAGS} -c $< -o $@
${PROGRAM}: ${PROGRAM_OBJS}
	${CC} ${CFLAGS}-o $@ ${PROGRAM_OBJSDIR} ${LIBS}

# Dependencias 
main.o: main.c ${PROGRAM_OPT}.h pngdec.h grapdec.h config.h
${PROGRAM_OPT}.o: ${PROGRAM_OPT}.c ${PROGRAM_OPT}.h
funcaux.o: funcaux.c funcaux.h 
pngdec.o: pngdec.c pngdec.h funcaux.h config.h args.h
gifdec.o: gifdec.c gifdec.h funcaux.h config.h args.h
jpegdec.o: jpegdec.c jpegdec.h funcaux.h config.h args.h
grapdec.o: grapdec.c grapdec.h gfx.h config.h
gfx.o: gfx.c gfx.h
config.o: config.c config.h funcaux.h 


%.o : %.c
	${CC} ${CFLAGS} -c $< -o obj/$@

${PROGRAM_OPT}.h: ${PROGRAM_OPT}.ggo
	gengetopt < ${PROGRAM_OPT}.ggo --file-name=${PROGRAM_OPT}

clean:
	rm -f obj/*.o coredump/*.coredump *~ ${PROGRAM} *.bak ${PROGRAM_OPT}.h ${PROGRAM_OPT}.c

docs: Doxyfile
	doxygen Doxyfile

Doxyfile:
	doxygen -g Doxyfile

indent:
	indent ${IFLAGS} *.c *.h

nomedofich :=  Xertin_$(shell date +d%dm%my%y-%Hh%Mm%Ss).coredump
codedump:
	coredumpctl -o ${nomedofich} dump ${DIR}/xertin
	mv ${nomedofich} ${DIR}/coredump
	gdb ${PROGRAM} coredump/${nomedofich}