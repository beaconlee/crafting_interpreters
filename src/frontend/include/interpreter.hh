#pragma once


#include "ast.hh"


namespace beacon_lox
{

class Interpreter : public Visitor
{
public:
  std::any
  literal_expr_visitor(LiteralExpr *literal) override
  {
    return literal->literal;
  }
  std::any
  unary_expr_visitor(UnaryExpr *unary) override
  {}
  std::any
  binary_expr_visitor(BinaryExpr *binary) override
  {}
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
};
} // namespace beacon_lox