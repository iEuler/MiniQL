#ifndef MINIQL_INTERESTRATE_HPP
#define MINIQL_INTERESTRATE_HPP

#include "time/date.hpp"
#include "typedef.hpp"
#include "patterns/observable.hpp"

namespace MiniQL {

class InterestRate {
    friend std::ostream& operator<<(std::ostream&, const InterestRate&);
  public:
    InterestRate() = default;
    InterestRate(Rate r, DayCounter dc, Compounding comp, Frequency freq)
      : r_(r), dc_(dc), comp_(comp), freq_(freq) {}
    ~InterestRate() = default;

    operator Rate() const { return r_; }

    Rate rate() const { return r_; }
    const DayCounter& dayCounter() const { return dc_; }
    const Compounding& compounding() const { return comp_; }
    const Frequency& frequency() const { return freq_; }

    Real compoundFactor(Time t) const;

    Real compoundFactor(const Date& d1,
                        const Date& d2) const {
      Time t = dc_.yearFraction(d1, d2);
      return compoundFactor(t);
    }

    DiscountFactor discountFactor(Time t) const { return 1.0/compoundFactor(t); }

    DiscountFactor discountFactor(const Date& d1,
                                  const Date& d2) const {
      Time t = dc_.yearFraction(d1, d2);
      return discountFactor(t);
    }

    static InterestRate impliedRate(Real compoundedRate,
                                    Time t,
                                    const DayCounter& dc = DayCounter(),
                                    Compounding comp = Compounding::Simple,
                                    Frequency freq = Frequency::Annual);

    static InterestRate impliedRate(Real compoundedRate,
                                    const Date& d1,
                                    const Date& d2,
                                    const DayCounter& dc = DayCounter(),
                                    Compounding comp = Compounding::Simple,
                                    Frequency freq = Frequency::Annual) {
      Time t = dc.yearFraction(d1, d2);
      return impliedRate(compoundedRate, t, dc, comp, freq);
    }



  private:
    Rate r_ = Rate();
    DayCounter dc_;
    Compounding comp_;
    Frequency freq_;
};

}  // namespace MiniQL

#endif