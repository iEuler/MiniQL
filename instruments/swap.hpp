#ifndef MINIQL_SWAP_HPP
#define MINIQL_SWAP_HPP

#include <memory>
#include <vector>
#include <cmath>

#include "../handle.hpp"
#include "../instrument.hpp"
#include "../patterns/observable.hpp"
#include "../time/date.hpp"
#include "../typedef.hpp"

namespace MiniQL {  

class CashFlow : public Observable {
  public:
    CashFlow() = default;
    CashFlow(const Date& date, const Real& amount) : date_(date), amount_(amount) {}
    ~CashFlow() = default;

    bool hasOccured(Date settlement) const { return (settlement > date_); }
    Date date() const { return date_; }
    Real amount() const { return amount_; }
  private:
    Date date_ = 0;
    Real amount_ = 0.0;
};

class FlatTermStructure : public Observable {
  public:
    FlatTermStructure() = default;
    FlatTermStructure(const Date& referenceDate, const Real& interestRate) 
        : referenceDate_(referenceDate), interestRate_(interestRate) {}
    ~FlatTermStructure() = default;
    
    Real interestRate() const { return interestRate_; }
    Date referenceDate() const { return referenceDate_; }
  private:
    Date referenceDate_ = 0;
    Real interestRate_ = 0.0;
};


class Swap : public Instrument {
  public:
    typedef std::vector<std::shared_ptr<CashFlow>> Leg;
    // simplified version. Need to redefine Handle class
    // typedef std::shared_ptr<FlatTermStructure> Handle; 
    Swap() = default;
    // Swap(const Leg&, const Leg&, const Handle&);
    Swap(const Leg&, const Leg&, const Handle<FlatTermStructure>&);
    ~Swap() = default;

    bool isExpired() const override;

    // Real firstLegBPS() const;
    // Real secondLegBPS() const;
    // int numberOfLegs() const { return 2; }
    // Date startDate() const;
    // Date maturityDate() const;
  protected:
    void performCalculations() const override;
    void setupExpired() const override;
    Leg firstLeg_, secondLeg_;
    Handle<FlatTermStructure> termStructure_;
    // mutable Real firstLegBPS_, secondLegBPS_;
};

inline Swap::Swap(const Leg& firstLeg, 
                  const Leg& secondLeg, 
                  const Handle<FlatTermStructure>& termStructure) 
: firstLeg_(firstLeg), secondLeg_(secondLeg), termStructure_(termStructure)
{
  // The registerWith works on Handle, because there is an automatic conversion 
  // to shared_ptr<Observable>. 
  registerWith(termStructure_);
  for (auto &pcf : firstLeg_)
    registerWith(pcf);
  for (auto &pcf : secondLeg_)
    registerWith(pcf);
}


}  // namespace MiniQL

#endif 
