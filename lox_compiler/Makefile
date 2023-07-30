CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := ast_printer_driver.cc generate_ast.cc lox.cc
DEPS     := $(SRCS:.cc=.d)


jlox: ast_printer lox.o
	@$(COMPILE) lox.o -o $@

ast_printer: expr.h ast_printer_driver.o
	@$(COMPILE) ast_printer_driver.o -o $@


generate_ast: generate_ast.o
	@$(COMPILE) $< -o $@


expr.h: generate_ast
	@./generate_ast .


.PHONY: clean
clean:
	rm -f *.d *.o ast_printer generate_ast jlox


-include $(DEPS)


.PHONY: test-lexing
test-lexing:
	@make jlox >/dev/null
	@echo "testing jlox with test-lexing.lox ..."
	@./jlox tests/test-lexing.lox | diff -u --color tests/test-lexing.lox.expected -;


.PHONY: test-lexing2
test-lexing2:
	@make jlox >/dev/null
	@echo "testing jlox with test-lexing2.lox ..."
	@./jlox tests/test-lexing2.lox | diff -u --color tests/test-lexing2.lox.expected -;

	.PHONY: test-parsing
test-parsing:
	@make jlox >/dev/null
	@echo "testing jlox with test-parsing.lox ..."
	@./jlox tests/test-parsing.lox | diff -u --color tests/test-parsing.lox.expected -;
