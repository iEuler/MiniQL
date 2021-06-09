#include <iostream>
#include <vector>

template<class Curve>
class BootstrapError {
  public:
    void operator()() { std::cout << a_.c_ << std::endl; }
    Curve a_;    
};

template<class Curve>
class IterativeBootstrap {    
  public:
    void print() { x(); }
    BootstrapError<Curve> x;
};

class Curve {
    friend class IterativeBootstrap<Curve>;
    friend class BootstrapError<Curve>;
    
  protected:
    int c_ = 1;  
};

// Note: this is not working with x.begin() if x is const vector
// typedef std::vector<int>::iterator ITER; 

// correct one
typedef std::vector<int>::const_iterator ITER;

void print(const ITER& it1, const ITER& it2) {
  for (auto it = it1; it != it2; ++it) std::cout << *it << ",";
  std::cout << std::endl;
}

int main() {
  IterativeBootstrap<Curve> b;
  b.print();

  const std::vector<int> x{2,4,5,6,7};
  print(x.begin(), x.begin()+2);
  
}