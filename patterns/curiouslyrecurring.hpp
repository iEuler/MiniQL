#ifndef MINIQL_CURIOSLYRECURRING_HPP
#define MINIQL_CURIOSLYRECURRING_HPP

namespace MiniQL {

template<class Impl>
class CuriouslyRecurringTemplate {
  protected:
    Impl& impl() { return static_cast<Impl&> (*this); }
    const Impl& impl() const { return static_cast<const Impl&> (*this); }
};

}  // namespace MiniQL

#endif 
