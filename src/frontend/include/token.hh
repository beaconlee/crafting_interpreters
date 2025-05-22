#pragma once

#include <string_view>
#include <variant>
#include <format>

namespace beacon_lox
{
enum TokenType
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
  BANS,
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

  LOX_EOF
};

using Literal = std::variant<std::nullptr_t, std::string_view, double, bool>;


class Token
{
public:
  explicit Token(const TokenType type,
                 const std::string_view lexeme,
                 const Literal &literal,
                 const unsigned int line)
    : type_{type}
    , lexeme_{lexeme}
    , literal_{literal}
    , line_{line}
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

// 自定义格式化器
template <>
struct std::formatter<beacon_lox::Literal>
{
  constexpr auto
  parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  auto
  format(const beacon_lox::Literal &literal, std::format_context &ctx) const
  {
    // visit 本身需要的也是一个函数对象, 这里使用lambda
    // 没有使用重载的 () 运算符, 而是手动返回
    return std::visit(
        [&ctx](const auto &value)
        {
          using T = std::decay_t<decltype(value)>;
          // if constexpr(std::is_same_v<T, std::nullptr_t>)
          // {
          //   return std::format_to(ctx.out(), "null");
          // }
          // else if constexpr(std::is_same_v<T, std::string_view> ||
          //                   std::is_same_v<T, double>)
          // {
          //   return std::format_to(ctx.out(), "{}", value);
          // }
          // else if constexpr(std::is_same_v<T, bool>)
          // {
          //   return std::format_to(ctx.out(), "{}", value ? "true" : "false");
          // }
          if constexpr(std::is_same_v<T, std::nullptr_t>)
          {
            return std::format_to(ctx.out(), "null");
          }
          else if constexpr(std::is_same_v<T, bool>)
          {
            return std::format_to(ctx.out(), "{}", value ? "true" : "false");
          }
          // 因为 value 的类型只能是这4种
          else if constexpr(std::is_same_v<T, double>)
          {
            // return std::format_to(ctx.out(), "{}", value);
            // 对于输出精度的理解...
            double d = 0.0;
            // if(std::abs(std::modf(value, &d)) < 1e-10)
            // {
            //   return std::format_to(ctx.out(), "{:.1f}", value);
            // }
            return std::format_to(ctx.out(), "{}", value);
          }
          else
          {
            return std::format_to(ctx.out(), "{}", value);
          }
        },
        literal);
  }
};



// 自定义格式化器
template <>
struct std::formatter<beacon_lox::Token>
{
  constexpr auto
  parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }

  auto
  format(const beacon_lox::Token &token, std::format_context &ctx) const
  {
    // visit 本身需要的也是一个函数对象, 这里使用lambda
    // 没有使用重载的 () 运算符, 而是手动返回
    return std::format_to(ctx.out(), "{}", token.get_lexeme());
  }
};
