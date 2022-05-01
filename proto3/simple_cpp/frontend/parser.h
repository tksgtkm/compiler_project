#ifndef __PARSER_H
#define __PARSER_H

#include <set>

#include "scanner.h"
#include "token.h"
#include "../intermediate/symtab.h"
#include "../intermediate/node.h"

namespace frontend {
  using namespace intermediate;

  class Parser {
  private:
    Scanner *scanner;
    Symtab *symtab;
    Token *currentToken;
    int lineNumber;
    int errorCount;

    static std::set<TokenType> statementStarters;
    static std::set<TokenType> statementFollowers;
    static std::set<TokenType> relationalOperators;
    static std::set<TokenType> simpleExpressionOperators;
    static std::set<TokenType> termOperators;

  public:
    static void initialize();

    Parser(Scanner *scanner, Symtab *symtab)
      : scanner(scanner), symtab(symtab), currentToken(nullptr),
        lineNumber(1), errorCount(0) {}

    int getErrorCount() const {return errorCount;}

    Node *parseProgram();

  private:
    Node *parseStatement();
    Node *parseAssignmentStatement();
    Node *parseCompoundStatement();
    Node *parseRepeatStatement();
    Node *parseWriteStatement();
    Node *parseWritelnStatement();
    Node *parseExpression();
    Node *parseSimpleExpression();
    Node *parseTerm();
    Node *parseFactor();
    Node *parseVariable();
    Node *parseIntegerConstant();
    Node *parseRealConstant();
    Node *parseStringConstant();

    void parseStatementList(Node *parseNode, TokenType terminalType);
    void parseWriteArguments(Node *node);

    void syntaxError(std::string message);
    void semanticError(std::string message);
  };
} // namespace frontend

#endif
