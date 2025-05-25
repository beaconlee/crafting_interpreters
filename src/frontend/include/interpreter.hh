#pragma once


#include "ast.hh"
#include "error.hh"
#include <iostream>

namespace beacon_lox
{

class Interpreter : public Visitor
{
public:
  void
  interpret(const Expr &expr)
  {
    try
    {
      auto value = evaluate(expr);
      std::cout << "result: " << stringify(value) << "\n";
    }
    catch(const std::exception &e)
    {
      std::cout << "catch exception: " << e.what() << "\n";
    }
  }

  std::any
  literal_expr_visitor(LiteralExpr *literal) override
  {
    // std::cout << "literal_expr_visitor::literal->literal type:" << literal->literal.index() << "\n";
    // // 这样返回的话, 这里返回的是一个 variant 对象, 而非一个double 类型数值.
    // !它被自动包装为 std::any 存储一个 std::variant 对象
    // return literal->literal;
    // // return 3.0;

    // std::cout << "literal_expr_visitor::literal->literal index:"
    //           << literal->literal.index() << "\n";

    // 使用 std::visit 提取并返回具体值
    return std::visit(
        [](const auto &value) -> std::any
        {
          // 所有类型都可以直接包装到 std::any
          return std::any{value};
        },
        literal->literal);
  }

  std::any
  unary_expr_visitor(UnaryExpr *unary) override
  {
    auto value = evaluate(unary->expr);
    switch(unary->op)
    {
      case UnaryOp::MINUS:
        try
        {
          return -std::any_cast<double>(value);
        }
        catch(std::exception &e)
        {
          // std::cout << "value: " << std::any_cast<std::string>(value)
          //           << "\n";
          throw Error::RuntimeError(unary->token, "unary minus must be number");
        }
      case UnaryOp::BANS:
        // 这里的关键点是, 在 lox 中除了 ture, 其它的都是 false
        return !is_true(value);
      default:
        // throw Error::RuntimeError(unary->token, "unkown unary operator");
        std::cout << "runtime error\n";
        return "runtime error";
    }
  }
  std::any
  binary_expr_visitor(BinaryExpr *binary) override
  {
    auto left = evaluate(binary->left);
    auto right = evaluate(binary->right);
    switch(binary->op)
    {
      case BinaryOp::PLUS:
        return std::any_cast<double>(left) + std::any_cast<double>(right);
      case BinaryOp::MINUS:
      case BinaryOp::SLASH:
      case BinaryOp::STAR:
      case BinaryOp::BANG_EQUAL:
      case BinaryOp::EQUAL_EQUAL:
      case BinaryOp::LESS_EQUAL:
      case BinaryOp::GREATER_EQUAL:
      case BinaryOp::LESS:
      case BinaryOp::GREATER:
        break;
    }
    return true;
  }
  std::any
  grouping_expr_visitor(GroupingExpr *grouping) override
  {
    return evaluate(grouping->expr);
  }

private:
  std::any
  evaluate(const Expr &expr)
  {
    // 这里忘记了 variant 的 visit 访问方法, accept 并不是 variant 的, 而是里面的值的
    // return expr.accept(this);
    return std::visit([this](const auto &value) { return value->accept(this); },
                      expr);
  }

  std::string
  stringify(std::any any)
  {
    if(is_type<std::nullptr_t>(any))
    {
      return "nil";
    }

    if(is_type<double>(any))
    {
      return std::to_string(std::any_cast<double>(any));
    }

    if(is_type<bool>(any))
    {
      return std::any_cast<bool>(any) ? "ture" : "false";
    }

    return std::any_cast<std::string>(any);
  }
  // 除了 nullptr 或者 false, 其它任何的东西都是 true
  bool
  is_true(std::any value)
  {
    if(is_type<std::nullptr_t>(value))
    {
      return false;
    }

    if(is_type<bool>(value))
    {
      return std::any_cast<bool>(value);
    }

    return true;
  }

  template <typename T>
  bool
  is_type(const std::any &any)
  {
    return any.type() == typeid(T);
  }
};
} // namespace beacon_lox