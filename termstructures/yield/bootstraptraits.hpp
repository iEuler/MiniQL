#ifndef MINIQL_BOOTSTRAPTRAITS_HPP
#define MINIQL_BOOTSTRAPTRAITS_HPP

#include <cmath>

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

  };

}  // namespace MiniQL

#endif