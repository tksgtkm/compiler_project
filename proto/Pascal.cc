#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Pascal.h"
#include "wci/frontend/Parser.h"
#include "wci/frontend/Scanner.h"
#include "wci/frontend/Source.h"
#include "wci/frontend/FrontendFactory.h"
#include "wci/intermediate/Symtab.h"
#include "wci/intermediate/Icode.h"
#include "wci/backend/Backend.h"
#include "wci/backend/BackendFactory.h"
#include "wci/message/Message.h"
#include "wci/message/MessageListener.h"

using namespace wci::frontend;
using namespace wci::intermediate;
using namespace wci::backend;
using namespace wci::message;

const std::string FLAGS = "[-ix]";
const std::string USAGE = "Usage: Pascal execute|compile " + FLAGS + " <source file path> ";

int main(int argc, char *args[]) {
  try {
    std::string operation = args[1];
    if ((operation != "compile") && (operation != "execute")) {
      throw USAGE;
    }

    std::string flags = "";
    int i = 1;
    while ((++i < argc) && (args[i][0] == '-')) {
      flags += std::string(args[i]).substr(1);
    }

    if (i < argc) {
      std::string path = args[i];
      Pascal(operation, path, flags);
    } else {
      throw std::string ("Missing source file.");
    }
  } catch (std::string& msg) {
    std::cout << "***** ERROR: " << msg << std::endl;
  }

  return 0;
}

Pascal::Pascal(std::string operation, std::string file_path, std::string flags) throw (std::string) : icode(nullptr), symtab(nullptr), backend(nullptr) {
  std::ifstream input;
  input.open(file_path);
  if (input.fail()) {
    throw std::string("Failed to open source file " + file_path);
  }

  Options::intermediate = flags.find('i') != std::string::npos;
  Options::xref         = flags.find('x') != std::string::npos;

  source = new Source(input);
  source->add_message_listener(this);

  parser = FrontendFactory::create_parser("Pascal", "top-down", source);
  parser->add_message_listener(this);
  parser->parse();

  source->close();

  symtab = parser->get_symtab();
  icode = parser->get_icode();

  backend = BackendFactory::create_backend(operation);
  backend->add_message_listener(this);
  backend->process(icode, symtab);
}

Pascal::~Pascal() {
  if (parser  != nullptr) delete parser;
  if (source  != nullptr) delete source;
  if (icode   != nullptr) delete icode;
  if (symtab  != nullptr) delete symtab;
  if (backend != nullptr) delete backend;
}

const std::string Pascal::SOURCE_LINE_FORMAT = "%03d %s\n";

const std::string Pascal::PARSER_SUMMARY_FORMAT =
  std::string("\n%20s source lines.\n%20d syntax errors.\n") +
  std::string("%20.2f seconds total parsing time.\n");

const std::string Pascal::INTERPRETER_SUMMARY_FORMAT =
  std::string("\n%20s statements executed.\n") +
  std::string("%20d runtime errors.\n") +
  std::string("%20.0f seconds total execution time.\n");

const std::string Pascal::COMPILER_SUMMARY_FORMAT =
  std::string("\n%20s instructions generated.\n") +
  std::string("%20.2f seconds total code generation time.\n");

void Pascal::message_received(Message& message) {
  switch (message.type) {
    case SOURCE_LINE: {
      printf(SOURCE_LINE_FORMAT.c_str(), message.content.source_line.line_number,
             message.content.source_line.line_text);
      break;
    }
    case PARSER_SUMMARY: {
      std::string line_count = commafy(message.content.parser_summary.line_count);
      int error_count = message.content.parser_summary.error_count;
      double elapsed_time = message.content.parser_summary.elapsed_time;
      printf(PARSER_SUMMARY_FORMAT.c_str(), line_count.c_str(), error_count, elapsed_time);
      break;
    }
    case INTERPRETER_SUMMARY: {
      std::string execution_count = commafy(message.content.interpreter_summary.execution_count);
      printf(INTERPRETER_SUMMARY_FORMAT.c_str(), execution_count.c_str(),
             message.content.interpreter_summary.runtime_errors,
             message.content.interpreter_summary.elapsed_time);
      break;
    }
    case COMPILER_SUMMARY: {
      std::string instruction_count = commafy(message.content.compiler_summary.instruction_count);
      printf(COMPILER_SUMMARY_FORMAT.c_str(), instruction_count.c_str(),
             message.content.compiler_summary.elapsed_time);
      break;
    }
    default: break;
  }
}

std::string Pascal::commafy(const int value) const {
  std::string str = std::to_string(value);
  int pos = str.length() - 3;

  while (pos > 0) {
    str.insert(pos, ",");
    pos -= 3;
  }

  return str;
}
