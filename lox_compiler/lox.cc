#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "ast_printer.h"
#include "error.h"
#include "parser.h"
#include "scanner.h"
#include "interpreter.h"

std::string readFile(std::string_view path) {
  std::ifstream file{path.data(), std::ios::in | std::ios::binary | std::ios::ate};
  if (!file) {
    std::cerr << "Failed to open file " << path << ": " << std::strerror(errno) << "\n";
    std::exit(74);
  }

  std::string contents;
  contents.resize(file.tellg());

  file.seekg(0, std::ios::beg);
  file.read(contents.data(), contents.size());

  return contents;
}

Interpreter interpreter{};

void run(std::string_view source) {
  Scanner scanner{source};
  std::vector<Token> tokens = scanner.scanTokens();
  Parser parser{tokens};
  std::shared_ptr<Expr> expression = parser.parse();

  if (hadError)
    return;

  interpreter.interpret(expression);

  // std::cout << AstPrinter{}.print(expression) << "\n";
}

void runFile(std::string_view path) {
  std::string contents = readFile(path);
  run(contents);

  if (hadError)
    std::exit(65);
}

void runPrompt() {
  for (;;) {
    std::cout << "> ";
    std::string line;
    if (!std::getline(std::cin, line))
      break;
    run(line);
    hadError = false;  
  }
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    std::cout << "Usage: jlox [script]\n";
    std::exit(64);
  }  else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}