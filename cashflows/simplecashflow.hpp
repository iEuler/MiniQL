#ifndef MINIQL_SIMPLECASHFLOW_HPP
#define MINIQL_SIMPLECASHFLOW_HPP

#include <memory>
#include <optional>
#include <vector>

#include "../patterns/visitor.hpp"
#include "../time/date.hpp"
#include "../cashflow.hpp"
#include "../error.hpp"
#include "../typedef.hpp"

namespace MiniQL {

class SimpleCashFlow : public CashFlow {
  public: 
    SimpleCashFlow(Real amount, const Date& date) : amount_(amount), date_(date) {}
    ~SimpleCashFlow() override = default;

    Date date() const override { return date_; }
    Real amount() const override { return amount_; }

    void accept(AcyclicVisitor& v) override {
      auto* v1 = dynamic_cast<Visitor<SimpleCashFlow>*>(&v);
        if (v1 != nullptr)
          v1->visit(*this);            
        else
          CashFlow::accept(v);
    }

  protected:
    Real amount_;
    Date date_;
};

}  // namespace MiniQL

#endif
