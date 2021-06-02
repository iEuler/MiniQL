#ifndef MINIQL_TYPEDEF_HPP
#define MINIQL_TYPEDEF_HPP

namespace MiniQL {
  // typedef double Date;
  typedef double Real;
  typedef double Rate;
  typedef double Time;

  enum OptionType {
    Put = -1, Call = 1
  };

  enum Compounding {
    Simple,                 // 1+rT
    Compounded,             // (1+r)^T
    Continous,              // e^{rT}
    SimpleThenCompounded
  };
  
}  // namespace MiniQL

#endif