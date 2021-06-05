#ifndef MINIQL_LINEARINTERPOLATION_HPP
#define MINIQL_LINEARINTERPOLATION_HPP

#include <memory>
#include <vector>

#include "../typedef.hpp"
#include "interpolation.hpp"

namespace MiniQL {

class LinearInterpolationImpl : public Interpolation::Impl {
  public:
    LinearInterpolationImpl(const Iter& xBegin, const Iter& xEnd, const Iter& yBegin)
    : Interpolation::Impl(xBegin, xEnd, yBegin),
      s_(xEnd - xBegin) { }
      // primitiveConst_(xEnd - xBegin), 
      
    void update() override 
    {
      Size xLen = xEnd_-xBegin_;
      for (Size i=0; i < (xLen-1); ++i) 
        s_[i] = (yBegin_[i+1] - yBegin_[i]) / (xBegin_[i+1] - xBegin_[i]);
      s_[xLen-1] = s_[xLen-2];
    }

    Real value(Real x) const override 
    {
      Size i = locate(x) - xBegin_;
      return yBegin_[i] + s_[i] * (x - xBegin_[i]);
    }
    // Real primitive(Real) const override;
    Real derivative(Real x) const override
    {
      Size i = locate(x) - xBegin_;
      return s_[i];
    }
    Real secondDerivative(Real) const override { return 0.0; }

  private:
    std::vector<Real> s_;  // s_[i] is the slope in interval i
    // std::vector<Real> primitiveConst_;
};

class LinearInterpolation : public Interpolation {    
  public:
    typedef std::vector<Real>::iterator Iter;

    LinearInterpolation(const Iter& xBegin, const Iter& xEnd, const Iter& yBegin)
    {
      impl_ = std::make_shared<LinearInterpolationImpl>(xBegin, xEnd, yBegin);
      impl_->update();
    }
    
};

// Interpolator
class Linear {
  public:
    typedef std::vector<Real>::iterator Iter;
    Interpolation interpolate(const Iter& xBegin, const Iter& xEnd, const Iter& yBegin) const
    {
      return LinearInterpolation(xBegin, xEnd, yBegin);
    }
    static const bool glob = false;
    static const Size requiredPoints = 2;
};
  
  
}  // namespace MiniQL

#endif