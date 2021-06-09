#ifndef MINIQL_BOOTSTRAPTRAITS_HPP
#define MINIQL_BOOTSTRAPTRAITS_HPP

#include <vector>

#include "zerocurve.hpp"
#include "../bootstraphelper.hpp"
#include "../interpolatedcurve.hpp"
#include "../yieldtermstructure.hpp"
#include "../../error.hpp"
#include "../../interestrate.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"


namespace MiniQL {

  // Zero-curve traits
  struct ZeroYield {
    // interpolated curve type
    template<class Interpolator>
    struct curve {
      typedef InterpolatedZeroCurve<Interpolator> type;
    };
    // helper class
    typedef BootstrapHelper<YieldTermStructure> helper;

    // start of curve data
    static Date initialDate(const YieldTermStructure* c) {
      return c->referenceDate();
    }

    // guesses
    // TODO: note this function has a different logic to the one in QuantLib
    template <class C>
    static Real guess(Size i, const C* c)
    {
      if (i==0) return 0.0;
      return c->data()[i-1];
    }   

    static void updateGuess(std::vector<Real>& data, Real rate, Size i)
    {
      data[i] = rate;
      if (i==1)
        data[0] = rate; 
      // for zero yield, the first data (corresponding to maturity = 0) is 
      // same as the first zero yield with non-zero maturity
    }  

  };

}  // namespace MiniQL

#endif