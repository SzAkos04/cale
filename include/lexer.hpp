#pragma once

#include "token.hpp"

#include <optional>
#include <string>
#include <vector>

class lexer_t {
  public:
    lexer_t(std::string buf);
    ~lexer_t() = default;

    std::optional<std::vector<Token>> lex();

  private:
    std::string mBuf;
    int mCur;
    int mLine;

    std::optional<Token> next();
};
