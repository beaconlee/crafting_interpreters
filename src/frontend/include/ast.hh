#pragma once

#include "token.hh"
#include "utils.hh"
#include <variant>
#include <memory>


namespace beacon_lox
{

enum class UnaryOp
{
  BANS = static_cast<int>(TokenType::BANG),
  MINUS = static_cast<int>(TokenType::MINUS),
};

enum class BinaryOp
{
  PLUS = static_cast<int>(TokenType::PLUS),
  MINUS = static_cast<int>(TokenType::MINUS),
  SLASH = static_cast<int>(TokenType::SLASH),
  STAR = static_cast<int>(TokenType::STAR),
  BANG_EQUAL = static_cast<int>(TokenType::BANG_EQUAL),
  EQUAL_EQUAL = static_cast<int>(TokenType::EQUAL_EQUAL),
  LESS_EQUAL = static_cast<int>(TokenType::LESS_EQUAL),
  GREATER_EQUAL = static_cast<int>(TokenType::GREATER_EQUAL),
  LESS = static_cast<int>(TokenType::LESS),
  GREATER = static_cast<int>(TokenType::GREATER),
};



struct LiteralExpr;
struct UnaryExpr;
struct BinaryExpr;
struct GroupingExpr;

using LiteralExprPtr = std::unique_ptr<LiteralExpr>;
using UnaryExprPtr = std::unique_ptr<UnaryExpr>;
using BinaryExprPtr = std::unique_ptr<BinaryExpr>;
using GroupingExprPtr = std::unique_ptr<GroupingExpr>;


using Expr =
    std::variant<LiteralExprPtr, UnaryExprPtr, BinaryExprPtr, GroupingExprPtr>;

struct LiteralExpr : private Uncopyabble
{
  Literal literal;
  explicit LiteralExpr(Literal _literal)
    : literal(_literal)
  {}
};

struct BinaryExpr : private Uncopyabble
{
  Expr left;
  Token token;
  BinaryOp op;
  Expr right;
  explicit BinaryExpr(Expr _left, Token _token, BinaryOp _op, Expr _right)
    : left(std::move(_left))
    , token(_token)
    , op(_op)
    , right(std::move(_right))
  {}
};

struct UnaryExpr : private Uncopyabble
{
  Expr expr;
  Token token;
  UnaryOp op;

  explicit UnaryExpr(Expr _expr, Token _token, UnaryOp _op)
    : expr(std::move(_expr))
    , token(_token)
    , op(_op)
  {}
};

struct GroupingExpr : private Uncopyabble
{
  Expr expr;
  explicit GroupingExpr(Expr _expr)
    : expr(std::move(_expr))
  {}
};

} // namespace beacon_lox