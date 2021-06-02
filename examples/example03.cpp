// g++ example03.cpp ../instruments/swap.cpp -o out

#include <iostream>

#include "../patterns/observable.hpp"
#include "../patterns/lazyobject.hpp"
#include "../instrument.hpp"
#include "../instruments/swap.hpp"

class A {
  public:
    void testA(const std::shared_ptr<A>&) {}
    void testB();
};

void A::testB() {
  // auto p = std::make_shared<A>(this); // not work
  std::shared_ptr<A> p(this);
  testA(p);
}


int main() {
  MiniQL::Observable observable;
  // MiniQL::Observer observer;
  std::cout << "Success.\n";
  std::cout << double() << std::endl;
  A a;
  a.testB();
  MiniQL::Instrument inst;
  MiniQL::Swap s;
}