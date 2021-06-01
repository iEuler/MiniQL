// g++ example04.cpp ../instruments/swap.cpp ../patterns/observable.cpp -o out

#include <iostream>
#include <vector>

#include "../handle.hpp"
#include "../instruments/swap.hpp"
#include "../typedef.hpp"

int main() {
  MiniQL::Date startDate = 0.0, couponFreq = .25, maturity = 3.0;
  MiniQL::Date couponDate = startDate + couponFreq;
  MiniQL::Real couponRate = 0.1, couponAmount = couponRate * couponFreq;
  MiniQL::Real interestRate = 0.1;
  MiniQL::Swap::Leg leg1, leg2;
  while (couponDate <= maturity) 
  {
    leg1.push_back(std::make_shared<MiniQL::CashFlow>(couponDate, couponAmount));
    couponDate += couponFreq;
  }  
  // MiniQL::Swap::Handle termStructure = 
  //     std::make_shared<MiniQL::YieldTermStructure>(startDate, interestRate);
  MiniQL::Handle<MiniQL::YieldTermStructure> 
      termStructure(std::make_shared<MiniQL::YieldTermStructure>(startDate, interestRate));
  MiniQL::Swap swap(leg1, leg2, termStructure);
  std::cout << swap.NPV() << std::endl;    
}