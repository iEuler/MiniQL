#ifndef MINIQL_BOOTSTRAPERROR_HPP
#define MINIQL_BOOTSTRAPERROR_HPP

#include <memory>

#include "../error.hpp"
#include "../typedef.hpp"

// #include "../termstructure.hpp"

namespace MiniQL {
  template<class Curve>
  class BootstrapError {
    public:
      typedef typename Curve::traits_type Traits;
      
      BootstrapError(const Curve* curve, 
                     std::shared_ptr<typename Traits::helper> instrument,
                     Size segment)
      : curve_(curve), helper_(std::move(helper)), segment_(segment) { }

      Real operator()(Rate guess) const 
      {
        curve_->interpolation_.update();
        return helper_->quoteError();
      }
      const std::shared_ptr<typename Traits::helper>& helper() { return helper_; }
    
    private:
      const Curve* curve_;
      const std::shared_ptr<typename Traits::helper> helper_;
      const Size segment_;

  };

}  // namespace MiniQL

#endif