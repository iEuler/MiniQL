#include <iostream>
#include <memory>
#include <exception>

#include "../math/brent.hpp"
#include "../math/secant.hpp"
#include "../math/solver1d.hpp"

class Myfunc1 {
  public:
    double operator()(double x) const { return (x-11.356)*(x*x - x + 1.0); }
};

int main() {

  // MiniQL::Secant solver_secant;
  // MiniQL::Solver1D<MiniQL::Secant> & solver1 = solver_secant;
  // std::cout << solver1.solve(Myfunc1()) << std::endl;

  try {
    std::shared_ptr<MiniQL::Secant> solver1b = std::make_shared<MiniQL::Secant>();
    std::cout << solver1b->solve(Myfunc1()) << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  std::shared_ptr<MiniQL::Brent> solver2 = std::make_shared<MiniQL::Brent>();
  std::cout << solver2->solve(Myfunc1(), 1e-14, 0.0, -1.0, 15.0) << std::endl;
}