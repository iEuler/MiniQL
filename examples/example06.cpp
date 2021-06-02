// g++ example06.cpp ../instruments/vanillaoption.cpp ../patterns/observable.cpp -o out

#include <iostream>
#include <memory>
#include <vector>

#include "../handle.hpp"
#include "../instruments/vanillaoption.hpp"
#include "../patterns/observable.hpp"
#include "../pricingengine.hpp"
#include "../typedef.hpp"

int main() {
  MiniQL::Date today = 0.2, expiry = 1.0;  
  MiniQL::Real spot = 1.02, strike = 1.1;
  MiniQL::Real ir = 0.1, vol = 0.12;
    
  MiniQL::Settings::instance().evaluationDate() = today;
  
  std::shared_ptr<MiniQL::StrikedTypePayoff> payoff = 
      std::make_shared<MiniQL::StrikedTypePayoff>(MiniQL::OptionType::Call, strike);
  std::shared_ptr<MiniQL::Exercise> exercise = 
      std::make_shared<MiniQL::Exercise>(MiniQL::Exercise::Type::European, 
                                         std::vector<MiniQL::Date>{expiry});
  // MiniQL::StrikedTypePayoff payoff(MiniQL::OptionType::Call, strike);  
  // MiniQL::Exercise exercise(MiniQL::Exercise::Type::European, std::vector<Date>{expiry});

  MiniQL::Handle<MiniQL::YieldTermStructure> 
      interestRate(std::make_shared<MiniQL::YieldTermStructure>(today, ir));
  MiniQL::Handle<MiniQL::YieldTermStructure> 
      volatility(std::make_shared<MiniQL::YieldTermStructure>(today, vol));

  // instrument  
  MiniQL::VanillaOption vanillaoption(payoff, exercise, interestRate, volatility, spot);

  // engine
  std::shared_ptr<MiniQL::AnalyticEuropeanEngine> engine(std::make_shared<MiniQL::AnalyticEuropeanEngine>());
  vanillaoption.setPricingEngine(engine);
  
  std::cout << "Hello" << std::endl;      
  std::cout << vanillaoption.valuationDate() << std::endl; 
  std::cout << vanillaoption.NPV() << std::endl; 
  std::cout << vanillaoption.delta() << std::endl; 
  
}