// shared_ptr works on virtual function
// for dynamic casting of shared_ptr, use std::dynamic_pointer_cast

#include <iostream>
#include <memory>

class A {
  public:
    virtual void print() const { std::cout << "class A.\n"; }
};

class B : public A {
  public:
    void print() const override { std::cout << "class B.\n"; }
    void printB() const { std::cout << "class B nonvirtual.\n"; }
    
};

/* Not working
int f(int i)[[expects:i > 0]]
{
    return i;
}
*/

int main() {
  A* a = new A;
  A* ab = new B;
  B* b = new B;
  a->print();
  ab->print();
  b->print();
  // ab->printB();  // not working
  B* ab2 = dynamic_cast<B*>(ab);
  ab2->printB();

  std::shared_ptr<A> sa(new A);
  std::shared_ptr<A> sab(new B);
  std::shared_ptr<B> sb(new B);

  sa->print();
  sab->print();
  sb->print();

  // std::shared_ptr<B> sab2a = dynamic_cast<B>(sab);  // not working
  std::shared_ptr<B> sab2 = std::dynamic_pointer_cast<B>(sab);
  ab2->printB();

  // std::cout << f(5) << std::endl;

}