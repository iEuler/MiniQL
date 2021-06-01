// g++ example05.cpp ../patterns/observable.cpp -o out
#include <iostream>
#include <memory>
#include <vector>

#include "../handle.hpp"
#include "../patterns/observable.hpp"
#include "../typedef.hpp"

template <class T>
class A {
  public:
    A(const std::shared_ptr<std::vector<T>>& p) : p_(p) {}
  protected:
    std::shared_ptr<std::vector<T>> p_;
};

template <class T>
class B : public A<T> {
  public:
    B(const std::shared_ptr<std::vector<T>>& p) : A<T>(p) {}
    size_t size() { return this->p_->size();}     // NEED this->
};

template <class T>
class Base
{
protected:
  std::vector<T> data_;
};

template <class T>
class Derived : public Base<T>
{
public:
    void clear()
    {
      this->data_.clear();    // NEED this->
    }
};

int main() {
  MiniQL::Handle<MiniQL::Observable> x;
  std::cout << "Hello" << std::endl;    
  
  auto py = std::make_shared<std::vector<int>>(7);
  A<int> a(py);
  B<int> b(py);
  std::cout << b.size() << std::endl;

  Derived<int> derived;
  derived.clear();

}