// g++ examples/example12.cpp patterns/observable.cpp -o out
#include <iostream>
#include <memory>

#include "../handle.hpp"
#include "../quote.hpp"
#include "../quotes/simplequote.hpp"
#include "../termstructures/yieldtermstructure.hpp"
#include "../termstructures/yield/bootstraptraits.hpp"
#include "../termstructures/yield/piecewiseyieldcurve.hpp"
#include "../termstructures/yield/ratehelper.hpp"
#include "../time/date.hpp"
#include "../typedef.hpp"

int main() {
  MiniQL::Date settlementDate = 0.0;

  std::shared_ptr<MiniQL::Quote> fra1Rate = std::make_shared<MiniQL::SimpleQuote>(0.002930);
  std::shared_ptr<MiniQL::Quote> fra2Rate = std::make_shared<MiniQL::SimpleQuote>(0.003100);
  std::shared_ptr<MiniQL::Quote> fra3Rate = std::make_shared<MiniQL::SimpleQuote>(0.003340);
  std::shared_ptr<MiniQL::Quote> fra4Rate = std::make_shared<MiniQL::SimpleQuote>(0.003620);
  std::shared_ptr<MiniQL::Quote> fra5Rate = std::make_shared<MiniQL::SimpleQuote>(0.003930);

  std::shared_ptr<MiniQL::RateHelper> fra1 = std::make_shared<MiniQL::FraRateHelper>(
      MiniQL::Handle<MiniQL::Quote>(fra1Rate), 0.0);
  std::shared_ptr<MiniQL::RateHelper> fra2 = std::make_shared<MiniQL::FraRateHelper>(
      MiniQL::Handle<MiniQL::Quote>(fra2Rate), 0.25);
  std::shared_ptr<MiniQL::RateHelper> fra3 = std::make_shared<MiniQL::FraRateHelper>(
      MiniQL::Handle<MiniQL::Quote>(fra3Rate), 0.5);
  std::shared_ptr<MiniQL::RateHelper> fra4 = std::make_shared<MiniQL::FraRateHelper>(
      MiniQL::Handle<MiniQL::Quote>(fra4Rate), 0.75);
  std::shared_ptr<MiniQL::RateHelper> fra5 = std::make_shared<MiniQL::FraRateHelper>(
      MiniQL::Handle<MiniQL::Quote>(fra5Rate), 1.0);
  
  std::vector<std::shared_ptr<MiniQL::RateHelper>> euribo6MInstruments;
  euribo6MInstruments.push_back(fra1);
  euribo6MInstruments.push_back(fra2);
  euribo6MInstruments.push_back(fra3);
  euribo6MInstruments.push_back(fra4);
  euribo6MInstruments.push_back(fra5);

  // std::shared_ptr<MiniQL::YieldTermStructure> euribo6MTermStructure = 
  //   std::make_shared<MiniQL::PiecewiseYieldCurve<MiniQL::ZeroYield, MiniQL::Linear>>(
  //       settlementDate, euribo6MInstruments);

  
  auto euribo6MTermStructure = 
    std::make_shared<MiniQL::PiecewiseYieldCurve<MiniQL::ZeroYield, MiniQL::Linear>>(
        settlementDate, euribo6MInstruments);
    
  auto times = euribo6MTermStructure->times();  
  auto dates = euribo6MTermStructure->dates();
  auto data = euribo6MTermStructure->data();

  std::cout << "date, time, zero yield" << std::endl;
  for (std::size_t i = 0; i < times.size(); ++i)
    std::cout << dates[i] << ", " << times[i] << ", " << data[i] << std::endl;

}