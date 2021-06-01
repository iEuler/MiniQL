#ifndef MINIQL_LAZYOBJECT_HPP
#define MINIQL_LAZYOBJECT_HPP

#include "observable.hpp"

namespace MiniQL {

class LazyObject : public virtual Observable, 
                   public virtual Observer {
  public:
    LazyObject() : calculated_(false) {};
    ~LazyObject() = default;

    void update() override { calculated_ = false; }    
  protected:    
    virtual void calculate() const;
    virtual void performCalculations() const = 0;
    mutable bool calculated_;
};

inline void LazyObject::calculate() const 
{
  if (!calculated_) {
    calculated_ = true;
    try {
      performCalculations();    // will call this function in derived class
    } catch (...) {
      calculated_ = false;
      throw;
    }
    
  }
}

}  // namespace MiniQL

#endif 
