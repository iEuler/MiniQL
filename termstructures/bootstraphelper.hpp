#ifndef MINIQL_BOOTSTRAPHELPER_HPP
#define MINIQL_BOOTSTRAPHELPER_HPP

#include <iostream>
#include <memory>
#include <cmath>

// #include "../termstructure.hpp"

#include "../error.hpp"
#include "../handle.hpp"
#include "../patterns/observable.hpp"
#include "../quote.hpp"
#include "../quotes/simplequote.hpp"
#include "../settings.hpp"
#include "../time/date.hpp"
#include "../typedef.hpp"

namespace MiniQL {

  // struct Pillar {
  //   enum Choice {
  //     MaturityDate,
  //     LastRelevantDate,
  //     CurstomDate
  //   };
  // };

  // std::ostream& operator<<(std::ostream&, Pillar::Choice);

  template<class TS>
  class BootstrapHelper : public Observable, public Observer {
    public:
      explicit BootstrapHelper(Handle<Quote> quote) 
      : quote_(std::move(quote)) 
      {
        registerWith(quote_);
      }
      explicit BootstrapHelper(Real quote)
      : quote_(Handle<Quote>(std::shared_ptr<Quote>(new SimpleQuote(quote)))) {}
      ~BootstrapHelper() override = default;

      const Handle<Quote>& quote() const { return quote_; }
      virtual Real impliedQuote() const = 0;
      Real quoteError() const { return quote_->value() - impliedQuote(); }

      virtual void setTermStructure(TS* t) { termStructure_ = t; }
      virtual Date earliestDate() const { return earliestDate_; }
      virtual Date maturityDate() const { return maturityDate_; }
      virtual Date latestDate() const { return latestDate_; }

      void update() override { notifyObservers(); }

    protected:
      Handle<Quote> quote_;
      TS* termStructure_ = nullptr;
      Date earliestDate_, latestDate_;
      Date maturityDate_;
  };

  class BootstrapHelperSorter {
    public:
      template<class Helper>
      bool operator()(const std::shared_ptr<Helper>& h1,
                      const std::shared_ptr<Helper>& h2) const
      {
        return h1->maturityDate() < h2->maturityDate();
      }
  };

  // template<typename TS>
  // class RaletiveDateBootstrapHelper : public BootstrapHelper<TS> {
  //   public:
  //     explicit RaletiveDateBootstrapHelper(const Handle<Quote>& quote);
  //     explicit RaletiveDateBootstrapHelper(Real quote);

  //     void update() override {
  //       Date d = Settings::instance().evaluationDate();
  //       if (std::abs(evaluationDate_ - d) > QL_EPSILON)
  //       {
  //         evaluationDate_ = d;
  //         initializeDates();
  //       }
  //       BootstrapHelper<TS>::update();
  //     }
    
  //   protected:
  //     virtual void initializeDates() = 0;
  //     Date evaluationDate_;
  // };


}  // namespace MiniQL

#endif