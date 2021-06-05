#include <cmath>

#include "error.hpp"
#include "interestrate.hpp"
#include "typedef.hpp"

namespace MiniQL {

  Real InterestRate::compoundFactor(Time t) const 
  {
    QL_REQUIRE(t>=0.0, "non negative time (" << t << ") required.");
    Real f = Real(freq_);
    switch (comp_) {
      case Compounding::Simple:
        return 1.0 + r_*t;
      case Compounding::Compounded:
        return std::pow(1+r_/f, t*f);
      case Compounding::Continous:
        return std::exp(r_*t);
      case Compounding::SimpleThenCompounded:
        if (t<=1.0/f)
          return 1.0 + r_*t;
        else
          return std::pow(1+r_/f, t*f);
      default:
        QL_FAIL("unknown Compounding type.");
    }

  }

  InterestRate InterestRate::impliedRate(Rate compoundedRate,
                                         Time t,
                                         const DayCounter& dc = DayCounter(),
                                         Compounding comp = Compounding::Simple,
                                         Frequency freq = Frequency::Annual) 
  {
    QL_REQUIRE(t>=0.0, "non negative time (" << t << ") required.");    
    Rate r;
    Real f = Real(freq);
    if (std::abs(compoundedRate - 1.0) < epsilon) {
      r = 0.0;
    } else {
      switch (comp) {
        case Compounding::Simple:
          r = (compoundedRate - 1.0) / t;
          break;
        case Compounding::Compounded:
          QL_REQUIRE(compoundedRate>=0.0, 
                     "positive compounded interest rate(" << compoundedRate << ") required.");
          r = f * (std::pow(compoundedRate, 1.0/(t*f)) - 1.0);
          break;
        case Compounding::Continous:
          QL_REQUIRE(compoundedRate>=0.0, 
                     "positive compounded interest rate(" << compoundedRate << ") required.");          
          r = std::log(compoundedRate) / t;          
          break;
        case Compounding::SimpleThenCompounded:
          if (t<=1.0/f)
            r = (compoundedRate - 1.0) / t;
          else
            r = f * (std::pow(compoundedRate, 1.0/(t*f)) - 1.0);
          break;
        default:
          QL_FAIL("unknown Compounding type.");
      }
  

    }
    return InterestRate(r, dc, comp, freq);
  }

  std::ostream& operator<<(std::ostream& out, const InterestRate& ir) {
    return out << "interest rate = " << ir.r_;
  }

}  // namespace MiniQL