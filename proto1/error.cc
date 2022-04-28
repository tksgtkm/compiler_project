#include <iostream>
#include <cstdlib>

#include "error.h"

int errorCount = 0;

static char *abortMsg[] = {
  NULL,
  "Invalid command line arguments",
  "Failed to open source file",
  "Failed to open intermediate form file",
  "Failed to open assembly file",
  "Too many syntax errors",
  "Stack overflow",
  "Code segment overflow",
  "Nesting too deep",
  "Runtime error",
  "Unimplemented feature",
};

void AbortTranslation(TAbortCode ac) {
  std::cerr << "Fatal translator error: " << abortMsg[-ac] << std::endl;
  std::exit(ac);
}
