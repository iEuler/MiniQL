#include <cmath>
#include <iostream>
#include <memory>

#include "../exercise.hpp"
#include "../payoff.hpp"
#include "../typedef.hpp"
#include "vanillaoption.hpp"

namespace MiniQL {

void AnalyticEuropeanEngine::calculate() const 
{
  if (arguments_.exercise->type() == Exercise::Type::European) 
  {
    std::shared_ptr<StrikedTypePayoff> payoff = 
        std::dynamic_pointer_cast<StrikedTypePayoff>(arguments_.payoff);
    if (payoff->optionType() == OptionType::Call) 
    {
      Date today = Settings::instance().evaluationDate();
      Date expiry = arguments_.exercise->lastDate();
      Real strike = payoff->strike();
      Real ir = arguments_.interestRate->interestRate();
      Real vol = arguments_.volatility->interestRate();
      Real spot = arguments_.spot;

      Real sigmaSqrtTau = vol * std::sqrt(expiry - today);
      Real d1 = ( std::log(spot/strike) + (ir + vol*vol/2.0) * (expiry - today) ) / sigmaSqrtTau;
      Real d2 = d1 - sigmaSqrtTau;
      Real sqrt2 = std::sqrt(2.0);
      Real Nd1 = (1.0 + std::erf(d1/sqrt2)) / 2.0;
      Real Nd2 = (1.0 + std::erf(d2/sqrt2)) / 2.0;

      Real optionprice = spot * Nd1 - strike * std::exp(-ir*(expiry-today)) * Nd2;
      Real delta = Nd1;

      // write results to results_ variable
      results_.valuationDate = today;
      results_.value = optionprice;
      results_.delta = delta;

      // std::cout << "Vanilla option calculated.\n";

      
    }
    else    
      std::cout << "Put option has not been implemented. " << std::endl;
  }
  else 
    std::cout << "Only European pricing engine is implemented." << std::endl;
  
}

}  // namespace MiniQL
