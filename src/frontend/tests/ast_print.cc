#include "ast.hh"
#include <format>
#include <iomanip>
#include <iostream>
// #include <vector>
#include <cmath>

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



std::string
to_string(const beacon_lox::Expr &expr);


std::string
parenthesize(std::string_view name, const beacon_lox::Expr &expr)
{
  return std::format("({}{})", name, to_string(expr));
}
std::string
parenthesize(std::string_view name,
             const beacon_lox::Expr &expr1,
             const beacon_lox::Expr &expr2)
{
  return std::format("({} {} {})", name, to_string(expr1), to_string(expr2));
}



std::string
print_unary(const beacon_lox::UnaryExprPtr &expr)
{
  return parenthesize(expr->token.get_lexeme(), expr->expr);
}

std::string
print_literal(const beacon_lox::LiteralExprPtr &expr)
{
  // return parenthesize(expr->literal);
  return std::format("({})", expr->literal);
}

std::string
print_binary(const beacon_lox::BinaryExprPtr &expr)
{
  return std::format("({} {} {})",
                     expr->token.get_lexeme(),
                     to_string(expr->left),
                     to_string(expr->right));
}

std::string
to_string(const beacon_lox::Expr &expr)
{
  switch(expr.index())
  {
    case 0:
      return print_literal(std::get<0>(expr));
    case 1:
      return print_unary(std::get<1>(expr));
    case 2:
      return print_binary(std::get<2>(expr));
    case 3:
      return {"22"};
    default:
      //error
      return "";
  }
}


int
main()
{
  beacon_lox::Literal li("beacon");
  beacon_lox::Expr expr;
  expr = std::make_unique<beacon_lox::LiteralExpr>(li);
  std::cout << std::format("{}\n", to_string(expr));

  beacon_lox::Token minus(beacon_lox::TokenType::MINUS, "-", "-", 2);
  beacon_lox::Expr binary{
      std::make_unique<beacon_lox::UnaryExpr>(std::move(expr),
                                              minus,
                                              beacon_lox::UnaryOp::MINUS)};
  std::cout << std::format("{}\n", to_string(binary));

  beacon_lox::Expr b2{std::make_unique<beacon_lox::UnaryExpr>(
      std::make_unique<beacon_lox::LiteralExpr>(
          beacon_lox::Literal((float)17.2156487)),
      minus,
      beacon_lox::UnaryOp::MINUS)};
  std::cout << std::format("{}\n", to_string(b2));

  beacon_lox::Expr b3{
      std::make_unique<beacon_lox::UnaryExpr>(std::move(b2),
                                              minus,
                                              beacon_lox::UnaryOp::MINUS)};
  std::cout << std::format("{}\n", to_string(b3));


  beacon_lox::Token star{beacon_lox::TokenType::STAR, "*", "*", 3};
  beacon_lox::Expr s{std::make_unique<beacon_lox::LiteralExpr>("17.364561")};
  beacon_lox::Expr b4{
      std::make_unique<beacon_lox::BinaryExpr>(std::move(b3),
                                               star,
                                               beacon_lox::BinaryOp::STAR,
                                               std::move(s))};
  std::cout << std::format("{}\n", to_string(b4));

  float ff = 23.42132;
  std::cout << "ff:" << std::setprecision(1) << std::fixed << ff << "\n";
  return 0;
}