#ifndef MINIQL_RATEHELPER_HPP
#define MINIQL_RATEHELPER_HPP


#include "../../error.hpp"
#include "../../interestrate.hpp"
#include "../yieldtermstructure.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"

#include "../../quote.hpp"
#include "../bootstraphelper.hpp"
#include "../handle.hpp"


namespace MiniQL {

typedef BootstrapHelper<YieldTermStructure> RateHelper;

class FraRateHelper : public RateHelper {
  public:
    FraRateHelper(const Handle<Quote>& rate,
                  Date startDate)
    : RateHelper(rate), startDate_(startDate) 
    {
      initializeDates();
    }

    FraRateHelper(Rate rate,
                  Date startDate)
    : RateHelper(rate), startDate_(startDate)
    {
      initializeDates();
    }

    Real impliedQuote() const override {
      Real d1 = termStructure_->discount(earliestDate_);
      Real d2 = termStructure_->discount(maturityDate_);
      return (d1/d2 - 1.0) / spanningTime_;
    }
    // void setTermStructure(YieldTermStructure* t) override;
  protected:
    void initializeDates() {
      earliestDate_ = startDate_;
      maturityDate_ = startDate_ + spanningTime_;
    }
    RelinkableHandle<YieldTermStructure> termStructureHandle_;
    Date startDate_;
    Time spanningTime_ = 0.25;

};

}  // namespace MiniQL

#endif