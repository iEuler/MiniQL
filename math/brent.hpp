#ifndef MINIQL_BRENT_HPP
#define MINIQL_BRENT_HPP

#include <cmath>

#include "../error.hpp"
#include "../typedef.hpp"
#include "solver1d.hpp"

namespace MiniQL {
  
class Brent : public Solver1D<Brent> {
  public:
    Brent() = default;
    ~Brent() = default;

    template<class F>
    Real solveImpl(const F& f, Real xAccuracy) const;
  
  private:
    Real sign(Real a, Real b) const {
      return b>=0.0 ? std::abs(a) : -std::abs(a);
    }
   
};

template<class F>
inline Real Brent::solveImpl(const F& f, Real xAccuracy) const {
  Real min1, min2;
  Real froot, p, q, r, s, xAcc1, xMid;

  // we want to start with root_ (which equals the guess) on
  // one side of the bracket and both xMin_ and xMax_ on the
  // other.
  froot = f(root_);
  ++evaluationNumber_;
  if (froot * fxMin_ < 0) {
      xMax_ = xMin_;
      fxMax_ = fxMin_;
  } else {
      xMin_ = xMax_;
      fxMin_ = fxMax_;
  }
  Real d = root_- xMax_;
  Real e = d;

  while (evaluationNumber_<=maxEvaluations_) {
      if ((froot > 0.0 && fxMax_ > 0.0) ||
          (froot < 0.0 && fxMax_ < 0.0)) {

          // Rename xMin_, root_, xMax_ and adjust bounds
          xMax_=xMin_;
          fxMax_=fxMin_;
          e=d=root_-xMin_;
      }
      if (std::fabs(fxMax_) < std::fabs(froot)) {
          xMin_=root_;
          root_=xMax_;
          xMax_=xMin_;
          fxMin_=froot;
          froot=fxMax_;
          fxMax_=fxMin_;
      }
      // Convergence check
      xAcc1=2.0*QL_EPSILON*std::fabs(root_)+0.5*xAccuracy;
      xMid=(xMax_-root_)/2.0;
      if (std::fabs(xMid) <= xAcc1 || (close(froot, 0.0))) {
          f(root_);
          ++evaluationNumber_;
          return root_;
      }
      if (std::fabs(e) >= xAcc1 &&
          std::fabs(fxMin_) > std::fabs(froot)) {

          // Attempt inverse quadratic interpolation
          s=froot/fxMin_;
          if (close(xMin_,xMax_)) {
              p=2.0*xMid*s;
              q=1.0-s;
          } else {
              q=fxMin_/fxMax_;
              r=froot/fxMax_;
              p=s*(2.0*xMid*q*(q-r)-(root_-xMin_)*(r-1.0));
              q=(q-1.0)*(r-1.0)*(s-1.0);
          }
          if (p > 0.0) q = -q;  // Check whether in bounds
          p=std::fabs(p);
          min1=3.0*xMid*q-std::fabs(xAcc1*q);
          min2=std::fabs(e*q);
          if (2.0*p < (min1 < min2 ? min1 : min2)) {
              e=d;                // Accept interpolation
              d=p/q;
          } else {
              d=xMid;  // Interpolation failed, use bisection
              e=d;
          }
      } else {
          // Bounds decreasing too slowly, use bisection
          d=xMid;
          e=d;
      }
      xMin_=root_;
      fxMin_=froot;
      if (std::fabs(d) > xAcc1)
          root_ += d;
      else
          root_ += sign(xAcc1,xMid);
      froot=f(root_);
      ++evaluationNumber_;
  }
}
  
}  // namespace MiniQL

#endif