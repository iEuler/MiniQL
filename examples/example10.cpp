// this file to test interpolation
// g++ examples/example10.cpp patterns/observable.cpp interestrate.cpp -o out

#include <iostream>
#include <vector>

#include "../typedef.hpp"
#include "../math/linearinterpolation.hpp"
#include "../math/interpolation.hpp"

#include "../settings.hpp"
#include "../termstructures/yield/zerocurve.hpp"

int main() {
  std::vector<MiniQL::Real> xTrain{1.0, 2.0, 2.5, 3.0}, yTrain{1.0, 4.0, 6.25, 9.0};
  MiniQL::Interpolation interpolation = MiniQL::Linear().interpolate(xTrain.begin(), xTrain.end(), yTrain.begin());

  std::vector<MiniQL::Real> xTest{0.6, 1.0, 1.4, 1.8, 2.2, 2.6, 3.0, 3.4}, yTest, yTestDerivative;

  for (auto x : xTest) {
    yTest.push_back(interpolation(x));
    yTestDerivative.push_back(interpolation.derivative(x));
  }

  for (std::size_t i = 0; i < xTest.size(); ++i) {
    std::cout << xTest[i] << ", " << yTest[i] << ", " << yTestDerivative[i] << std::endl;
  }

  // MiniQL::Date today = 0.0;  
  std::vector<MiniQL::Date> dates{100, 365, 2*365, 900, 3*365};
  std::vector<MiniQL::Rate> yields{0.005, 0.01, 0.02, 0.03, 0.04};
  MiniQL::InterpolatedZeroCurve<MiniQL::Linear> zero(dates, yields);

  auto dates_out = zero.dates();
  auto times_out = zero.times();
  auto data_out = zero.data();
  
  

  for (auto x : dates_out) std::cout << x << ", ";
  std::cout << std::endl;
  for (auto x : times_out) std::cout << x << ", ";
  std::cout << std::endl;
  for (auto x : data_out) std::cout << x << ", ";
  std::cout << std::endl;
  std::vector<MiniQL::Real> interpolated_data;
  for (auto x : xTest) {
    interpolated_data.push_back(zero.zeroYieldImpl(x));    
  }
  for (std::size_t i = 0; i < xTest.size(); ++i) {
    std::cout << xTest[i] << ", " << interpolated_data[i] << std::endl;
  }

}