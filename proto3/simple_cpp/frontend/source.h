#ifndef __SOURCE_H
#define __SOURCE_H

#include <iostream>
#include <fstream>
#include <string>

namespace frontend {
  class Source {

    privete:
      std::ifstream source;
      std::string sourceFileName;
      int lineNum;
      char currentCh;

    public:
      static const char EOL = '\n';

      Source (std::string sourceFileName) : lineNum(1) {
        source.open(sourceFileName);

        if (source.fail()) {
          std::cout << "*** ERROR: Failed to open " << sourceFileName << std::endl;
          exit(-1);
        }
        currentCh = nextChar();
      }

      int lineNumber() const {return lineNum;}

      char currentChar() const {return currentCh;}

      char nextChar() {
        currentCh = source.get();

        if (source.eof()) {
          currentCh = EOF;
        } else if (currentCh == EOL) {
          lineNum++;
        } else if (source.fail()) {
          std::cout << "*** ERROR: Failed to read " << sourceFileName << std::endl;
          exit(-1);
        }

        return currentCh;
      }
  };
} // namespace frontend

#endif
