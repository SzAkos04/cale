#pragma once

#include "expression.hpp"

#include <memory>
#include <string>
#include <utility>
#include <variant>
#include <vector>

enum class StmtType {
    EXPR,

    FN,
    RETURN,

    STMT_EOF,
    UNKNOWN,
};

class stmt_t {
  public:
    stmt_t(const std::string &name, const std::vector<std::string> &params,
           const std::vector<stmt_t> &body)
        : mType(StmtType::FN), mData(Fn{name, params, body}) {}

    stmt_t(std::unique_ptr<expr_t> expression)
        : mType(StmtType::RETURN), mData(Return{std::move(expression)}) {}

    stmt_t() : mType(StmtType::STMT_EOF) {}

    ~stmt_t() = default;

    StmtType getType() const { return mType; }

    const auto &getFn() const {
        if (std::holds_alternative<Fn>(mData)) {
            return std::get<Fn>(mData);
        }
        throw std::bad_variant_access();
    }

    const auto &getReturn() const {
        if (std::holds_alternative<Return>(mData)) {
            return std::get<Return>(mData);
        }
        throw std::bad_variant_access();
    }

  private:
    struct Fn {
        std::string name;
        std::vector<std::string> params;
        std::vector<stmt_t> body;
    };

    struct Return {
        std::unique_ptr<expr_t> expression;
    };

    StmtType mType;
    std::variant<Fn, Return> mData;
};
