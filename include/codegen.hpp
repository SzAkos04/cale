#pragma once

#include "stmt.hpp"
#include <fstream>
#include <vector>
class codegen_t {
  public:
    codegen_t(std::vector<stmt_t> statements);
    ~codegen_t() = default;

    int generate();

  private:
    std::vector<stmt_t> mStatements;
    int mCur;

    int gen_stmt(std::ofstream &outfile, const stmt_t &stmt);
    int gen_fn(std::ofstream &outfile, const stmt_t &stmt);
};
