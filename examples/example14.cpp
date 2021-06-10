// test decorator
#include <iostream>

class Shape {
  public:
    virtual void print() const = 0;
};

class Circle : public Shape {
  public:
    void print() const override { std::cout << "Shape = Circle\n"; }
};

class Square : public Shape {
  public:
    void print() const override { std::cout << "Shape = Square\n"; }
};

class BaseDeco : public Shape {
  public:
    BaseDeco(Shape* shape) : shape_(shape) {}
    void print() const override {
      this->shape_->print();
    }
  protected:
    Shape* shape_;    
};

class BlueDeco : public BaseDeco {
  public:
    BlueDeco(Shape* shape) : BaseDeco(shape) {}
    void print() const override {
      std::cout << "Color = Blue, ";
      BaseDeco::print();      
    }  
};

class RedDeco : public BaseDeco {
  public:
    RedDeco(Shape* shape) : BaseDeco(shape) {}
    void print() const override {
      std::cout << "Color = Red, ";
      BaseDeco::print();      
    }  
};

class YellowDeco : public Shape {
  public:
    YellowDeco(Shape* shape) : shape_(shape) {}
    void print() const override {
      std::cout << "Color = Yellow, ";
      shape_->print();      
    }  
  protected:
    Shape * shape_;
};

int main() {
  Shape* c(new Circle);
  Shape* s(new Square);
  c->print();
  s->print();

  Shape* bc(new BlueDeco(c));
  Shape* rs(new RedDeco(s));
  bc->print();
  rs->print();

  Shape* rbc(new RedDeco(bc));
  Shape* brs(new BlueDeco(rs));
  rbc->print();
  brs->print();

  Shape* yc(new YellowDeco(c));
  yc->print();


}