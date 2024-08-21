#include "token.hpp"

Token::Token(TokenType type, std::string lexeme, int line) {
    mType = type;
    mLexeme = lexeme;
    mLine = line;
}
