#ifndef MINIQL_HANDLE_HPP
#define MINIQL_HANDLE_HPP

#include <memory>
#include "patterns/observable.hpp"

namespace MiniQL {

template<typename T>
class Link : public virtual Observable, 
             public virtual Observer {
  public:    
    explicit Link(const std::shared_ptr<T>& h, bool isObserver);
    virtual ~Link() = default;

    void linkTo(const std::shared_ptr<T>& h, bool isObserver);
    void empty() const { return !h_; }
    const std::shared_ptr<T>& currentLink() const { return h_; }
    void update() override { notifyObservers(); }

  private:
    std::shared_ptr<T> h_;
    bool isObserver_;
};

template<typename T>
class Handle {
  public:
    
    explicit Handle(const std::shared_ptr<T>& p = std::shared_ptr<T>(),
                    bool isObserver = true) 
        : link_(std::make_shared<Link<T>>(p, isObserver)) {};
    ~Handle() = default;

    // dereferencing
    const std::shared_ptr<T>& currentLink() const { return link_->currentLink(); }
    const std::shared_ptr<T>& operator->() const { return link_->currentLink(); }
    const std::shared_ptr<T>& operator*() const { return link_->currentLink(); }
    bool empty() const { return link_->empty(); }
    // allows registration as observable
    operator std::shared_ptr<Observable>() const { return link_; }
    // also need operator==, !=, <
    
  protected:
    std::shared_ptr<Link<T>> link_;
};

template<typename T>
class RelinkableHandle : public Handle<T> {
  public:
    explicit RelinkableHandle(
        const std::shared_ptr<T>& p = std::shared_ptr<T>(),
        bool isObserver = true)
      : Handle<T>(p, isObserver) {}
    explicit RelinkableHandle(
        T* p,
        bool isObserver = true)
      : Handle<T>(p, isObserver) {}
    ~RelinkableHandle() = default;

    void linkTo(
        const std::shared_ptr<T>& p = std::shared_ptr<T>(),
        bool isObserver = true)
    {
      this->link_->linkTo(p, isObserver); 
      // NOTE: has to use this->. linkTo functions are defined in both classes.
    }
};


template<typename T>
inline Link<T>::Link(const std::shared_ptr<T>& h, bool isObserver) 
    : isObserver_(false)
{
  linkTo(h, isObserver);
}

template<typename T>
inline void Link<T>::linkTo(const std::shared_ptr<T>& h, bool isObserver) 
{
  if ((h_ != h) || (isObserver != isObserver_)) {
    if (h_ && isObserver_)
      unregisterWith(h_);
    h_ = h;
    isObserver_ = isObserver;
    if (h_ && isObserver_)
      registerWith(h_);
    notifyObservers();    
  }
}


}  // namespace MiniQL

#endif 
