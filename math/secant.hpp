#ifndef MINIQL_SECANT_HPP
#define MINIQL_SECANT_HPP

#include "../error.hpp"
#include "../typedef.hpp"
#include "solver1d.hpp"

namespace MiniQL {
  
class Secant : public Solver1D<Secant> {
  public:
    Secant() = default;
    ~Secant() = default;

    template<class F>
    Real solveImpl(const F& f, Real xAccuracy) const;
  
  private:
    Real sign(Real a, Real b) const {
      return b>=0.0 ? std::abs(a) : -std::abs(a);
    }
    bool close(Real x1, Real x2) const {
      return std::abs(x1-x2) < QL_EPSILON;
    }

};

template<class F>
inline Real Secant::solveImpl(const F& f, Real xAccuracy) const {
  Real fl, froot, dx, xl;

  // Pick the bound with the smaller function value
  // as the most recent guess
  if (std::fabs(fxMin_) < std::fabs(fxMax_)) {
      root_ = xMin_;
      froot = fxMin_;
      xl = xMax_;
      fl = fxMax_;
  } else {
      root_ = xMax_;
      froot = fxMax_;
      xl = xMin_;
      fl = fxMin_;
  }
  while (evaluationNumber_<=maxEvaluations_) {
      dx = (xl-root_)*froot/(froot-fl);
      xl = root_;
      fl = froot;
      root_ += dx;
      froot = f(root_);
      ++evaluationNumber_;
      if (std::fabs(dx) < xAccuracy || (close(froot, 0.0)))
          return root_;
  }
}
  
}  // namespace MiniQL

#endif