COMPILER = proto
SRCS = pascal.cc \
       wci/globals.cc \
			 wci/backend/backend.cc \
			 wci/backend/compiler/code_generator.cc \
			 wci/backend/interpreter/executor.cc \
			 wci/frontend/parser.cc \
			 wci/frontend/source.cc \
			 wci/frontend/pascal/pascal_parser_td.cc \
			 wci/frontend/pascal/pascal_scanner.cc \
			 wci/message/message_handler.cc

BOOST_INCLUDER_DIR = /usr/local/include/boost

HEADER_DEPENDENCIES = header_dependencies.txt

OBJS = $(SRCS:.cc=.o)

CC = g++
CFLAGS = -std=c++0x -O0 -g3 -Wall -Wno-sign-compare -Wno-deprecated -I$(BOOST_INCLUDER_DIR)
.cc.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: $(COMPILER)

clean:
	rm $(OBJS) $(COMPILER)

$(COMPILER) : $(OBJS)
	$(CC) -o $(COMPILER) $(OBJS)

execute: $(COMPILER) $(src)
	./$(COMPILER) compile -$(flags) $(src)

dependencies: depends
	$(CC) -MM $(SRCS) > raw_dpendencies.txt
	./depends < raw_dependencies.txt > $(HEADER_DEPENDENCIES)
	rm raw_dpendencies.txt

depends: depends.hidden
	cp depends.hidden depends.cc
	g++ -o depends.cc
	rm depends.cc

sinclude $(HEADER_DEPENDENCIES)
