#pragma once

#include <string_view>
#include <variant>

namespace beacon_lox
{
enum class TokenType
{ // Single-character tokens.
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  // One or two character tokens.
  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  // Literals.
  IDENTIFIER,
  STRING,
  NUMBER,

  // Keywords.
  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  END
};

using Literal = std::variant<std::nullptr_t, std::string_view, double, bool>;

class Token
{
public:
  explicit Token(const TokenType type,
                 const std::string_view lexeme,
                 const Literal &literal,
                 const unsigned int line)
    : type_(type)
    , lexeme_(lexeme)
    , literal_(literal)
    , line_(line)
  {}

  [[nodiscard]] TokenType
  get_type() const
  {
    return type_;
  }

  [[nodiscard]] std::string_view
  get_lexeme() const
  {
    return lexeme_;
  }

  [[nodiscard]] Literal
  get_literal() const
  {
    return literal_;
  }

  [[nodiscard]] unsigned int
  get_line() const
  {
    return line_;
  }

private:
  TokenType type_;
  // 形式区分,  lexis: 词语,单词 -eme: 表示最小单位
  // 最小语义单元的原始字符串表示, 就是 token
  std::string_view lexeme_;
  // 语义, 就是值
  Literal literal_;
  unsigned int line_;
};

} // namespace beacon_lox