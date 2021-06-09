#ifndef MINIQL_INTERPOLATEDCURVE_HPP
#define MINIQL_INTERPOLATEDCURVE_HPP

#include <cmath>
#include <vector>

#include "../error.hpp"
#include "../interestrate.hpp"
#include "../termstructure.hpp"
#include "../typedef.hpp"
#include "../time/date.hpp"
#include "../math/interpolation.hpp"

namespace MiniQL {

template<class Interpolator>
class InterpolatedCurve {
  public:
    // InterpolatedCurve() = default;
    
    // standard constructor
    InterpolatedCurve(std::vector<Time> times,
                      std::vector<Real> data,
                      const Interpolator& i = Interpolator())
    : times_(std::move(times)), data_(std::move(data)), interpolator_(i) { }
    // {
    //   setupInterpolation();
    // }
    
    // Note the constructor below is also default constructor
    InterpolatedCurve(const Interpolator& i = Interpolator())
    : interpolator_(i) { }
    
    // copy constructor
    InterpolatedCurve(const InterpolatedCurve& c)
    : times_(c.times_), data_(c.data_), interpolator_(c.interpolator_) 
    {
      setupInterpolation();
    }
    // copy assignment
    InterpolatedCurve& operator=(const InterpolatedCurve& c)
    {
      if (this != &c) 
      {
        times_ = c.times_;
        data_ = c.data_;
        interpolator_ = c.interpolator_;
        setupInterpolation();
      }      
      return *this;
    }
    ~InterpolatedCurve() = default;

    void setupInterpolation() {
      interpolation_ = interpolator_.interpolate(times_.begin(), 
                                                 times_.end(), 
                                                 data_.begin());
    }
  
  protected:
    mutable std::vector<Time> times_;
    mutable std::vector<Real> data_;
    mutable Interpolation interpolation_;
    Interpolator interpolator_;
    // Date maxDate_;
};

}  // namespace MiniQL

#endif