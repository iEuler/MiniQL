#ifndef MINIQL_EVENT_HPP
#define MINIQL_EVENT_HPP

#include <optional>

#include "patterns/observable.hpp"
#include "patterns/visitor.hpp"
#include "time/date.hpp"
#include "error.hpp"
#include "settings.hpp"
#include "typedef.hpp"


namespace MiniQL {

  // class AcyclicVisitor;

  class Event : public Observable {
    public:
      ~Event() override = default;

      // return the date at which the event occurs
      virtual Date date() const = 0;

      // virtual bool hasOccurred(const Date& refDate = Date(),
      //                          std::optional<bool> includeRefDate = std::none) const;
      virtual bool hasOccurred(const Date& d = Date(),
                               bool includeRefDate = true) const
      {
        Date refDate = d != Date() ? d : Settings::instance().evaluationDate();
        if (includeRefDate)
          return date() < refDate;
        else
          return date() <= refDate;
      }

      virtual void accept(AcyclicVisitor& v) {
        auto* v1 = dynamic_cast<Visitor<Event>*>(&v);
        if (v1 != nullptr)
          v1->visit(*this);
        // else
        //   QL_FAIL("not an event visitor.");
      }

  };

  namespace detail {

    class simple_event : public Event {
      public:
        simple_event(const Date& date) : date_(date) {}
        Date date() const override { return date_; }
      private:
        Date date_;
    };
  
  }

}  // namespace MiniQL


#endif 
