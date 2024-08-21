#pragma once

#include <string>
enum class TokenType {
    // single character tokens
    LEFT_PAREN,  // `(`
    RIGHT_PAREN, // `)`
    LEFT_BRACE,  // `{`
    RIGHT_BRACE, // `}`
    SEMICOLON,   // `;`
    COLON,       // `:`

    // one or two character tokens
    BANG,          // `!`
    BANG_EQUAL,    // `!=`
    EQUAL,         // `=`
    EQUAL_EQUAL,   // `==`
    GREATER,       // `>`
    GREATER_EQUAL, // `>=`
    LESS,          // `<`
    LESS_EQUAL,    // `<=`
    PLUS,          // `+`
    PLUS_EQUAL,    // `+=`
    MINUS,         // `-`
    MINUS_EQUAL,   // `-=`
    SLASH,         // `/`
    SLASH_EQUAL,   // `/=`
    STAR,          // `*`
    STAR_EQUAL,    // `*=`
    MODULO,        // `%`
    MODULO_EQUAL,  // `%=`

    // two character tokens
    AND, // `&&`
    OR,  // `||`

    // literals
    IDENTIFIER,
    STRING,
    NUMBER,
    BOOL,
    TT_NULL,

    // keywords
    FN,     // `fn`
    RETURN, // `return`

    COMMENT,

    TT_EOF,
    UNKNOWN,
};

class Token {
  public:
    Token() = default;
    Token(TokenType type, std::string lexeme, int line);
    ~Token() = default;

    TokenType getType() { return mType; }
    std::string getLexeme() { return mLexeme; }
    int getLine() { return mLine; }

  private:
    TokenType mType;
    std::string mLexeme;
    int mLine;
};
