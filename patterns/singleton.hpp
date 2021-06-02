#ifndef MINIQL_SINGLETON_HPP
#define MINIQL_SINGLETON_HPP

#include <memory>

namespace MiniQL {

template <typename T>
class Singleton {
  public:
    static T& instance();
    Singleton(const Singleton&) = delete;   // disallow copy constructor
    Singleton operator=(const Singleton&) = delete;   // disallow copy assignment
  protected:
    Singleton() = default;  
};

template <typename T>
T& Singleton<T>::instance() {
  static std::shared_ptr<T> instance_(new T);
  return *instance_;

  // static T instance_;  // This version also works. However it may not work in multi-thread case (?)
  // return instance_;
}

}  // namespace MiniQL

#endif 
