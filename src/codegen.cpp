#include "codegen.hpp"
#include "debug.hpp"
#include "stmt.hpp"

#include <fstream>

codegen_t::codegen_t(std::vector<stmt_t> statements) {
    mStatements = statements;
    mCur = 0;
}

int codegen_t::generate() {
    std::ofstream outfile("temp.asm");
    if (!outfile.good()) {
        perr("failed to open file");
        return 1;
    }

    outfile << "section .text" << std::endl;
    outfile << "global _start" << std::endl;

    outfile << "_start:" << std::endl;
    outfile << "call main" << std::endl;

    outfile << "mov ebx, eax" << std::endl;
    outfile << "mov eax, 1" << std::endl;
    outfile << "int 0x80" << std::endl;

    for (auto &statement : mStatements) {
        gen_stmt(outfile, statement);
    }

    outfile.close();
    return 0;
}

int codegen_t::gen_stmt(std::ofstream &outfile, const stmt_t &stmt) {
    switch (stmt.getType()) {
    case StmtType::FN:
        return gen_fn(outfile, stmt);
    default:
        error("not yet implemented");
        return 1;
    }
}
int codegen_t::gen_fn(std::ofstream &outfile, const stmt_t &stmt) {
    const auto &fn = stmt.getFn();
    outfile << fn.name << ":" << std::endl;

    constexpr int param_size = 4; // assume all parameters need 4 bytes

    outfile << "push ebp" << std::endl;
    outfile << "mov ebp, esp" << std::endl;
    outfile << "sub esp, " << fn.params.size() * param_size << std::endl;

    for (auto &statement : fn.body) {
        gen_stmt(outfile, statement);
    }

    if (fn.name == "main") {
        // make main return 0 by default
        outfile << "mov eax, 0" << std::endl;
    }

    outfile << "mov esp, ebp" << std::endl;
    outfile << "pop ebp" << std::endl;
    outfile << "ret" << std::endl;

    return 0;
}
