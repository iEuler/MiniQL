// this file to test interpolation
// g++ examples/example10.cpp patterns/observable.cpp -o out

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

  MiniQL::Date today = 0.0;  
  MiniQL::InterpolatedZeroCurve<MiniQL::Linear> zero;

}