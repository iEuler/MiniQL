#ifndef MINIQL_ITERATIVEBOOTSTRAP_HPP
#define MINIQL_ITERATIVEBOOTSTRAP_HPP

#include <algorithm>
#include <cmath>
#include <memory>
#include <vector>

#include "bootstraperror.hpp"
#include "bootstraphelper.hpp"
#include "../../error.hpp"
#include "../../typedef.hpp"
#include "../../math/brent.hpp"
#include "../../math/secant.hpp"


// TODO: ipmlement initialize(), setup(), calculate()

namespace MiniQL {

template<class Curve>             // Curve example: PiecewiseYieldCurve<Traits, Interpolator, IterativeBootstrap>
class IterativeBootstrap {    
  public:
    typedef typename Curve::traits_type Traits;               // example: ZeroYield
    typedef typename Curve::interpolator_type Interpolator;

    IterativeBootstrap(Real accuracy = QL_EPSILON,
                       Real minValue = Real(),
                       Real maxValue = Real())
    : accuracy_(accuracy), minValue_(minValue), maxValue_(maxValue) { }
    
    void setup(Curve* ts);
    void calculate() const;

  private:
    void initialize() const;
    Real accuracy_;
    Real minValue_, maxValue_;
    // Size maxAtteampts_;
    // Real maxFactor_;
    // Real minFactor_;
    Curve* ts_ = nullptr;
    Size n_;
    Brent firstSovler_;
    Secant solver_;
    mutable bool initialized_ = false, validCurve_ = false, loopRequired_;
    mutable Size firstAliveHelper_, alive_;
    mutable std::vector<Real> previousData_;    
    mutable std::vector<std::shared_ptr<BootstrapError<Curve>> > errors_;

};

template<class Curve>
void IterativeBootstrap<Curve>::setup(Curve* ts)
{
  ts_ = ts;
  n_ = ts_->instruments_.size();
  QL_REQUIRE(n_>0, "no bootstrap helpers given");
  for (Size j=0; j<n_; ++j)
    ts_->registerWith(ts_->instruments_[j]);
}

template<class Curve>
void IterativeBootstrap<Curve>::initialize() const
{
  //ensure helpers are sorted
  std::sort(ts_->instruments_.begin(), ts_->instruments_.end(), BootstrapHelperSorter());
  
  // skip expired helpers
  Date firstDate = Traits::initialDate(ts_);
  QL_REQUIRE(ts_->instruments_.back()->maturityDate() > firstDate,
             "all instruments expired");
  firstAliveHelper_ = 0,
  while (ts_->instruments_[firstAliveHelper_]->maturityDate() <= firstDate)
    ++ firstAliveHelper_;
  alive_ = n_ - firstAliveHelper_;
  // TODO: check alive_ > Interpolator::requiredPoints 

  // calculate dates and times, create errros_
  std::vector<Date>& dates = ts_->dates_;
  std::vector<Time>& times = ts_->times_;
  dates.resize(alive_+1);
  times.resize(alive_+1);
  errors_.resize(alive_+1);
  dates[0] = firstDate;
  times[0] = ts_->timeFromReference(dates[0]);

  for (Size i=1, j=firstAliveHelper_; j<n_; ++i, ++j) 
  {
    const std::shared_ptr<typename Traits::helper>& helper = ts_->instruments_[j];
    dates[i] = helper->maturityDate();
    times[i] = ts_->timeFromReference(dates[i]);
    QL_REQUIRE(std::abs(dates[i] - dates[i-1])>QL_EPSILON, 
               "more than one instrument with maturity " << dates[i]); 
    errors_[i] = std::make_shared<BootstrapError<Curve>>(ts_, helper, i);
  }

  initialized_ = true;
}

template<class Curve>
void IterativeBootstrap<Curve>::calculate() const
{
  if (!initialized_)
    initialize();
  
  // setup helpers
  for (Size j=firstAliveHelper_; j<n_; ++j) {
    const std::shared_ptr<typename Traits::helper>& helper = ts_->instruments_[j];
    helper->setTermStructure(const_cast<Curve*>(ts_));
    // ts_ of type Curve = PiecewiseYieldCurve<Traits, Interpolator, IterativeBootstrap>
    // helper is of type BootstrapHelper<YieldTermStructure>
  }

  const std::vector<Time>& times = ts_->times_;
  const std::vector<Real>& data = ts_->data_;
  Real accuracy = accuracy_;
  Size maxIterations = MAX_FUNCTION_EVALUATIONS;

  // there might be a valid curve state to use as guess
  bool validData = validCurve_;

  for (Size iteration=0; /*no end condition*/; ++iteration) 
  {
    previousData_ = ts_->data_;

    for (Size i=1; i<=alive_; ++i)
    {
      Real guess = Traits::guess(i, ts_);
      if (!validatData) 
      {
        ts_->interpolation_ = ts_->interpolator_.interpolate(times.begin(),
                                                              times.begin()+i+1,
                                                              data.begin());
        ts_->interpolation_.update();
      }

      // TODO
      solver_.solve(*errors_[i], accuracy, guess, min, max);
      
    }

    // exit condition
    Real change = std::abs(data[1] - previousData_[1]);
    for (Size i=2; i<=alive_; ++i)
      change = std:max(change, std::abs(data[i] - previousData_[i]));
    if (change < accuracy)
      break;
    
    if (iteration >= maxIterations) {
      QL_FAIL("convergence not reached.");
    }

    validData = true;
  }

  validCurve_ = true;
}

}  // namespace MiniQL

#endif