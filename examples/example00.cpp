#include <iostream>
#include <string>

void print() { std::cout << std::endl; }

template<typename T, typename... Args>
void print(const T& firstArg, const Args&... args) {
  std::cout << firstArg << ", ";
  print(args...);
}

template<typename... Args>
void print(const Args&... args) {  
  std::cout << sizeof...(args) << ", ";
  std::cout << "Another variadic function." << std::endl;  
}

int main() {
  std::cout << __cplusplus << std::endl;
  print("hello", 4, 2.3);
  std::cout << std::hash<int>() (123) << std::endl;
  std::cout << std::hash<std::string>() ("hello") << std::endl;
  std::cout << std::hash<std::string>() (std::string("hello")) << std::endl;
  std::cout << std::hash<const char*>() ("hello") << std::endl;
  std::cout << std::hash<char>() ('h') << std::endl;
  std::cout << std::hash<double>() (3.1415926) << std::endl;
  std::cout << std::hash<float>() (3.1415926) << std::endl;

  int x = 123;
  std::cout << std::hash<int*>() (&x) << std::endl;
  std::cout << reinterpret_cast<size_t>(&x) << std::endl;
}