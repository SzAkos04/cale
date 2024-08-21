#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <utility>
#include <variant>

enum class LiteralType {
    String,
    Integer,
    Float,
    Boolean,
    Identifier,
    Null,
};

class literal_t {
  public:
    literal_t(std::string str)
        : mType(LiteralType::String), mData(std::move(str)) {}

    literal_t(long integer) : mType(LiteralType::Integer), mData(integer) {}

    literal_t(double floating) : mType(LiteralType::Float), mData(floating) {}

    literal_t(bool boolean) : mType(LiteralType::Boolean), mData(boolean) {}

    literal_t(std::string identifier, [[maybe_unused]] bool is_identifier)
        : mType(LiteralType::Identifier), mData(std::move(identifier)) {}

    literal_t() : mType(LiteralType::Null), mData(nullptr) {}

    ~literal_t() = default;

    LiteralType getType() const { return mType; }

    std::optional<std::string> getString() const {
        if (mType == LiteralType::String) {
            return std::get<std::string>(mData);
        }
        return std::nullopt;
    }

    std::optional<long> getInteger() const {
        if (mType == LiteralType::Integer) {
            return std::get<long>(mData);
        }
        return std::nullopt;
    }

    std::optional<double> getFloat() const {
        if (mType == LiteralType::Float) {
            return std::get<double>(mData);
        }
        return std::nullopt;
    }

    std::optional<bool> getBoolean() const {
        if (mType == LiteralType::Boolean) {
            return std::get<bool>(mData);
        }
        return std::nullopt;
    }

    std::optional<std::string> getIdentifier() const {
        if (mType == LiteralType::Identifier) {
            return std::get<std::string>(mData);
        }
        return std::nullopt;
    }

  private:
    LiteralType mType;
    std::variant<std::string, long, double, bool, std::nullptr_t> mData;
};
