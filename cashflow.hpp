#ifndef MINIQL_CASHFLOW_HPP
#define MINIQL_CASHFLOW_HPP

#include <memory>
#include <optional>
#include <vector>

#include "patterns/visitor.hpp"
#include "time/date.hpp"
#include "event.hpp"
#include "typedef.hpp"

namespace MiniQL {

class CashFlow : public Event {
  public:
    CashFlow() = default;
    // CashFlow(const Date& date, const Real& amount) : date_(date), amount_(amount) {}
    ~CashFlow() override = default;

    Date date() const override = 0;
    virtual Real amount() const = 0;

    // bool hasOccured(const Date& refDate = Date(),
    //                 bool includeRefDate = true) const {}
    
    void accept (AcyclicVisitor& v) override {
      auto* v1 = dynamic_cast<Visitor<CashFlow>*>(&v);
      if (v1 != nullptr)
        v1->visit(*this);
      else
        Event::accept(v);
    }

  // private:
  //   Date date_ = 0;
  //   Real amount_ = 0.0;
};

typedef std::vector<std::shared_ptr<CashFlow> > Leg;

}  // namespace MiniQL


#endif 
