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

    Node *rhsNode = parseExpression();
    assignmentNode->adopt(rhsNode);

    return assignmentNode;
  }

  Node *Parse::parseCompoundStatement() {
    Node *compoundNode = new Node(COMPOUND);
    compondNode->lineNumber = currentToken->lineNumber;

    currentToken = scanner->nextToken();
    parseStatementList(compoundNode, END);

    if (currentToken->type == END) {
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Expecting END");
    }

    return compoundNode;
  }

  void Parser::parseStatementList(Node *parentNode, TokenType terminalType) {
    while ((currentToken->type != terminalTypr) && (currentToken->type != END_OF_FILE)) {
      Node *stmtNode = parseStatement();
      if (stmtNode != nullptr)
        parentNode->adopt(stmtNode);

      if (currentToken->type == SEMICOLON) {
        while (currentToken->type == SEMICOLON) {
          currentToken = scanner->nextToken();
        }
      } else if (statementStarters.find(currentToken->type) != statementStarters.end()) {
        syntaxError("Missing ;");
      }
    }
  }

  Node *Parser::parseRepeatStatement() {
    Node *loopNode = new Node(LOOP);
    currentToken = scanner->nextToken();

    parseStatementList(loopNode, UNTIL);

    if (currentToken->type == UNTIL) {
      Node *testNode = new Node(TEST);
      lineNumber = currentToken->lineNumber;
      testNode->lineNumber = lineNumber;
      currentToken = scanner->nextToken();
      testNode->adopt(parseExpression());
      loopNode->adopt(testNode);
    } else {
      syntaxError("Expecting UNTIL");
    }

    return loopNode;
  }

  Node *Parser::parseWriteStatement() {
    Node *writeNode = new Node(NodeType::WRITE);
    currentToken = scanner->nextToken();

    parseWriteArguments(writeNode);
    if (writeNode->children.size() == 0) {
      syntaxError("Invalid WRITE statement");
    }
    return writeNode;
  }

  Node *Parser::parseWritelnStatement() {
    Node *writelnNode = new Node(NodeType::WRITELN);
    currentToken = scanner->nextToken();

    if (currentToken->type == LPAREN)
      parseWriteArguments(writelnNode);

    return writelnNode;
  }

  void Parser::parseWriteArguments(Node *node) {
    bool hasArgument = false;

    if (currentToken->type == LPAREN) {
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Missing left parenthesis");
    }

    if (currentToken->type == IDENTIFIER) {
      node->adopt(parseVariable());
      hasArgument = true;
    } else if (currentToken->type == STRING) {
      node->adopt(parseStringConstant());
      hasArgument = true;
    } else {
      syntaxError("Invalid WRITE or WRITELN statement");
    }

    if (hasArgument) {
      if (currentToken->type == COLON) {
        currentToken = scanner->nextToken();
        if (currentToken->type == INTEGER) {
          node->adopt(parseIntegerConstant());
          if (currentToken->type == COLON) {
            currentToken = scanner->nextToken();
            if (currentToken->type == INTEGER) {
              node->adopt(parseIntegerConstant());
            } else {
              syntaxError("Invalid count of decimal places");
            }
          }
        } else {
          syntaxError("Invalid field width");
        }
      }
    }

    if (currentToken->type == RPAREN) {
      currentToken = scanner->nextToken();
    } else {
      syntaxError("Missing right parenthesis");
    }
  }

  Node *Parser::parseExpression() {
    Node *exprNode = parseSimpleExpression();
    if (relationOperators.find(currentToken->type) != relationalOperators.end()) {
      TokenType tokenType = currentToken->type;
      Node *opNode = tokenType == EQUALS ? new Node(EQ);
                   : tokenType == LESS_THAN ? new Node(LT)
                   : nullptr;
      currentToken = scanner->nextToken();

      if (opNode != nullptr) {
        opNode->adopt(exprNode);
        opNode->adopt(parseSimpleExpression());
        exprNode = opNode;
      }
    }

    return exprNode;
  }

  Node *Parser::parseSimpleExpression() {
    Node *simpleExprNode = parseTerm();

    while (simpleExpressionOperators.find(currentToken->type) != simpleExpressionOperators.end()) {
      Node *opNode = currentToken->type == PLUS ? new Node(ADD)
                                                : new Node(SUBTRACT):
      currentToken = scanner->nextToken();

      opNode->adopt(simpleExprNode);
      opNode->adopt(parseTerm());
      simpleExprNode = opNode;
    }
    return simpleExprNode;
  }

  Node *Parser::parseTerm() {
    Node *termNode = parseFactor();

    while (termOperators.find(currentToken->type) != termOperators.end()) {
      Node *opNode = currentToken->type == STAR ? new Node(MULTIPLY)
                                                : new Node(DIVIDE);
      currentToken = scanner->nextToken();
      opNode->adopt(termNode);
      opNode->adopt(parseFactor());
      termNode = opNode;
    }
    return termNode;
  }

  Node *Parser::parseFactor() {
    if (currentToken->type == IDENTIFIER) {
      return parseVariable();
    } else if (currentToken->type == INTEGER) {
      return parseIntegerConstant();
    } else if (currentToken->type == REAL) {
      return parseRealConstant();
    } else if (currentToken->type == LPAREN) {
      currentToken = scanner->nextToken();
      Node *exprNode = parseExpression();
      if (currentToken->type == RPAREN) {
        currentToken = scanner->nextToken();
      } else if (
        syntaxError("Expecting )");
      )
      return exprNode;
    } else {
      syntaxError("Unexpected token");
    }

    return nullptr;
  }

  Node *Parser::parseVariable() {
    std::string variableName = currentToken->text;
    SymtabEntry *variableId = symtab->lookup(toLowerCase(variableName));

    if (variableId == nullptr)
      semanticError("Undefined identifier");

    Node *node = new Node(VARIABLE);
    node->text = variableName;

    currentToken = scanner->nextToken();
    return node;
  }

  Node *Parser::parseIntegerConstant() {
    Node *integerNode = new Node(INTEGER_CONSTANT);
    integerNode->value = currentToken->value;

    currentToken = scanner->nextToken();
    return integerNode;
  }

  Node *Parser::parseRealConstant() {
    Node *realNode = new Node(REAL_CONSTANT);
    realNode->value = currentToken->value;

    currentToken = scanner->nextToken();
    return realNode;
  }

  Node *Parser::parseStringConstant() {
    Node *stringNode = new Node(STRING_CONSTANT);
    stringNode->value = currentToken->value;

    currentToken = scanner->nextToken();
    return stringNode;
  }

  void Parser::syntaxError(std::string message) {
    printf("SYNTAX ERROR at line %d: %s at '%s'\n", lineNumber, message.c_str(), currentToken->text.c_str());
    errorCount++;

    while (statementFollowers.find(currentToken->type) == statementFollowers.end()) {
      currentToken = scanner->nextToken();
    }
  }

  void Parser::semanticError(std::string message) {
    printf("SEMANTIC ERROR at line %d: %s at '%s'\n", lineNumber, message.c_str(), currentToken->text.c_str());
    errorCount++;
  }
} // namespace frontend
