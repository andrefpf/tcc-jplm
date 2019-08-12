#ifndef UTILS_STREAM_COMMONEXCEPTIONS_H__
#define UTILS_STREAM_COMMONEXCEPTIONS_H__

#include <exception>
#include <limits>
#include <string>

namespace ManagedStreamExceptions {
class OverflowOfInitialPositionException : public std::exception {
 protected:
  std::string message;

 public:
  OverflowOfInitialPositionException(uint64_t initial_position)
      : message(
            std::string("Overflow of initial position with value ") +
            std::to_string(initial_position) +
            std::string(". However, this class only supports a offset of ") +
            std::to_string(std::numeric_limits<int64_t>::max())) {
  }
  const char* what() const noexcept override {
    return message.c_str();
  }
};



class TryingToAccessBeyondEOFException : public std::exception {
public:
	const char* what() const noexcept override {
    return "ManagedStream tried to access data after EOF";
  }	
};

}  // namespace ManagedStreamExceptions

#endif /* end of include guard: UTILS_STREAM_COMMONEXCEPTIONS_H__ */