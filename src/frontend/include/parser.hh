#pragma once

#include "ast.hh"
#include "token.hh"
#include <vector>


namespace beacon_lox
{
class Parser
{
public:
  Parser(std::vector<Token> token)
    : tokens_(std::move(token))
  // , cur_iter_(tokens_.begin())
  // , end_iter_(tokens_.end())
  {}

private:
  auto
  expression() -> Expr
  {
    return equality();
  }

  auto
  equality() -> Expr
  {
    Expr expr = comparsion();
    while(match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL))
    {
      // Token operator = p
      auto op = previos();

      // expr = std::make_unique<BinaryExpr>(std::move(expr),);
    }

    return expr;
  }

  auto
  comparsion() -> Expr
  {
    return {};
  }

  // 关于 match 自己的意图和作者的存在区别
  // 作者书中的意图是当发现匹配时,就会消耗掉这个token
  // [[nodiscard]] auto
  // match(const TokenType &type) const -> bool
  // {
  //   if(is_at_end())
  //   {
  //     return false;
  //   }

  //   return type == tokens_[cur_].get_type();
  // }
  [[nodiscard]] auto
  match(const TokenType &type) -> bool
  {
    if(check(type))
    {
      advance();
      return true;
    }
    return false;
  }

  // [[nodiscard]] auto
  // check(const TokenType &type) const -> bool
  // {
  //   if(is_at_end())
  //   {
  //     return false;
  //   }

  //   return type == tokens_[cur_].get_type();
  // }
  template <typename... Args>
  [[nodiscard]] auto
  check(const Args &...args) const -> bool
  {
    if(is_at_end())
    {
      return false;
    }

    return ((tokens_[cur_].get_type() == args) || ...);
  }

  template <typename... Args>
  [[nodiscard]] auto
  match(Args... args) -> bool
  {
    if(check(args...))
    {
      advance();
      return true;
    }

    return false;
  }

  [[nodiscard]] auto
  peek() -> const Token &
  {
    // 感觉这里应该还是要加一个处理的
    //   if(cur_iter_ == end_iter_)
    //   {
    //     return
    //   }
    return tokens_[cur_];
  }

  [[nodiscard]] auto
  previos() -> const Token &
  {
    return tokens_[cur_ - 1];
  }

  [[nodiscard]] auto
  is_at_end() const -> bool
  {
    return tokens_[cur_].get_type() == TokenType::LOX_EOF;
  }

  // 昨天自己有个疑问, 如果超过最后一个字符要怎么处理,这里显示了结果
  // 如果已经到了最后了, 不移动 迭代器 就可以了
  void
  advance()
  {
    if(is_at_end())
    {
      return;
    }
    ++cur_;
  }

  std::vector<Token> tokens_;
  // std::vector<Token>::iterator cur_iter_;
  int cur_{0};
  // 预期: 每个 token 序列的最后一个都是 EOF!
  // 所以这里没有必要单独存储一个 end 了
  // std::vector<Token>::iterator end_iter_;
};
} // namespace beacon_lox