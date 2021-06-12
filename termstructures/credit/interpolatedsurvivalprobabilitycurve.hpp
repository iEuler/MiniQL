#ifndef MINIQL_INTERPOLATEDSURVIVALPROBABILITYCURVE_HPP
#define MINIQL_INTERPOLATEDSURVIVALPROBABILITYCURVE_HPP

#include <cmath>
#include <vector>
#include <iostream>

#include "../../error.hpp"
#include "../interpolatedcurve.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"
#include "../../math/linearinterpolation.hpp"
#include "survivalprobabilitystructure.hpp"

namespace MiniQL {

template<class Interpolator>
class InterpolatedSurvivalProbabilityCurve 
  : public SurvivalProbabilityStructure,
    public InterpolatedCurve<Interpolator> {

  public:
    InterpolatedSurvivalProbabilityCurve() = default;
    InterpolatedSurvivalProbabilityCurve(
        const std::vector<Date>& dates,
        const std::vector<Probability>& probabilities,
        const Calendar& calendar = Calendar(),
        const DayCounter& dc = DayCounter(),
        const Interpolator& interpolator = Interpolator());      
    InterpolatedSurvivalProbabilityCurve(
        const Date& referenceDate,
        const Calendar& calendar = Calendar(),
        const DayCounter& dc = DayCounter(),
        const Interpolator& interpolator = Interpolator())
      : SurvivalProbabilityStructure(referenceDate, calendar, dc),
        InterpolatedCurve<Interpolator>(interpolator) {}
    ~InterpolatedSurvivalProbabilityCurve() = default;

    const std::vector<Date>& dates() const { return dates_; }
    const std::vector<Time>& times() const { return this->times_; }
    const std::vector<Real>& data() const { return this->data_; }
    const std::vector<Probability>& survivalProbabilities() const { return this->data_; }

    Rate survivalProbabilityImpl(Time t) const override 
    { 
      if (t <= this->times_.back())
        return this->interpolation_(t); 
      
      // flat hazard rate extrapolation
      Time tMax = this->times_.back();
      Probability sMax = this->data_.back();
      Rate negHazardMax = this->interpolation_.derivative(tMax) / sMax;
      return sMax * std::exp( negHazardMax * (t-tMax) );
    }

  protected:
    mutable std::vector<Date> dates_;

  private:
    void initialize(const Compounding& comp = Compounding::Simple,
                    const Frequency& frequency = Frequency::Annual);

};


template<class Interpolator>
inline InterpolatedSurvivalProbabilityCurve<Interpolator>::InterpolatedSurvivalProbabilityCurve(
    const std::vector<Date>& dates,
    const std::vector<Probability>& probabilities,
    const Calendar& calendar,
    const DayCounter& dc,
    const Interpolator& interpolator)
: SurvivalProbabilityStructure(dates.front(), calendar, dc),
  InterpolatedCurve<Interpolator>(std::vector<Time>(), probabilities, interpolator),
  dates_(dates)
{
  initialize();
}

template<class Interpolator>
void InterpolatedSurvivalProbabilityCurve<Interpolator>::initialize(
  const Compounding& comp,
  const Frequency& frequency)
{
  this->times_.resize(dates_.size());
  this->times_[0] = 0.0;
  for (Size i=1; i<dates_.size(); ++i) 
    this->times_[i] = this->dayCounter_.yearFraction(dates_[0], dates_[i]);      
  
  
  // need to rebuild interpolation_, which depends on times_
  this->interpolation_ = this->interpolator_.interpolate(this->times_.begin(),
                                                         this->times_.end(),
                                                         this->data_.begin());
  this->interpolation_.update();  // update the slope in interpolation implementation
}


}  // namespace MiniQL

#endif