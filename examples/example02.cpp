#include <iostream>
#include <vector>

// #include "singleton.hpp"
#include "../settings.hpp"


int main() {
  MiniQL::Settings::instance().evaluationDate() = 3.4;  
  std::cout << MiniQL::Settings::instance().evaluationDate() << std::endl;

  auto x = std::vector<int>({7});
  auto y = std::vector<int>{7};
  auto z = std::vector<int>(7);
  for (auto i : y) std::cout << i << ", ";
  std::cout << std::endl;
  
}