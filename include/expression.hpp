#include "literal.hpp"

#include <memory>
#include <string>
#include <utility>
#include <variant>

enum class ExprType {
    UNARY,
    BINARY,
    LITERAL,
    VARIABLE,
    ASSIGNMENT,
};

enum class UnaryOp {
    NEGATION,
    LOGICAL_NOT,
};

enum class BinaryOp {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    LOGICAL_AND,
    LOGICAL_OR,
    EQUALITY,
    INEQUALITY,
};

class expr_t {
  public:
    expr_t(literal_t literal)
        : mType(ExprType::LITERAL), mData(std::move(literal)) {}

    expr_t(std::string variable)
        : mType(ExprType::VARIABLE), mData(std::move(variable)) {}

    expr_t(UnaryOp op, std::unique_ptr<expr_t> operand)
        : mType(ExprType::UNARY), mData(UnaryExpr{op, std::move(operand)}) {}

    expr_t(BinaryOp op, std::unique_ptr<expr_t> lhs,
           std::unique_ptr<expr_t> rhs)
        : mType(ExprType::BINARY),
          mData(BinaryExpr{op, std::move(lhs), std::move(rhs)}) {}

    expr_t(std::string variable, std::unique_ptr<expr_t> value)
        : mType(ExprType::ASSIGNMENT),
          mData(AssignmentExpr{std::move(variable), std::move(value)}) {}

    ~expr_t() = default;

    ExprType getType() const { return mType; }

    std::optional<literal_t> getLiteral() const {
        if (mType == ExprType::LITERAL) {
            return std::get<literal_t>(mData);
        }
        return std::nullopt;
    }

    std::optional<std::string> getVariable() const {
        if (mType == ExprType::VARIABLE) {
            return std::get<std::string>(mData);
        }
        return std::nullopt;
    }

    std::optional<std::pair<UnaryOp, expr_t *>> getUnaryExpr() const {
        if (mType == ExprType::UNARY) {
            const auto &expr = std::get<UnaryExpr>(mData);
            return std::make_pair(expr.op, expr.operand.get());
        }
        return std::nullopt;
    }

    std::optional<std::tuple<BinaryOp, expr_t *, expr_t *>>
    getBinaryExpr() const {
        if (mType == ExprType::BINARY) {
            const auto &expr = std::get<BinaryExpr>(mData);
            return std::make_tuple(expr.op, expr.lhs.get(), expr.rhs.get());
        }
        return std::nullopt;
    }

    std::optional<std::pair<std::string, expr_t *>> getAssignmentExpr() const {
        if (mType == ExprType::ASSIGNMENT) {
            const auto &expr = std::get<AssignmentExpr>(mData);
            return std::make_pair(expr.variable, expr.value.get());
        }
        return std::nullopt;
    }

  private:
    struct UnaryExpr {
        UnaryOp op;
        std::unique_ptr<expr_t> operand;
    };

    struct BinaryExpr {
        BinaryOp op;
        std::unique_ptr<expr_t> lhs;
        std::unique_ptr<expr_t> rhs;
    };

    struct AssignmentExpr {
        std::string variable;
        std::unique_ptr<expr_t> value;
    };

    ExprType mType;
    std::variant<literal_t, std::string, UnaryExpr, BinaryExpr, AssignmentExpr>
        mData;
};
