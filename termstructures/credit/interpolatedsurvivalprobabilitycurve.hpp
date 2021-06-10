#ifndef MINIQL_INTERPOLATEDSURVIVALPROBABILITYCURVE_HPP
#define MINIQL_INTERPOLATEDSURVIVALPROBABILITYCURVE_HPP

#include <cmath>
#include <vector>
#include <iostream>

#include "../../error.hpp"
#include "../../interestrate.hpp"
#include "../interpolatedcurve.hpp"
#include "../yieldtermstructure.hpp"
#include "../../typedef.hpp"
#include "../../time/date.hpp"
#include "../../math/linearinterpolation.hpp"
#include "zeroyieldstructure.hpp"

namespace MiniQL {

template<class Interpolator>
class InterpolatedSurvivalProbabilityCurve 
  : public SurvivalProbabilityStructure,
    public InterpolatedCurve<Interpolator> {

};

}  // namespace MiniQL

#endif