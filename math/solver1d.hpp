
/*! \file solver1d.hpp
    \brief 1D root solver class
*/

#ifndef MINIQL_SOLVER1D_HPP
#define MINIQL_SOLVER1D_HPP

#include <cmath>
#include <iostream>

#include "../typedef.hpp"
#include "../error.hpp"
#include "../patterns/curiouslyrecurring.hpp"

namespace MiniQL {

constexpr Size MAX_FUNCTION_EVALUATIONS = 100;

template<class Impl>
class Solver1D : public CuriouslyRecurringTemplate<Impl> {
  public:
    Solver1D() = default;

    template<class F>
    Real solve(const F& f,
               Real accuracy = QL_EPSILON,
               Real guess = 0.0,
               Real xMin = 0.0,
               Real xMax = 0.0) const;

    void setMaxEvaluations(Size evaluations) { maxEvaluations_ = evaluations; }
    void setLowerBound(Size lb) { 
      lowerBound_ = lb;
      lowerBoundEnforced_ = true;
    }
    void setUpperBound(Size ub) { 
      upperBound_ = ub;
      upperBoundEnforced_ = true;
    }
    
    bool close(Real x1, Real x2) const {
      return std::abs(x1-x2) < QL_EPSILON;
    }

  protected:
    mutable Real root_, xMin_, xMax_, fxMin_, fxMax_;
    Size maxEvaluations_ = MAX_FUNCTION_EVALUATIONS;
    mutable Size evaluationNumber_;

  private:
    Real enforceBounds_(Real x) const {
      if (lowerBoundEnforced_ && x < lowerBound_)
        return lowerBound_;
      if (upperBoundEnforced_ && x > upperBound_)
        return upperBound_;
      return x;
    }
    Real lowerBound_, upperBound_;
    bool lowerBoundEnforced_ = false, upperBoundEnforced_ = false;
};

template<class Impl>
template<class F>
inline Real Solver1D<Impl>::solve(const F& f,
                                  Real accuracy,
                                  Real guess,
                                  Real xMin,
                                  Real xMax) const
{
  accuracy = std::max(accuracy, QL_EPSILON);
  xMin_ = xMin;
  xMax_ = xMax;
  if (close(xMin, xMax)) {
    xMin_ -= 1.0;
    xMax_ += 1.0;
  }  
  // std::cout << xMin_ << ", " << xMax_ << std::endl;

  QL_REQUIRE(xMin_ <= xMax_, "invalid range: xMin (" << xMin_ << ") > xMax (" << xMax_ << ")" );
  QL_REQUIRE(!lowerBoundEnforced_ || xMin_ >= lowerBound_ , 
             "xMin (" << xMin_ << ") < enforced lower bound (" << lowerBound_ << ")" );
  QL_REQUIRE(!upperBoundEnforced_ || xMax_ <= upperBound_ , 
             "xMax (" << xMax_ << ") > enforced upper bound (" << upperBound_ << ")" );
  fxMin_ = f(xMin_);
  if (std::abs(fxMin_) < accuracy)
    return xMin_;
  fxMax_ = f(xMax_);
  if (std::abs(fxMax_) < accuracy)
    return xMax_;
  
  evaluationNumber_ = 2;
  
  if (lowerBoundEnforced_&& upperBoundEnforced_) {
    QL_REQUIRE(fxMin_*fxMax_ < 0, 
              "root not bracketed: f[" << xMin_ << ", " << xMax_ << "] -> [" 
              << fxMin_ << ", " << fxMax_ << "]");
    QL_REQUIRE(guess > xMin_ && guess < xMax_,
              "guess (" << guess << ") is not in [xMin, xMax] = [" << xMin_ << ", " << xMax_ << "]");
  }

  root_ = guess;  
  
  return this->impl().solveImpl(f, accuracy);

}

}  // namespace MiniQL

#endif 
