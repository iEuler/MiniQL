#ifndef MINIQL_PIECEWISEYIELDCURVE_HPP
#define MINIQL_PIECEWISEYIELDCURVE_HPP

#include <iostream>
#include <memory>
#include <vector>

#include "bootstraptraits.hpp"
#include "../bootstraperror.hpp"
#include "../iterativebootstrap.hpp"
#include "../../patterns/lazyobject.hpp"


namespace MiniQL {

template<class Traits, class Interpolator,
         template<class> class Bootstrap = IterativeBootstrap>
class PiecewiseYieldCurve 
  : public Traits::template curve<Interpolator>::type,    // InterpolatedZeroCurve<Interpolator>
    public LazyObject 
{
  
  public:
    typedef typename Traits::template curve<Interpolator>::type base_curve;
    typedef PiecewiseYieldCurve<Traits, Interpolator, Bootstrap> this_curve;
    typedef Traits traits_type;
    typedef Interpolator interpolator_type;
    typedef Bootstrap<this_curve> bootstrap_type;     
    // base_curve = InterpolatedZeroCurve<Interpolator>
    // this_curve = PiecewiseYieldCurve<Traits, Interpolator, IterativeBootstrap>
    // bootstrap_type = IterativeBootstrap<PiecewiseYieldCurve<Traits, Interpolator, IterativeBootstrap>>

    friend class Bootstrap<this_curve>;
    friend class BootstrapError<this_curve> ;

    PiecewiseYieldCurve(
        const Date& referenceDate,
        std::vector<std::shared_ptr<typename Traits::helper>> instruments,
        const DayCounter& dayCounter = DayCounter(),
        const Interpolator i = Interpolator(),
        bootstrap_type bootstrap = bootstrap_type() )
    : base_curve(referenceDate, Calendar(), dayCounter, i),
      instruments_(std::move(instruments)),
      accuracy_(QL_EPSILON),
      bootstrap_(bootstrap)
    {
      bootstrap_.setup(this);
    }

    // const Date& maxDate() const override;
    const std::vector<Date>& dates() const;
    const std::vector<Time>& times() const;
    const std::vector<Real>& data() const;

    // Note: update() must be defined in this class, since it has been defined
    // in both base classes: InterpolatedZeroCurve<Interpolator> and LazyObejct
    void update() override { LazyObject::update(); }  

  private:
    void performCalculations() const override;
    DiscountFactor discountImpl(Time) const override;
    std::vector<std::shared_ptr<typename Traits::helper> > instruments_;
    // *instruments_[i] is of type BootstrapHelper<YieldTermStructure>
    Real accuracy_;
    Bootstrap<this_curve> bootstrap_;

};

// template<class C, class I, template<class> class B> 
// inline const Date& PiecewiseYieldCurve<C,I,B>::maxDate() const 
// { 
//   calculate();
//   return base_curve::maxDate(); 
// }

template<class C, class I, template<class> class B> 
inline const std::vector<Date>& PiecewiseYieldCurve<C,I,B>::dates() const 
{ 
  calculate();
  return base_curve::dates(); 
}

template<class C, class I, template<class> class B> 
inline const std::vector<Time>& PiecewiseYieldCurve<C,I,B>::times() const 
{ 
  calculate();
  return base_curve::times(); 
}

template<class C, class I, template<class> class B> 
inline const std::vector<Real>& PiecewiseYieldCurve<C,I,B>::data() const 
{ 
  calculate();
  return base_curve::data(); 
}

template<class C, class I, template<class> class B> 
inline DiscountFactor PiecewiseYieldCurve<C,I,B>::discountImpl(Time t) const
{ 
  calculate();
  return base_curve::discountImpl(t); 
}

template<class C, class I, template<class> class B>
inline void PiecewiseYieldCurve<C,I,B>::performCalculations() const
{
  // delegate to bootstrapper
  bootstrap_.calculate();  
}
    

}  // namespace MiniQL

#endif