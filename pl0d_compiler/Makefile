CC	= gcc
CFLAGS	=
LFLAGS	=

OBJS	= codegen.o \
	  compile.o \
	  get_source.o \
	  main.o \
	  table.o

.SUFFIXES	: .o .c

.c.o	: 
	$(CC) $(CFLAGS) -c $<

pl0d	: ${OBJS}
	$(CC) -o $@ ${OBJS}

clean	:
	\rm -rf *~ *.o

