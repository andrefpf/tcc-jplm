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

class OutOfBoundsException : public std::exception {
 protected:
  std::string message;

 public:
  OutOfBoundsException(size_t n_bytes, uint64_t initial_position, uint64_t final_position, uint64_t current_position)
      : message(
            std::string("Out of bounfs exception. Tried to access ")+std::to_string(n_bytes)+
            std::string("byte(s) from position ") + 
            std::to_string(current_position) +
            std::string(". However, this object only is permited to access between  ") +
            std::to_string(initial_position) + std::string(" to ") +
            std::to_string(final_position)) {
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


class ClosedStreamException : public std::exception {
public:
	const char* what() const noexcept override {
    return "ManagedStream needs an open stream in constructor";
  }	
};


}  // namespace ManagedStreamExceptions

#endif /* end of include guard: UTILS_STREAM_COMMONEXCEPTIONS_H__ */