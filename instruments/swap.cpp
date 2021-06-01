#include "swap.hpp"

namespace MiniQL {

bool Swap::isExpired() const 
{
  Date settlement = termStructure_->referenceDate();
  for (auto &pcf : firstLeg_) {
    if (!pcf->hasOccured(settlement)) 
      return false;
  }
  for (auto &pcf : secondLeg_) {
    if (!pcf->hasOccured(settlement)) 
      return false;
  }
  return true;
}


void Swap::setupExpired() const
{
  Instrument::setupExpired();
  // other code related to cache variables in Swap class
}

void Swap::performCalculations() const
{
  Real npv1 = 0.0, npv2 = 0.0;
  Real ir = termStructure_->interestRate();
  Date t0 = termStructure_->referenceDate();
  for (auto &pcf : firstLeg_) {
    if (!pcf->hasOccured(t0)) 
      npv1 += std::exp( - ir * (pcf->date() - t0) ) * pcf->amount();            
  }
  for (auto &pcf : secondLeg_) {
    if (!pcf->hasOccured(t0)) 
      npv2 += std::exp( - ir * (pcf->date() - t0) ) * pcf->amount();      
  }
  NPV_ = - npv1 + npv2;
}

}  // namespace MiniQL