#ifndef MINIQL_TYPEDEF_HPP
#define MINIQL_TYPEDEF_HPP

namespace MiniQL {
  

  // typedef double Date;
  typedef double Real;
  typedef double Rate;
  typedef double Time;
  typedef double DiscountFactor;
  typedef std::size_t Size;

  constexpr Real QL_EPSILON = 1e-14;
  

  enum class OptionType {
    Put = -1, Call = 1
  };

  enum class Compounding {
    Simple,                 // 1+rT
    Compounded,             // (1+r)^T
    Continuous,              // e^{rT}
    SimpleThenCompounded
  };

  enum class Frequency {
    NoFrequency = -1,       // null frequency
    Once = 0,               // only once, e.g. a zero-coupon
    Annual = 1,             // once a year
    Semiannual = 1,         // twice a year
    Quarterly = 4,          // every third month
    Monthly = 12,           // once a month
  };
  
}  // namespace MiniQL

#endif