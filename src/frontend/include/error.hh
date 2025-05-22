#pragma once

#include <vector>
#include <string>
#include <format>

#include "token.hh"


namespace beacon_lox
{
enum class LoxStatus
{
  OK,
  ERROR
};

struct RuntimeError : public std::runtime_error
{
  Token _token;
  explicit RuntimeError(const Token &token, const std::string &msg)
    : std::runtime_error(msg)
    , _token(token)
  {}
};

class Error
{
public:
  void
  error(const long unsigned int line, const std::string_view msg)
  {
    report(line, "", msg);
  }
  void
  error(const Token &token, const std::string_view msg)
  {
    if(token.get_type() == TokenType::LOX_EOF)
    {
      report(token.get_line(), " at end", msg);
      return;
    }

    report(token.get_line(),
           " at '" + std::string(token.get_lexeme()) + "'",
           msg);
  }

  void
  error(const RuntimeError &runtime_error)
  {
    error(runtime_error._token, runtime_error.what());
  }


private:
  void
  report(const long unsigned int line,
         const std::string_view where,
         const std::string_view msg)
  {
    errs_.emplace_back(
        std::format("[line {}] Error{}: {}\n", line, where, msg));
  }
  std::vector<std::string> errs_;
  LoxStatus status_{LoxStatus::OK};
};
} // namespace beacon_lox