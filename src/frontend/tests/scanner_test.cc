#include "scanner.hh"
#include <sstream>
#include <fstream>
#include <iostream>
#include <format>
#include <unordered_map>


using beacon_lox::TokenType;

// 完整的映射表
const std::unordered_map<TokenType, std::string> tokenTypeToString = {
    {TokenType::LEFT_PAREN, "LEFT_PAREN"},
    {TokenType::RIGHT_PAREN, "RIGHT_PAREN"},
    {TokenType::LEFT_BRACE, "LEFT_BRACE"},
    {TokenType::RIGHT_BRACE, "RIGHT_BRACE"},
    {TokenType::COMMA, "COMMA"},
    {TokenType::DOT, "DOT"},
    {TokenType::MINUS, "MINUS"},
    {TokenType::PLUS, "PLUS"},
    {TokenType::SEMICOLON, "SEMICOLON"},
    {TokenType::SLASH, "SLASH"},
    {TokenType::STAR, "STAR"},
    {TokenType::BANG, "BANG"},
    {TokenType::BANG_EQUAL, "BANG_EQUAL"},
    {TokenType::EQUAL, "EQUAL"},
    {TokenType::EQUAL_EQUAL, "EQUAL_EQUAL"},
    {TokenType::GREATER, "GREATER"},
    {TokenType::GREATER_EQUAL, "GREATER_EQUAL"},
    {TokenType::LESS, "LESS"},
    {TokenType::LESS_EQUAL, "LESS_EQUAL"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::STRING, "STRING"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::AND, "AND"},
    {TokenType::CLASS, "CLASS"},
    {TokenType::ELSE, "ELSE"},
    {TokenType::FALSE, "FALSE"},
    {TokenType::FUN, "FUN"},
    {TokenType::FOR, "FOR"},
    {TokenType::IF, "IF"},
    {TokenType::NIL, "NIL"},
    {TokenType::OR, "OR"},
    {TokenType::PRINT, "PRINT"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::SUPER, "SUPER"},
    {TokenType::THIS, "THIS"},
    {TokenType::TRUE, "TRUE"},
    {TokenType::VAR, "VAR"},
    {TokenType::WHILE, "WHILE"},
    {TokenType::END, "EOF"}};

// 这里已经忘记了怎么实现自定义 format 了.
template <>
class std::formatter<TokenType>
{
public:
  constexpr auto
  parse(std::format_parse_context &ctx)
  {
    return ctx.begin();
  }


  template <typename FormatContext>
  auto
  format(const TokenType &value, FormatContext &context) const
  {
    auto it = tokenTypeToString.find(value);
    std::string str = (it == tokenTypeToString.end()) ? "Unkonwn" : it->second;
    return std::format_to(context.out(), "{}", str);
  }
};

using Literal = std::variant<std::nullptr_t, std::string_view, double, bool>;


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
            return std::format_to(ctx.out(), "{}.0", value);
          }
          else
          {
            return std::format_to(ctx.out(), "{}", value);
          }
        },
        literal);
  }
};

int
main(int /*argc*/, char ** /*argv*/)
{
  // std::string_view path{argv[1]};
  std::stringstream ss;
  std::fstream file("/workspace/crafting_interpreters/data/bea.lox");
  if(!file.is_open())
  {
    return 65;
  }
  ss << file.rdbuf();
  beacon_lox::Scanner scanner{ss.str()};
  auto tokens = scanner.scan_tokens();

  for(const auto token : tokens)
  {
    // std::cout << "format:" << token.get_lexeme() << "\n";
    std::cout << std::format("{} {} {}\n",
                             token.get_type(),
                             token.get_lexeme(),
                             token.get_literal());
  }
  return 0;
}