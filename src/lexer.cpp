#include "debug.hpp"
#include "lexer.hpp"
#include "token.hpp"

#include <cctype>
#include <optional>
#include <string>
#include <vector>

lexer_t::lexer_t(std::string buf) {
    mBuf = buf;
    mCur = 0;
    mLine = 1;
}

std::optional<std::vector<Token>> lexer_t::lex() {
    std::vector<Token> tokens;

    std::optional<Token> token;
    while (true) {
        token = next();

        if (!token.has_value()) {
            return std::nullopt;
        }

        if (token->getType() == TokenType::TT_EOF) {
            break;
        }

        tokens.push_back(std::move(*token));
    }

    return tokens;
}

std::optional<Token> lexer_t::next() {
    while (std::isspace(mBuf[mCur])) {
        if (mBuf[mCur] == '\n') {
            mLine++;
        }
        mCur++;
    }

    switch (mBuf[mCur]) {
    // single character tokens
    case '(':
        mCur++;
        return Token(TokenType::LEFT_PAREN, "(", mLine);
    case ')':
        mCur++;
        return Token(TokenType::RIGHT_PAREN, ")", mLine);
    case '{':
        mCur++;
        return Token(TokenType::LEFT_BRACE, "{", mLine);
    case '}':
        mCur++;
        return Token(TokenType::RIGHT_BRACE, "}", mLine);
    case ';':
        mCur++;
        return Token(TokenType::SEMICOLON, ";", mLine);
    case ':':
        mCur++;
        return Token(TokenType::COLON, ":", mLine);

    // one or two character tokens
    case '!':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::BANG_EQUAL, "!=", mLine);
        } else {
            return Token(TokenType::BANG, "!", mLine);
        }
    case '=':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::EQUAL_EQUAL, "==", mLine);
        } else {
            return Token(TokenType::EQUAL, "=", mLine);
        }
    case '>':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::GREATER_EQUAL, ">=", mLine);
        } else {
            return Token(TokenType::GREATER, ">", mLine);
        }
    case '<':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::LESS_EQUAL, "<=", mLine);
        } else {
            return Token(TokenType::LESS, "<", mLine);
        }
    case '+':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::PLUS_EQUAL, "+=", mLine);
        } else {
            return Token(TokenType::PLUS, "+", mLine);
        }
    case '-':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::MINUS_EQUAL, "-=", mLine);
        } else {
            return Token(TokenType::MINUS, "-", mLine);
        }
    case '/':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::SLASH_EQUAL, "/=", mLine);
        } else if (mBuf[mCur] == '/') {
            while (mBuf[++mCur] == '\n')
                ;
            return Token(TokenType::COMMENT, "", mLine);
            // TODO: multi-line comments
        } else {
            return Token(TokenType::SLASH, "/", mLine);
        }
    case '*':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::STAR_EQUAL, "*=", mLine);
        } else {
            return Token(TokenType::STAR, "*", mLine);
        }
    case '%':
        mCur++;
        if (mBuf[mCur] == '=') {
            mCur++;
            return Token(TokenType::MODULO_EQUAL, "%=", mLine);
        } else {
            return Token(TokenType::MODULO, "%", mLine);
        }

    // two character tokens
    case '&':
        mCur++;
        if (mBuf[mCur] == '&') {
            mCur++;
            return Token(TokenType::AND, "&&", mLine);
        } else {
            return std::nullopt;
        }
    case '|':
        mCur++;
        if (mBuf[mCur] == '|') {
            mCur++;
            return Token(TokenType::OR, "||", mLine);
        } else {
            return std::nullopt;
        }

    case '"': {
        mCur++;
        int start = mCur;
        int max_len = mBuf.length() - 1;
        while (mBuf[mCur] != '"') {
            if (mCur > max_len) {
                std::string err_msg =
                    "unterminated string at line " + std::to_string(mLine);
                error(err_msg);
                return std::nullopt;
            }
            mCur++;
        }
        std::string lexeme = mBuf.substr(start, mCur - start);
        debug("string: " << lexeme);
        mCur++;
        return Token(TokenType::STRING, lexeme, mLine);
    }
    case '\0':
        return Token(TokenType::TT_EOF, "", mLine);
    default:
        if (std::isdigit(mBuf[mCur])) {
            int start = mCur;
            while (std::isdigit(mBuf[mCur]) ||
                   (mBuf[mCur] == '.' && std::isdigit(mBuf[mCur + 1]))) {
                mCur++;
            }
            std::string lexeme = mBuf.substr(start, mCur - start);

            return Token(TokenType::NUMBER, lexeme, mLine);
        } else if (std::isalpha(mBuf[mCur]) || mBuf[mCur] == '_') {
            int start = mCur;
            while (std::isalnum(mBuf[mCur]) || mBuf[mCur] == '_') {
                mCur++;
            }
            std::string lexeme = mBuf.substr(start, mCur - start);

            TokenType tt = TokenType::IDENTIFIER;
            if (lexeme == "fn") {
                tt = TokenType::FN;
            } else if (lexeme == "return") {
                tt = TokenType::RETURN;
            }
            return Token(tt, lexeme, mLine);
        } else {
            error("not yet implemented");
            return std::nullopt;
        }
    }
}
