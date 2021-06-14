// test visitor

#include <iostream>
#include <memory>
#include <vector>

#include "../error.hpp"

class AcyclicVisitor {
  public:
  virtual ~AcyclicVisitor() = default;
};

template<typename T>
class Visitor {
  public:
    virtual void visit(T&) const = 0;
    virtual ~Visitor() = default;
};

class Base {
  public:
    int value_ = 0;
    virtual void accept(AcyclicVisitor& v) {
      auto *v1 = dynamic_cast<Visitor<Base>*>(&v);
      if (v1 != nullptr)
        v1->visit(*this);
    }
};

class DerivedA : public Base {
  public:
    int a_ = 1;
  void accept(AcyclicVisitor& v) override {
      auto *v1 = dynamic_cast<Visitor<DerivedA>*>(&v);
      if (v1 != nullptr)
        v1->visit(*this);
    }
};

class DerivedB : public Base {
  public:
    int b_ = 1;
    void accept(AcyclicVisitor& v) override {
      auto *v1 = dynamic_cast<Visitor<DerivedB>*>(&v);
      if (v1 != nullptr)
        v1->visit(*this);
    }
};

class DerivedA2 : public DerivedA {
  public:
    int a2_ = 1;
    // void accept(AcyclicVisitor& v) override {
    //   auto *v1 = dynamic_cast<Visitor<DerivedA2>*>(&v);
    //   if (v1 != nullptr)
    //     v1->visit(*this);
    // }
};

class Aggregate : public AcyclicVisitor,
                  public Visitor<Base>,
                  public Visitor<DerivedA>,
                  public Visitor<DerivedB>,
                  public Visitor<DerivedA2> {
  public:
  mutable int base = 0, derivedA = 0, derivedB = 0, derivedA2 = 0;
  
  Aggregate() = default;

  void visit(Base&) const override {
    base += 1;
  }

  void visit(DerivedA&) const override {
    derivedA += 1;
  }

  void visit(DerivedB&) const override {
    derivedB += 1;
  }

  void visit(DerivedA2&) const override {
    derivedA2 += 1;
  }

  void print() {
    std::cout << "base = " << base
              << ", derivedA = " << derivedA
              << ", derivedB = " << derivedB
              << ", derivedA2 = " << derivedA2
              << std::endl;
  }

};

int main() {
  Aggregate agg;
  auto b = std::make_shared<Base>();
  auto dA = std::make_shared<DerivedA>();
  auto dB = std::make_shared<DerivedB>();
  auto dA2 = std::make_shared<DerivedA2>();

  std::vector<std::shared_ptr<Base>> v;
  v.push_back(b);
  v.push_back(dA);
  v.push_back(dB);
  v.push_back(dA2);

  for (auto x : v) x->accept(agg);

  agg.print();

  QL_FAIL("Test fail message");
  std::cout << "done" << std::endl;

}