#ifndef MINIQL_INTERPOLATION_ORIGINAL_HPP
#define MINIQL_INTERPOLATION_ORIGINAL_HPP

#include <algorithm>
#include <memory>
#include <vector>

#include "../typedef.hpp"

namespace MiniQL {

class Interpolation {
  protected:
    class Impl {
      public:
        virtual ~Impl() = default;
        virtual void update() = 0;
        virtual std::vector<Real> xValues() const = 0;
        virtual std::vector<Real> yValues() const = 0;
        virtual Real value(Real) const = 0;
        virtual Real primitive(Real) const = 0;
        virtual Real derivative(Real) const = 0;
        virtual Real secondDerivative(Real) const = 0;
    };
    std::shared_ptr<Impl> impl_;
  
  public:    
    class TemplateImpl : public Impl {
      public:
        typedef std::vector<Real>::iterator Iter;

        TemplateImpl(const Iter& xBegin, const Iter& xEnd, const Iter& yBegin)
        : xBegin_(xBegin), xEnd_(xEnd), yBegin_(yBegin) { }
        
        std::vector<Real> xValues() const override { return std::vector<Real>(xBegin_, xEnd_); }
        std::vector<Real> yValues() const override { 
          return std::vector<Real>(yBegin_, yBegin_ + (xEnd_ - xBegin_)); 
        }
        Iter locate(Real x) const {
          if (x < *xBegin_)
            return xBegin_;
          else if (x >= *(xEnd_-1))
            return xEnd_-2;
          else
            return std::lower_bound(xBegin_, xEnd_, x);
        }
                
      protected:
        Iter xBegin_, xEnd_, yBegin_;        
    };

    Interpolation() = default;
    ~Interpolation() = default;

    Real operator()(Real x) const { return impl_->value(x); }
    Real primitive(Real x) const { return impl_->primitive(x); }
    Real derivative(Real x) const { return impl_->derivative(x); }
    Real secondDerivative(Real x) const { return impl_->secondDerivative(x); }
    void update() { impl_->update(); }

};
  
  
}  // namespace MiniQL

#endif