#ifndef MINIQL_VISITOR_HPP
#define MINIQL_VISITOR_HPP

#include <memory>

namespace MiniQL {

  class AcyclicVisitor {
    public:
      virtual ~AcyclicVisitor() = default;
  };

  // Visitor for a specific class
  template <class T>
  class Visitor {
    public:
      virtual ~Visitor() = default;
      virtual void visit(T&) = 0;
  };

}  // namespace MiniQL

#endif 
