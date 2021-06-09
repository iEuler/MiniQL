// g++ example07.cpp -o out
// test date, calendar, dayCount

#include <exception>
#include <iostream>

// #include "../time/date.hpp"
#include "../error.hpp"
#include "../interestrate.hpp"
#include "../quote.hpp"
#include "../termstructure.hpp"
#include "../termstructures/yieldtermstructure.hpp"

class A {
  public:
  virtual void print1() { std::cout << "class A virtual print1 \n"; }
  virtual void print2() { std::cout << "class A virtual print2 \n"; }
  void print_nonvirtual() { std::cout << "class A non virtual \n"; }
  void print() { 
    print1();
    print2();
  }
};

class B : public A {
  public:
  void print1() override { std::cout << "class B virtua print1 \n"; }  
  void print_nonvirtual() { std::cout << "class B non virtual \n"; }
};

class C : public B {
  public:
  void print2() override { std::cout << "class C virtua print2 \n"; }  
  void print_nonvirtual() { std::cout << "class C non virtual \n"; }
};


int main() {
  if (0) 
  {
    try 
    {
      QL_FAIL("Test ql fail.");
    }
    catch (std::exception &e)
    {
      std::cout << e.what() << std::endl;
    }
    QL_FAIL("Test ql fail 2nd.")
    QL_ENSURE(5<3, "Test ensure fail.");
  }

  A a;
  B b;
  C c;
  std::cout << "\nclass A: \n";
  a.print1();
  a.print2();
  a.print_nonvirtual();
  a.print();
  std::cout << "\nclass B: \n";
  b.print1();
  b.print2();
  b.print_nonvirtual();
  b.print();
  std::cout << "\nclass C: \n";
  c.print1();
  c.print2();
  c.print_nonvirtual();
  c.print();

  A &aa = a, &bb = b, &cc = c;

  std::cout << "\nBy reference: \n";
  std::cout << "\nclass A: \n";
  aa.print1();
  aa.print2();
  aa.print_nonvirtual();
  aa.print();
  std::cout << "\nclass B: \n";
  bb.print1();
  bb.print2();
  bb.print_nonvirtual();
  bb.print();
  std::cout << "\nclass C: \n";
  cc.print1();
  cc.print2();
  cc.print_nonvirtual();
  cc.print();





}