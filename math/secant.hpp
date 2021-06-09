#ifndef MINIQL_SECANT_HPP
#define MINIQL_SECANT_HPP

#include <cmath>
#include <iostream>

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
    
};

template<class F>
inline Real Secant::solveImpl(const F& f, Real xAccuracy) const {
  Real fl, froot, dx, xl;
  
  // Pick the bound with the smaller function value
  // as the most recent guess
  // if (std::fabs(fxMin_) < std::fabs(fxMax_)) {
  //     root_ = xMin_;
  //     froot = fxMin_;
  //     xl = xMax_;
  //     fl = fxMax_;
  // } else {
  //     root_ = xMax_;
  //     froot = fxMax_;
  //     xl = xMin_;
  //     fl = fxMin_;
  // }

  // use newton's method for first step
  xl = root_;
  fl = f(root_);
  Real slope = ( f(root_ + 0.0001) - fl ) / 0.0001;
  if (std::abs(slope) < 1e-4) 
    slope = slope >= 0.0? 1.0 : -1.0;
  root_ = root_ - fl / slope;
  froot = f(root_);
  
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
  QL_FAIL("Secant method is not convergent in " << evaluationNumber_ << " steps.");
}
  
}  // namespace MiniQL

#endif