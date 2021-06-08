#include <unordered_set>
#include <iostream>
#include <random>
#include <memory>
#include <list>

#include <ext/new_allocator.h>

template<typename T>
class A {
  public:
    void print() { std::cout << "class A" << std::endl; }
};

typedef A<int> Aint;

class B: public Aint {
  public:
    void printB() { std::cout << "class B" << std::endl; }
};

int main() {
  std::unordered_multiset<int> c;
  std::cout << c.max_size() << std::endl;
  std::cout << c.bucket_count() << std::endl;
  auto cnt_last = c.bucket_count();
  
  for (int i=0; i<100; ++i) {
    c.insert(rand());
    auto cnt = c.bucket_count();
    if (cnt!=cnt_last) {
      std::cout << "i = " << i << ", bucket_count = " << cnt <<std::endl;
      cnt_last = cnt;
    }
  }

  A<double> a;
  B b;
  a.print();
  b.print();
  b.printB();

  std::list<int, std::allocator<int>> c1;
  std::list<int, __gnu_cxx::new_allocator<int>> c2;

  int* p;
  std::allocator<int> alloc1;
  p = alloc1.allocate(10);
  alloc1.deallocate(p, 10);

  __gnu_cxx::new_allocator<int> alloc2;
  p = alloc2.allocate(10);
  alloc2.deallocate(p, 10);
  
}