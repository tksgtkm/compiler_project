COMPILER = proto
SRCS = Pascal.cc \
       wci/Globals.cc \
			 wci/backend/Backend.cc \
			 wci/backend/compiler/CodeGenerator.cc \
			 wci/backend/interpreter/Executor.cc \
			 wci/frontend/Parser.cc \
			 wci/frontend/Source.cc \
			 wci/frontend/pascal/PascalParserTd.cc \
			 wci/frontend/pascal/PascalScanner.cc \
			 wci/message/MessageHandler.cc

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
