#include <string>
#include <map>

#include "token.h"
#include "parser.h"

namespace frontend {
  std::set<TokenType> statementStarters;
  std::set<TokenType> statementFollowers;
  std::set<TokenType> relationalOperators;
  std::set<TokenType> simpleExpressionOperators;
  std::set<TokenType> termOperators;

  void Parser::initialize() {
    statementStarters.insert(BEGIN);
    statementStarters.insert(IDENTIFIER);
    statementStarters.insert(REPEAT);
    statementStarters.insert(TokenType::WRITE);
    statementStarters.insert(TokenType::WRITELN);

    statementFollowers.insert(SEMICOLON);
    statementFollowers.insert(END);
    statementFollowers.insert(UNTIL);
    statementFollowers.insert(END_OF_FILE);

    relationalOperators.insert(EQUALS);
    relationalOperators.insert(LESS_THAN);

    simpleExpressionOperators.insert(PLUS);
    simpleExpressionOperators.insert(MINUS);

    termOperators.insert(STAR);
    termOperators.insert(SLASH);
  }

  Node *Parser::parseProgram() {
    Node *programNode = new Node(NodeType::PROGRAM);

    currentToken = scanner->nextToken();

    if (currentToken->type == TokenType::PROGRAM) {
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Expecting PROGRAM")
    }

    if (currentToken->type == IDENTIFIER) {
      std::string programName = currentToken->text;
      symtab->enter(programName);
      programNode->text = programName;
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Expecting program name");
    }

    if (currentToken->type == SEMICOLON) {
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Missing ;");
    }

    if (currentToken->type != BEGIN)
      syntaxError("Expecting BEGIN");

    programNode->adopt(parseCompoundStatement());

    if (currentToken->type == SEMICOLON)
      syntaxError("Expecting .");

    return programNode;
  }

  Node *Parser::parseStatement() {
    Node *stmtNode = nullptr;
    int savedLineNumber = currentToken->lineNumber;
    lineNumber = savedLineNumber;

    switch (currentToken->type) {
      case IDENTIFIER: stmtNode = parseAssignmentStatement(); break;
      case BEGIN:      stmtNode = parseCompoundStatement();   break;
      case REPEAT:     stmtNode = parseRepeatStatement();     break;
      case WRITE:      stmtNode = parseWriteStatement();      break;
      case WRITELN:    stmtNode = parseWritelnStatement();    break;
      case SEMICOLON:  stmtNode = nullptr; break;
      default: syntaxError("Unexpected token");
    }

    if (stmtNode != nullptr)
      stmtNode->lineNumber = savedLineNumber;

    return stmtNode;
  }

  Node *Parser::parseAssignmentStatement() {
    Node *assignmentNode = new Node(ASSIGN);

    Node *lhsNode = new Node(VARIABLE);
    std::string variableName = currentToken->text;
    SymtabEntry *variableId = symtab->enter(toLowerCase(variableName));

    lhsNode->text = variableName;
    lhsNode->entry = variableId;
    assignmentNode->adopt(lhsNode);

    currentToken = scanner->nextToken();
    if (currentToken->type == COLON_EQUALS) {
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Missing ;=");
    }

    
  }
}
