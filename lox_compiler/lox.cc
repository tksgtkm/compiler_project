#include <cstring>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "error.h"

void runFile(std::string_view path);

void run(std::string_view source) {

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

int main(int argc, char *argv[]) {
  if (argc > 2) {
    std::cout << "Usage: jlox [script]\n";
    std::exit(64);
  }  else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}