#ifndef MINIQL_TERMSTRUCTURE_HPP
#define MINIQL_TERMSTRUCTURE_HPP

#include "time/date.hpp"
#include "patterns/observable.hpp"

namespace MiniQL {
  class TermStructure : public virtual Observable,
                        public virtual Observer {
    public:
      TermStructure() = default;
      TermStructure(const Date& referenceDate,
                    const Calendar& calendar = Calendar(),
                    const DayCounter& dc = DayCounter())
        : referenceDate_(referenceDate),
          calendar_(calendar),
          dayCounter_(dc) { }

      ~TermStructure() override = default;

      virtual const Date& referenceDate() const { return referenceDate_; }
      virtual const Date& maxDate() const = 0;

      void update() override { notifyObservers(); }
          
    protected:
      // bool moving_ = true;
      mutable Date referenceDate_;
      // mutable bool updated_ = true;
      Calendar calendar_;
      DayCounter dayCounter_;

  };

   
  
}  // namespace MiniQL

#endif