#include "debug.hpp"
#include "parser.hpp"
#include "stmt.hpp"
#include "token.hpp"

#include <optional>
#include <string>
#include <vector>

parser_t::parser_t(std::vector<Token> tokens) {
    mTokens = tokens;
    mCur = 0;
}

std::optional<std::vector<stmt_t>> parser_t::parse() {
    std::vector<stmt_t> statements;

    std::optional<stmt_t> statement;

    while (true) {
        statement = next();

        if (!statement.has_value()) {
            return std::nullopt;
        }

        if (statement->getType() == StmtType::STMT_EOF) {
            break;
        }

        statements.push_back(std::move(*statement));
    }

    return statements;
}

std::optional<stmt_t> parser_t::next() {
    switch (mTokens[mCur].getType()) {
    case TokenType::FN: {
        mCur++; // consume `fn`
        if (mTokens[mCur].getType() != TokenType::IDENTIFIER) {
            error("Identifier expected");
            return std::nullopt;
        }
        std::string name = mTokens[mCur].getLexeme();
        mCur++; // consume identifier

        if (mTokens[mCur].getType() != TokenType::LEFT_PAREN) {
            error("`(` expected");
            return std::nullopt;
        }
        mCur++; // consume `(`

        std::vector<std::string> params;

        if (mTokens[mCur].getType() != TokenType::RIGHT_PAREN) {
            error("`)` expected");
            return std::nullopt;
        }
        mCur++; // consume `)`

        if (mTokens[mCur].getType() != TokenType::LEFT_BRACE) {
            error("`{` expected");
            return std::nullopt;
        }
        mCur++; // consume `{`

        std::vector<stmt_t> body;
        while (mTokens[mCur].getType() != TokenType::RIGHT_BRACE) {
            /* body.push_back(*next()); */
            mCur++;
        }
        mCur++; // consume `}`

        return stmt_t(name, params, body);
    }
    case TokenType::RETURN:

    case TokenType::TT_EOF:
        return stmt_t();
    default:
        return stmt_t();
    }
}
