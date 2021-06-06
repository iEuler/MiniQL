#ifndef MINIQL_BOOTSTRAPHELPER_HPP
#define MINIQL_BOOTSTRAPHELPER_HPP

#include <iostream>
#include <memory>

// #include "../termstructure.hpp"

#include "../error.hpp"
#include "../handle.hpp"
#include "../patterns/observable.hpp"
#include "../quote.hpp"
#include "../quotes/simplequote.hpp"
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
  class BootstrapHelper : public Observalbe, public Observer {
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
      Handel<Quote> quote_;
      TS* termStructure_ = nullptr;
      Date earliestDate_, latestDate_;
      Date maturityDate_;
  };


}  // namespace MiniQL

#endif