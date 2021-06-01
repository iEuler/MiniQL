#ifndef MINIQL_OBSERVABLE_HPP
#define MINIQL_OBSERVABLE_HPP

#include <unordered_set>
#include <memory>
#include <iostream>

namespace MiniQL {

class Observer;

class Observable {
    friend class Observer;
  public:
    // NOTE: cannot use shared pointer below. Otherwise the Observer::registerWith
    // function cannot be defined, since it calls o->registerObserver(this);
    typedef std::unordered_set<Observer*> Set_type;
    typedef Set_type::iterator Iterator;

    Observable() = default;
    Observable(const Observable&);
    Observable& operator=(const Observable&);
    virtual ~Observable() = default;

    void notifyObservers();

  private:    
    void registerObserver(Observer*);
    void unregisterObserver(Observer*);
    Set_type observers_;

};

class Observer {    
  public:
    typedef std::unordered_set<std::shared_ptr<Observable>> Set_type;
    typedef Set_type::iterator Iterator;

    Observer() = default;
    Observer(const Observer&);
    Observer& operator=(const Observer&);
    virtual ~Observer();
    
    void registerWith(const std::shared_ptr<Observable>&);
    void unregisterWith(const std::shared_ptr<Observable>&);
    void registerWithObservables(const std::shared_ptr<Observer>&);

    virtual void update() = 0;
    // virtual void update();
    
  private:
    Set_type observables_;
};



// inline member functions

inline Observable::Observable(const Observable& o)
{
  // the observer set is not copied; no observer is asked to register with this object
}

inline Observable& Observable::operator=(const Observable& o)
{
  // as above, the observer set is not copied. 
  // Moreover, observers of this object must be notified of the change
  if (&o != this) {
    // observers_ = o.observers_;
    notifyObservers();
  }
  return *this;
}

inline void Observable::registerObserver(Observer* h)
{
  observers_.insert(h);
}

inline void Observable::unregisterObserver(Observer* h)
{
  observers_.erase(h);
}

inline Observer::Observer(const Observer& h) : observables_(h.observables_) {}


inline Observer& Observer::operator=(const Observer& h)
{
  if (&h != this) {
    for (auto& o : observables_) {
      // o->unregisterObserver(this);
      unregisterWith(o);      
    }
    observables_ = h.observables_;
    for (auto& o : observables_)
      // o->registerObserver(this);
      registerWith(o);

  }
  return *this;
}

inline Observer::~Observer() 
{
  for (auto &o : observables_) {      
      // unregisterWith(o);
      o->unregisterObserver(this);
  }
}

inline void Observer::registerWith(const std::shared_ptr<Observable>& po)
{
  if (po != nullptr) {
    po->registerObserver(this);
    observables_.insert(po);    
  }
}

inline void Observer::unregisterWith(const std::shared_ptr<Observable>& po)
{
  if (po != nullptr) {
    po->unregisterObserver(this);
    observables_.erase(po);
  }
}

inline void Observer::registerWithObservables(const std::shared_ptr<Observer>& ph)
{
  if (ph != nullptr) {
    for (auto &po : ph->observables_)
      registerWith(po);
  }
}


}  // namespace MINIQL
#endif 
