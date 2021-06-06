#ifndef MINIQL_ITERATIVEBOOTSTRAP_HPP
#define MINIQL_ITERATIVEBOOTSTRAP_HPP

#include <cmath>
#include <memory>
#include <vector>

#include "../../error.hpp"
#include "../../typedef.hpp"
#include "../../math/brent.hpp"
#include "../../math/secant.hpp"

// TODO: ipmlement initialize(), setup(), calculate()

namespace MiniQL {

template<class Curve>
class IterativeBootstrap {    
  public:
    typedef typename Curve::traits_type Traits;
    typedef typename Curve::interpolator_type Interpolator;

    IterativeBootstrap(Real accuracy = QL_EPSILON,
                       Real minValue = Real(),
                       Real maxValue = Real())
    : accuracy_(accuracy), minValue_(minValue), maxValue_(maxValue) { }
    
    void setup(Curve* ts);
    void calculate() const;

  private:
    void initialize() const;
    Real accuracy_;
    Real minValue_, maxValue_;
    // Size maxAtteampts_;
    // Real maxFactor_;
    // Real minFactor_;
    Curve* ts_ = nullptr;
    Size n_;
    Brent firstSovler_;
    Secant solver_;
    mutable bool initialized_ = false, validCurve_ = false, loopRequired_;
    mutable Size firstAliveHelper_, alive_;
    mutable std::vector<Real> previousData_;    

};

}  // namespace MiniQL

#endif