#ifndef MINIQL_ZEROCURVE_HPP
#define MINIQL_ZEROCURVE_HPP

#include <cmath>
#include <vector>
#include <iostream>

#include "../../error.hpp"
#include "../../interestrate.hpp"
#include "../interpolatedcurve.hpp"
#include "../yieldtermstructure.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"
#include "../../math/linearinterpolation.hpp"
#include "zeroyieldstructure.hpp"

namespace MiniQL {

template<class Interpolator>
class InterpolatedZeroCurve : public ZeroYieldStructure,
                              public InterpolatedCurve<Interpolator> {
  public:
    InterpolatedZeroCurve() = default;
    InterpolatedZeroCurve(const std::vector<Date>& dates,
                          const std::vector<Rate>& yields,
                          const Calendar& calendar = Calendar(),
                          const DayCounter& dc = DayCounter(),
                          const Interpolator& interpolator = Interpolator());      
    InterpolatedZeroCurve(const Date& referenceDate,
                          const Calendar& calendar = Calendar(),
                          const DayCounter& dc = DayCounter(),
                          const Interpolator& interpolator = Interpolator())
    : ZeroYieldStructure(referenceDate, calendar, dc),
      InterpolatedCurve<Interpolator>(interpolator) {}

    ~InterpolatedZeroCurve() override = default;

    // const Date& maxDate() const overrride { return dates_.back(); }

    const std::vector<Date>& dates() const { return dates_; }
    const std::vector<Time>& times() const { return this->times_; }
    const std::vector<Real>& data() const { return this->data_; }
    const std::vector<Real>& zeroRates() const { return this->data_; }

    Rate zeroYieldImpl(Time t) const override { return this->interpolation_(t); }

  protected:    
    mutable std::vector<Date> dates_;
  
  private:
    void initialize(const Compounding& comp = Compounding::Simple,
                    const Frequency& frequency = Frequency::Annual);
};

template<class Interpolator>
inline InterpolatedZeroCurve<Interpolator>::InterpolatedZeroCurve(
    const std::vector<Date>& dates,
    const std::vector<Rate>& yields,
    const Calendar& calendar,
    const DayCounter& dc,
    const Interpolator& interpolator)
: ZeroYieldStructure(dates.front(), calendar, dc),
  InterpolatedCurve<Interpolator>(std::vector<Time>(), yields, interpolator),
  dates_(dates)
{
  initialize();
}

template<class Interpolator>
void InterpolatedZeroCurve<Interpolator>::initialize(
  const Compounding& comp,
  const Frequency& frequency)
{
  this->times_.resize(dates_.size());
  for (Size i=1; i<dates_.size(); ++i) 
    this->times_[i] = this->dayCounter_.yearFraction(dates_[0], dates_[i]);      
  
  std::cout << "flag 1" << std::endl;
  for (auto x : this->times_) std::cout << x << ", ";
  std::cout << std::endl;

  if (comp != Compounding::Continous) {    
    this->times_[0] = 1.0/365;
    for (Size i=0; i<dates_.size(); ++i) 
    {
      InterestRate r(this->data_[i], this->dayCounter_, comp, frequency);
      std::cout << r << ", " << this->times_[i] << std::endl;
      this->data_[i] = r.equivalentRate(this->times_[i], this->dayCounter_, comp, frequency);  
    }
    this->times_[0] = 0.0;
  }

  std::cout << "flag 2" << std::endl;

  // need to rebuild interpolation_, which depends on times_
  this->interpolation_ = this->interpolator_.interpolate(this->times_.begin(),
                                                         this->times_.end(),
                                                         this->data_.begin());
  this->interpolation_.update();  // update the slope in interpolation implementation
}


}  // namespace MiniQL

#endif