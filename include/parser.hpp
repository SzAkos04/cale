#pragma once

#include "stmt.hpp"
#include "token.hpp"

#include <optional>
#include <vector>

class parser_t {
  public:
    parser_t(std::vector<Token> tokens);
    ~parser_t() = default;

    std::optional<std::vector<stmt_t>> parse();

  private:
    std::vector<Token> mTokens;
    int mCur;

    std::optional<stmt_t> next();
};
