#ifndef __PASCAL_H
#define __PASCAL_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "wci/object.h"
#include "wci/frontend/parser.h"
#include "wci/frontend/scanner.h"
#include "wci/frontend/source.h"
#include "wci/intermediate/symtab.h"
#include "wci/intermediate/icode.h"
#include "wci/backend/backend.h"
#include "wci/message/message.h"
#include "wci/message/message_listener.h"

using namespace wci::frontend;
using namespace wci::intermediate;
using namespace wci::backend;
using namespace wci::message;

class Pascal : public MessageListener {
public:
  Pascal(std::string operation, std::string file_path, std::string flags) throw (std::string);

  virtual ~Pascal();

  void message_received(Message& message);

private:
  Parser  *parser;
  Source  *source;
  ICode   *icode;
  SymTab  *symtab;
  Backend *backend;

  static const std::string SOURCE_LINE_FORMAT;
  static const std::string PARSER_SUMMARY_FORMAT;
  static const std::string INTERPRETER_SUMMARY_FORMAT;
  static const std::string COMPILER_SUMMARY_FORMAT;

  static const int PREFIX_WIDTH;

  std::string commafy(const int value) const;
};

#endif
