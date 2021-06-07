#include <unordered_set>
#include <iostream>
#include <random>

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
}