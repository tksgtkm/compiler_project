#ifndef __BUFFER_H
#define __BUFFER_H

#include <fstream>
#include <cstdio>
#include <string>

#include "misc.h"
#include "error.h"

std::string eofChar;
int inputPosition;
int listFlag;
int level;

int maxInputBufferSize = 256;

class TTextInBuffer {
protected:
  std::fstream file;
  std::string *const pFileName;
  std::string text[maxInputBufferSize];
  std::string *pChar;

  virtual std::string GetLine(void) = 0;
}
