CXX      := g++
CXXFLAGS := -ggdb -std=c++17
CPPFLAGS := -MMD

COMPILE  := $(CXX) $(CXXFLAGS) $(CPPFLAGS)

SRCS     := ast_printer_driver.cc generate_ast.cc lox.cc
DEPS     := $(SRCS:.cc=.d)


jlox: expr.h stmt.h lox.o
	@$(COMPILE) lox.o -o $@

ast_printer: expr.h stmt.h ast_printer_driver.o
	@$(COMPILE) ast_printer_driver.o -o $@


generate_ast: generate_ast.o
	@$(COMPILE) $< -o $@


expr.h stmt.h: generate_ast
	@./generate_ast .


.PHONY: clean
clean:
	rm -f *.d *.o ast_printer generate_ast jlox


-include $(DEPS)


define make_test
.PHONY: $(1)
$(1):
	@make jlox >/dev/null
	@echo "testing jlox with $(1).lox ..."
	@./jlox tests/$(1).lox | diff -u --color tests/$(1).lox.expected -;
endef


TESTS = \
test-statements \
test-statements2 \
test-statements3 \
test-statements4 \
test-statements5 \
test-statements6 \


$(foreach test, $(TESTS), $(eval $(call make_test,$(test))))


.PHONY: test-all
test-all:
	@for test in $(TESTS); do \
		make -s $$test; \
	done

.PHONY: test-expressions2
test-expressions2:
	@make jlox >/dev/null
	@echo "testing jlox with test-expressions2.lox ..."
	@./jlox tests/test-expressions2.lox | diff -u --color tests/test-expressions2.lox.expected -;