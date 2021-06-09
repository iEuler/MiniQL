#ifndef MINIQL_ERROR_HPP
#define MINIQL_ERROR_HPP

#include <exception>
#include <memory>
#include <sstream>
#include <string>

#include "time/date.hpp"
#include "typedef.hpp"

namespace MiniQL {

class Error : public std::exception {
  public:
    
    Error(const std::string& file,
          long line,
          const std::string& functionName,
          const std::string& message = "")
    {
      std::string msg{format(file, line, functionName, message)};
      message_ = std::make_shared<std::string>(msg);
    }

    std::string format(const std::string& file, 
                       long line,
                       const std::string& functionName,
                       const std::string& message) {
      std::string msg{""};
// #ifdef QL_ERROE_LINES
      msg += file + ", line = " + std::to_string(line) + ", ";
// #endif
// #ifdef QL_ERROE_FUNCTIONS
      if (functionName != "(unknown)")
        msg += "in function '" + functionName + "': ";
// #endif
      msg += message;  
      return msg;
    }
    
    const char* what() const _GLIBCXX_USE_NOEXCEPT override { return message_->c_str(); };
    
  protected:
    std::shared_ptr<std::string> message_;
};

}  // namespace MiniQL

#define QL_FAIL(message) \
  std::ostringstream _ql_msg_stream;  \
  _ql_msg_stream << message;  \
  throw MiniQL::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, _ql_msg_stream.str()); 

#define QL_ASSERT(condition, message) \
  if (!(condition)) {\
    std::ostringstream _ql_msg_stream;  \
    _ql_msg_stream << message;  \
    throw MiniQL::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, _ql_msg_stream.str()); \
  } else

#define QL_REQUIRE(condition, message) \
  if (!(condition)) {\
    std::ostringstream _ql_msg_stream;  \
    _ql_msg_stream << message;  \
    throw MiniQL::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, _ql_msg_stream.str()); \
  } else

#define QL_ENSURE(condition, message) \
  if (!(condition)) {\
    std::ostringstream _ql_msg_stream;  \
    _ql_msg_stream << message;  \
    throw MiniQL::Error(__FILE__, __LINE__, __PRETTY_FUNCTION__, _ql_msg_stream.str()); \
  } else

#endif 
